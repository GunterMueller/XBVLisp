/*
 *    glMemory.c
 * Librairie de la gestion de la memoire des objets
 * utilises pour l'interface entre Xbvl et GL (Iris)
 * D.Ploix, decembre 93
 */


#if (OPEN_GL | MESA_GL)
#include "vlisp.h"

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "glXbvl.h"

#ifdef STDC_HEADERS
extern void *calloc();
extern short my_pushname();
#else
extern void *calloc(unsigned, unsigned);
extern short my_pushname(short);
#endif


/* *************************************************** *
 * Procedure d'initialisation du mode d'interaction GL, 
 * Principalement l'allocation de la memoire d'objets
 * GL accessibles depuis LISP
 * *************************************************** */

Int glLibInit() 
{
  Int *tmp;

  aux1 = (Int) (bgl = (GLObjectPtr) (hpil+40)); 
  aligner(aux1); 
  bgl=(GLObjectPtr) aux1; /* on cale */

  /* construction de la free list */
  for(hgl = bgl; hgl < bgl+GL_OBJECT_MEMORY_SIZE-1; hgl++) hgl->atome = (Int *) (hgl+1);
  hgl->atome =NULL;
  free_gl = bgl;
  hgl = bgl + GL_OBJECT_MEMORY_SIZE - 1;
}

/* erreur d'allocation d'objet GL */
void 
ergl()
{
  wst("Plus d'objets GL disponibles");
}

/* creation d'un element GL (gestion de la free list) */
GLObjectPtr 
cregl()
{
  GLObjectPtr cur;
  /* en premier lieu verification de l'etat de la pile d'objets GL */
  if(!free_gl) gci(); 
  if(!free_gl) {ergl(); return NULL;}
  cur=free_gl;
  free_gl=(GLObjectPtr)free_gl->atome;
  return cur;
}
  
/* restitution d'un objet a la free list */
void 
freegl(obj)
GLObjectPtr obj;
{
  if(obj->callback_number) {
    Int i;
    for(i = 0; i < obj->callback_number; i++)
      XtFree(obj->callback_expression[i]);
    XtFree((char *) obj->callback_expression);
    XtFree((char *) obj->callback_event);
  }
  if(!free_gl) {free_gl = obj; obj->atome = NULL;}
  else 
    {
      obj->atome = (Int *) free_gl;
      free_gl = obj;
    }
}
                                          /* ******************************************** */
                                          /* Acces au noms (pour une selection graphique) */
                                          /* ******************************************** */

Int gl_loadnom() /* (GLloadname num) */
{
  if(!wobj || !isnum(a1)) { a1 = nil; derec;}
  glLoadName(valnb(a1));
  a1=t;
  derec;
}

short 
my_pushname(name)
short name;
{
  if(name < 0) name = glGenLists(1);
  glPushName(name);
  return name;
}

Int gl_pushnom() /* (GLpushname [numero]) */
{
  if(!wobj) derec;
  if(!isnum(a1)) a1 = cranb(my_pushname(-1));
  else           my_pushname(valnb(a1));
  derec;
}

Int gl_popnom() /* (GLpopname) */
{
  if(!wobj) derec;
  glPopName();
  derec;
}

/* Demande d'un nouveau tag */
Int gl_mygentag()
{
  if(!wobj) derec;
  a1 = cranb(glGenLists(1));
  derec;
}


                                /* *********************************************************** */
                                /* Routines liees a la gestion des Listes (specifique Open GL) */
                                /* *********************************************************** */

Int gl_islist()
{
  Int l_num;
  if(!isnum(a1)) { a1 = nil ; derec; }
  l_num = valnb(a1);
  a1 = (glIsList(l_num) ? t : nil);
  derec;
}
  

Int gl_setrootlist() /* (GLrootlist [num] [widget]) : fixe la liste root ... : SUBR2 */
{
  GLuint lst;
  Widget w;
  GLObjectPtr tmp_w;
  
  if(!wobj) derec;
  if(!isnum(a1)) {a1 = cranb(wobj->root_list); derec; }
  lst = (GLuint) valnb(a1);
  if(isWidget(a2) && (w = GET_WIDGET_FROM_ATOME(a2))) {
      for(tmp_w = bgl; tmp_w < hgl; ) {
	if((Widget)tmp_w->w == w) break ;
	tmp_w++;
      }
      if(tmp_w >= hgl) {a1 = nil; derec;}
    }
  else tmp_w = wobj;
  a1 = cranb((tmp_w->root_list? tmp_w->root_list : 0)) ;
  tmp_w->root_list = lst;
  redessine();
  derec;
}

Int gl_genlist() /* (GLgenlist [range]) */
{
  Int range = 1;
  if(!wobj) derec;
  if(isnum(a1)) range = valnb(a1);
  a1 = cranb(glGenLists(range));
  derec;
}


Int gl_newlist() /* (GLnewlist [num]) : creation ou remplacement de la liste num : SUBR1 */
{
  GLuint l_num;

  if(!wobj) derec;
  if(a1 == nil) l_num = glGenLists(1);
  else l_num = (unsigned int) valnb(a1);
  
  glNewList(l_num, GL_COMPILE);

  a1 = cranb(l_num);
  derec;
}

Int gl_endlist() /* (GLendlist) : ferme la liste courante : SUBR0 */
{
  if(!wobj) derec;
  glEndList();
  if(wobj->root_list > 0) redessine();
  a1=t;
  derec;
}


Int gl_dellist() /* (GLdellist num [range]) : efface la liste num : SUBR2 */
{
  if(!wobj || a1 == nil) derec;
  glDeleteLists((GLuint) valnb(a1), (isnum(a2) ? (GLuint) valnb(a2) : 1));
  a1=t;
  derec;
}

Int gl_calllist() /* (GLcalllist num) : dessin de la liste num : SUBR1 */
{
  GLuint list = valnb(a1);
  if(!wobj || a1 == nil) derec;
  
  glCallList((GLuint) list);
  a1 = t;
  derec;
}


Int gl_pushnewlist() /* (GLpushnewlist num1 num2) */
{
  if(!wobj || a1 == nil || a2 == nil) derec;
  glNewList((GLuint) valnb(a1),GL_COMPILE);
  glCallList((GLuint) valnb(a2));
  glEndList();
  glNewList((GLuint) valnb(a2),GL_COMPILE);
  derec;
}

#endif /* OPEN_GL | MESA_GL */


