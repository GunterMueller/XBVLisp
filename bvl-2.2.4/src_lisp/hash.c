/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	                hash.c		
 *      pour faire de hash-coding sur les noms des atomes.
 *          Ernesto Sendoya, mai 1992
 *			
 */

#include <vlisp.h>
#include <stdio.h>
#include <string.h>


#define PAK_NAME_FORMAT "%s|%s"



/* 
 * fonction de Hash-coding prit du dragon book
 */

struct atome *hash_atable[NHASH_PRIME];

/* Verification du remplissage de la Hash Table */
Int check_hash()
{
  Int i;
  long n_cases = 0, max_prof = 0, tmp_prof, n_atome = 0;
  ATOMEPTR tmp;

  for(i = 0; i < NHASH_PRIME; i++) 
    if(hash_atable[i]) {
      ++n_cases;
      for(tmp_prof = 0, tmp = hash_atable[i]; 
	  tmp && tmp != (ATOMEPTR)nil; 
	  tmp = tmp->next_at, ++tmp_prof, ++n_atome) ;
      if(tmp_prof > max_prof) max_prof = tmp_prof;
    }
  wint(n_cases); wst("Cases sur "); wint(NHASH_PRIME); wst(" Utilisees\n");
  wst("Prof. Max = "); wint(max_prof); wst(" pour "); wint(n_atome); wst(" Atomes\n");
  derec;
}


Int 
hashpjw(s, tab_l)
     CHAR *s;
     Int tab_l;
{
  CHAR *ptr;
  unsigned h = 0,g;
  for ( ptr = s; *ptr ; ptr += 1 ){
    h = ( h << 4 ) + *ptr ;
    if( g = h & 0xf0000000){
      h = h ^ (g >> 24);
      h = h ^ g;
    }
  }
/*  return h % tab_l;*/
/* LA LIGNE SUIVANTE FONCTIONNE UNIQUEMENT SI tab_l EST UNE PUISSANCE DE 2 */
  return h & (tab_l-1) ;
}
/*
 * inserre un element dans la hash-table
 */

Int 
insert_atome(at, nhash)
     struct atome *at ;
     Int nhash;
{

/*  Int nhash  = get_hashind(at); */

  at->next_at = hash_atable[nhash];
  hash_atable[nhash] = at;

  return nhash;
}
/*
 * enleve un element de la hash-table
 */

Boolean 
enleve_atome(at)
     struct atome *at ;
{

  struct atome *prevat, *couat;
  Int nhash = get_hashind(at);

  /* on trouve l'atome et on l'enleve */
  for (couat = prevat = hash_atable[nhash]; couat && couat != at; ){
    prevat = couat; /* on avance */
    couat = couat->next_at;
  }
  if(couat){ /* trouvee */
#ifdef DEBUGGC
	        fprintf(stderr,"Free Atome %s\n", STRPNAME(at));
#endif
    if(prevat == couat) /* premier element */
      hash_atable[nhash] = couat->next_at;
    else
      prevat->next_at = couat->next_at;
    return True;
  }
  else {
#ifdef DEBUGGC
	        fprintf(stderr,"Atome Not Found %s\n", STRPNAME(at));
#endif
    return False;
  }
}
 
/* 
 *
 *  get_hashind retrouve l'indice d'un atome.
 */

Int 
get_hashind(at)
     struct atome *at;
{
  CHAR *name = (CHAR *) (at->pn + 2/2);
  
  if((Int *)at->pakage != nil){
    /* on calcule le nom de l'atome */
    CHAR atnom[MAX_COLS + 1]; 
    CHAR *pakname = (CHAR*) (((struct atome *)(at->pakage))->pn + 2/2);

    (void)sprintf(atnom, PAK_NAME_FORMAT, pakname, name);
    return  hashpjw(atnom, NHASH_PRIME);
  }
  return hashpjw (name, NHASH_PRIME);
}

/*
 * retrouve un element dans la hash-table.
 * 10/06/92 introduction du traitement des noms
 * du format package|pn pour garantir la consistance
 * des repressentation d'atomes dans l'univers Xt et
 * bVLISP.
 */

