/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* fonctions pour la creation de hierarchies des widgets
 *               ObjAtome.c
 *		Ernesto Sendoya
 *		  Mars 92
 */

#include <Objets.h>

/* cree et interne une ressource dans bvlisp */
Int xCreateResource()  /* (xCreateResource resnom rep_type val_res {nomat} ) */
{
  CHAR *resnom, *rep_type, *nomat ;
  Int nb, type;
  XtArgVal val, lisp_val;
  

  if(get_noms( &resnom, &rep_type, &nb) != 2 
     || (type = get_oneOBJET(&lisp_val)) == VIDE || type == INCONNU){
    if(nb == 1) XtFree(resnom);
    if(nb ==2) XtFree(rep_type);
    a1 = nil;
    derec;
  }
  a1 = cdr(a1); /* on avance */
  if(isnotnil(a1) && get_oneName(&nomat) == True)
    ;
  else nomat = NULL;

  val = getValueRessource( (char *)lisp_val, rep_type, type, &nb);
  if(nb == -1) /* convertion echoue */
    a1 = nil;
  a1 = (Int *) interneObjetRessource(resnom , rep_type, val, nomat, nb);
  derec;
}


/* cree un atome conteneur de la ressource en argument */
struct atome *  interneObjetRessource(nom_res , rep_type, xt_ressource, nom_at, tofree)
     CHAR * nom_res, *rep_type; /* le nom et la representation du type de la ressource */
     XtArgVal  xt_ressource;    /* la ressource */
     CHAR *nom_at;
     Int tofree ;
{
  struct atome * at_res;  
  XtBvlRessource * o_res;
  CHAR nom_pn[128];
  static Int nbr = 0;
  
  o_res = XtNew(XtBvlRessource);
  o_res->type = RESSOURCE;
  o_res->rep_type = rep_type;
  XtSetArg(o_res->arg_val[0], nom_res, xt_ressource);
  o_res->toFree = (Boolean)tofree;
  o_res->cnt = 0;
  
  /* un prefixe pour le pn de l'objet */
  nom_pn[0] = 'R';
  nom_pn[1] = ':';
  
  if(nom_at == NULL)sprintf((nom_pn + 1),"%d:%s", nbr++, nom_res);
  else strcpy ((nom_pn + 2), nom_at);
  at_res = (struct atome*) genat(nom_pn);
  at_res->obj = SET_OBJ(o_res);
  at_res->cval = (Int *)at_res; /* l'atome a ca propre valeur */
  return at_res;
}

