/* Ce fichier contiend les ajouts d'operations sur les listes propres a Xbvlisp
   Les nouvelles fonctionnalites sont :
   - mesaCopyFlatten <l_num> <new_l_num> : copie le contenue de la liste l_num
     sans les appels a des sous-listes (applatissement de la liste)
   - mesaWriteList <file_des> <l_num> : ecrit dans file_des (descripteur de fichier)
     le contenu de la liste l_num
   - mesaReadList <file_des> <l_num> : cree la liste l_num a partir des instructions
     graphiques sauvegardees dans file_des
*/

#ifdef MODIFIEDMESA
#include "vlisp.h"

#ifdef STDC_HEADERS
extern Int mesa_xbvl_link();
#else
extern Int mesa_xbvl_link(char, Int);
#endif

static int OPCODE_END_OF_LIST = -1, OPCODE_SIZE;
static int OPCODE_CALL_LIST, OPCODE_CALL_LIST_OFFSET;
static unsigned int *OPCODE_OFFSET;
typedef Int Node;

#define OPCODE_CONTINUE     (OPCODE_END_OF_LIST - 1)
static int OPCODE_NEWLIST;

#define InstSize(opcode)   (OPCODE_OFFSET[opcode])
#define get_list_node(list) (Node *) mesa_xbvl_link(3, list)
#define get_list_offset_node(list) (Node *) mesa_xbvl_link(31, list)
#define mesa_new_list(list) mesa_xbvl_link(4, list)
#define mesa_end_list()     mesa_xbvl_link(5, 0)
#define mesa_new_node(opcode) mesa_xbvl_link(6, opcode)


/* Initialisation du lien entre MESA et Xbvl */
static void init_mesa_xbvl_link()
{
  
  OPCODE_SIZE = mesa_xbvl_link(0, 0);
  OPCODE_END_OF_LIST = mesa_xbvl_link(1, 0);
  OPCODE_CALL_LIST = mesa_xbvl_link(11, 0);
  OPCODE_CALL_LIST_OFFSET = mesa_xbvl_link(12, 0);
  OPCODE_OFFSET = (int *) mesa_xbvl_link(2, 0);
  OPCODE_NEWLIST = OPCODE_END_OF_LIST+1;
}

static mesa_do_copy(Node *n, int is_flatten)
{
  Node *dest;

  while(n) {
    if(*n == (Node) OPCODE_END_OF_LIST) break;

    if(*n == (Node) OPCODE_CONTINUE) {
      n = (Node *) *(n+1);
      continue;
    }

    else if(is_flatten && *n == OPCODE_CALL_LIST) {
      mesa_do_copy(get_list_node(*(n+1)), is_flatten);
    }
    else if(is_flatten && *n == OPCODE_CALL_LIST_OFFSET) {
      mesa_do_copy(get_list_offset_node(*(n+1)), is_flatten);
    }
    else {
      dest = (Node *) mesa_new_node(*n);
      memcpy((void *) dest, (void *) n, (InstSize(*n) * sizeof(Int)));
    }
    n += InstSize(*n);
  }
}

extern Int mesa_write_list();

Int mesaCopy() /* (MESAcopylist liste_origine liste_destination is_flatten) SUBR3 */
{
  int i, n_s;

  if(OPCODE_END_OF_LIST < 0) init_mesa_xbvl_link();

  mesa_new_list(valnb(a2));
  mesa_do_copy((Node *) get_list_node(valnb(a1)), a3 == t);
  mesa_end_list();
  a1 = t;
  derec;
}


int mesa_write_list()
{
  Node *n;
  FILE *fp;
  pop_cast(fp, (FILE *));
  pop_cast(n, (Node *));
  fwrite(&OPCODE_NEWLIST, sizeof(Int), 1, fp);
  fwrite((n+1), sizeof(Int), 1, fp);
  (void)mesa_write(fp, get_list_node(*(n+1)), t);
  derec;
}

void mesa_write(FILE *fp, Node *n, Int *save_listes)
{
  while(n) {

    if(*n == OPCODE_END_OF_LIST) break;

    if(*n == OPCODE_CONTINUE) {
      n = (Node *) *(n+1);
      continue;
    }

    if(*n == OPCODE_CALL_LIST) {
      if(save_listes == nil) mesa_write(fp, get_list_node(*(n+1)), save_listes);
      else { 
	push(n); push(fp); pushf(mesa_write_list); 
	fwrite(n, sizeof(Int), InstSize(*n), fp);
      }
    }

    else if(*n == OPCODE_CALL_LIST_OFFSET) 
      mesa_write(fp, get_list_offset_node(*(n+1)), save_listes);

    else fwrite(n, sizeof(Int), InstSize(*n), fp);

    n += InstSize(*n);
  }
}

Int mesaWriteList() /* (MESAwritelist f_des l_num [save_listes]) SUBR 3 */
{
  Node *n;
  Int l_num = valnb(a2);
  int i, s;
  FILE *fp;

  if(OPCODE_END_OF_LIST < 0) init_mesa_xbvl_link();

  fp = (FILE *) valnb(a1);
  n = (Node *) get_list_node(l_num);
  fwrite(&OPCODE_NEWLIST, sizeof(Int), 1, fp);
  fwrite(&l_num, sizeof(Int), 1, fp);
  mesa_write(fp, n, a3);
  a1 = t;
  derec;
}

Int mesaReadList() /* (MESAreadlist f_des [l_num]) */
{
  Node *n;
  Int opc, nlst = -1;
  FILE *fp;
  int cc;

  if(OPCODE_END_OF_LIST < 0) init_mesa_xbvl_link();

  fp = (FILE *) valnb(a1);

  while((cc = fread(&opc, sizeof(Int), 1, fp))) {

    if(opc == OPCODE_END_OF_LIST) break;

    if(opc == OPCODE_NEWLIST) {
      if(nlst >= 0) {
	fseek(fp, (long) (-1 * sizeof(Int)), SEEK_CUR); 
	break;
      }
      fread(&nlst, sizeof(Int), 1, fp);
      if(isnum(a2)) nlst = valnb(a2);
      mesa_new_list(nlst);
    }
    else {
      n = (Node *) mesa_new_node(opc);
      fread(n+1, sizeof(Int), (InstSize(opc)-1), fp);
    }
  }

  if(nlst >= 0) { mesa_end_list(); a1 = cranb(nlst); }
  else a1 = nil;
  derec;
}

Int mesaGetList() /* (MESAgetlist f_des) */
{
  Int opc, *lst = NULL, nlst;
  FILE *fp;
  int cc;

  if(OPCODE_END_OF_LIST < 0) init_mesa_xbvl_link();

  fp = (FILE *) valnb(a1);
  a1 = nil;

  while((cc = fread(&opc, sizeof(Int), 1, fp))) {

    if(opc == OPCODE_END_OF_LIST) break;

    if(opc == OPCODE_NEWLIST) {
      fread(&nlst, sizeof(Int), 1, fp);
      if(!lst) lst = a1 = ncons(cranb(nlst));
      else { 
	*CDR(lst) = ncons(cranb(nlst));
	lst = cdr(lst);
      }
    }
    else fseek(fp, (long) ((InstSize(opc)-1) * sizeof(Int)), SEEK_CUR);
  }

  rewind(fp);

  derec;
}

  

#endif /* modified mesa */
      
      
      
  

  
  