struct atome *
lookfor_atome_hash(name, h_ind)
CHAR *name;
Int *h_ind;
{
  Int nhash = -1;
  struct atome *at ;
  CHAR atnom[256], *pakname = NULL, *base_name;

  /* '|' peut etre un atome */
  if((base_name = index (name, '|')) && *name != '|' && *(base_name + 1)){ 
    /* pour retrouver des atomes avec des noms: <package|name> cela 
       garanti la repressentation graphique unique de tous les atomes. 
       Pour un atome aller de xbVLISP->Xt->xbVLISP */
    *base_name = 0;
    pakname = name;
    name = base_name + 1;
  }
  if(!pakname){
    nhash = hashpjw (name, NHASH_PRIME );
    for(at = hash_atable[nhash]; at != 0 ; at = at->next_at){
      if(strcmp(name, (CHAR *)(at->pn + 2/2)) == 0) break;
    }
  }
  if( pakname
     ||(at == NULL && (Int *)*atpackage != nil)
     || (at && (Int *)at >= atuser && (Int *)at->pakage != (Int *)*atpackage)){
    /* -------essayons le package--------- */
    /* calculons le nouveau nom de l'atome */
    if(! pakname )
      pakname = (CHAR *)(((struct atome *)*atpackage)->pn + 2/2) ;
    (void)sprintf(atnom, PAK_NAME_FORMAT,  pakname, name);
    nhash = hashpjw(atnom, NHASH_PRIME);
    for(at = hash_atable[nhash]; at != 0 ; at = at->next_at){
      if(strcmp(name, (CHAR *)(at->pn + 2/2)) == 0) break;
      
    }
  }
  if(h_ind) *h_ind = nhash;
  return  at ;
}

#if 0
/* def par #define dans vlisp.h pour optimisation */
struct atome *lookfor_atome( name )   CHAR *name; {return lookfor_atome_hash(name, NULL); }
#endif


Int 
myoblist() /* nsbr */
{
  Int i;

  a1 = nil;
  for( i = 0; i <  NHASH_PRIME ; i += 1){
    for ( a2 = (Int *)hash_atable[i]; a2 != 0 ; a2 = (Int *)((struct atome *) a2)->next_at){
       if (a2 == undef || a2 == nil ) continue;/* undef:modifit l'evaluation si reccupere comme arg. */
       a1 = cons (a2, a1);                     /* nil: ? */ 
    }
  }
  /* a1 = cdr ( a1 ); a1 = cdr ( a1 ); oblist normal enleve nil et undef */
  derec;
}

Int 
test_hash() /* nsbr */
{
  Int n = 0, *at_hash;
  Int pak_courr = *atpackage;
  
  for(a1=nil,a2=derat; a2 ; n++, a2=(Int *)((struct atome *)a2)->nexa){
    *atpackage = (Int)((struct atome *)a2)->pakage;
    at_hash = (Int *) lookfor_atome( STRPNAME(a2));
    if (at_hash != a2){
      CHAR errMess[129];
      sprintf (errMess, "Katastroffe ... %s [%d] et %s [%d] Differents\n"
	       ,STRPNAME(a2), (Int)a2, STRPNAME(at_hash), (Int)at_hash);
      wst (errMess);
    }
  }
  *atpackage = pak_courr;
  a1 = (Int *)valnb((Int *)n);
  derec;
}
      

/*
 * calcule le nom d'un atome,
 * le package, si est pressent, va en prefixe du nom.
 * Utilise pour repressenter les atomes comme chaines de caracteres
 * dans Xt.
 */

CHAR *NewString( CHAR *str) {
  return strcpy( malloc ((unsigned) strlen(str) + 1), str);
}

CHAR * 
getAtomName(at)
     struct atome * at;
{
  
  if((Int *)at->pakage != nil){
    /* on calcule le nom de l'atome */
    CHAR atnom[MAX_COLS + 1]; 
    CHAR *pakname = (CHAR*) (((struct atome *)(at->pakage))->pn + 2/2);
    
    (void)sprintf(atnom, PAK_NAME_FORMAT, pakname, STRPNAME(at));
    return NewString( atnom );
  } else 
    return NewString(STRPNAME(at));
}







