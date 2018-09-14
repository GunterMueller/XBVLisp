/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *            xgetargs.c
 * fonctions pour la recuperation d`arguments
 * dans une liste.
 * 
 */
#include <Objets.h>

























/*---------------------------------------------------------------------------
 *   recupere les noms eventuels des fenetres passees en arguments
 *   a des fonctions avec des appels  (nomfonc [nom1] [nom2])
 *   toujour retourne le nombre de chaines trouvees en argument,
 *   get_noms retourne ERROR s'il arrive a lire un objet de type different.
 ---------------------------------------------------------------------------*/

Int get_noms(nomtop, nomwind, nb)
CHAR **nomtop, **nomwind;
Int *nb;
{
  Int type;

  *nb = 0;
  if((type = get_oneOBJET((XtArgVal *)nomtop)) == INTEGER
     ||  type == FLOAT || type == INCONNU)
    return ERROR;
  if(type == VIDE) return 0;
  *nb = 1;
  a1 = cdr(a1); /* avancer */
  if((type = get_oneOBJET((XtArgVal *)nomwind)) == INTEGER
     || type ==  FLOAT || type == INCONNU)
    return ERROR;
  if(type == VIDE)
    return 1;
  a1 = cdr(a1); /* avancer */
  return (*nb = 2);
}

/*--------------------------------------------------------------------------- 
 *   recupere le couple de nombres suivants de la liste passee 
 *   en argument.
 *  retourne le nombre de nobres lus, en a5 la suite des arguments.
 ---------------------------------------------------------------------------*/
Int get_ints( premier, suivant, nb)
     Int *premier, *suivant, *nb;
{
  *nb = 0;
  if(init_oneINT(premier) != INTEGER)
    return 0;
  *nb = 1;
  a1 = cdr(a1);
  if(init_oneINT(suivant) != INTEGER)
    return 1;
  a1 = cdr(a1); 
  return (*nb = 2);
}
/*
 * retourne le nombre trouve dans le premier argument.
 */

Int get_oneNBR( nbr_ret)
     Int *nbr_ret;
{
  Int type = get_oneOBJET( (XtArgVal *) nbr_ret) ;
  if(type == STRING || type == NAME){
    wst("mauvais argument: un objet de type INTEGER ou FLOAT attendu !\n");
    XtFree((String) *nbr_ret);
  }
  return type ;
}

/*
 * utilise pour extraire des valeurs d'une liste d'arguments.
 * retourne la donnee trouvee dans l'argument suivant
 * et son type : STRING, INTEGER, FLOAT, NAME, INCONNU.
 */

Int get_oneOBJET( data )
  XtArgVal *data;
{
  Int *arg, type;

  if(isnil(a1)) return VIDE;
  arg = car(a1);
  if(isnil(arg)) return VIDE;
  if((type = getNameFromArg((CHAR **)data, arg))>0)
    return type;
#ifndef NO_VFLO
  else if(isflo(arg)){ /* retourner la valeur */
    *data = (XtArgVal)valflo(arg);
    return FLOAT;
  }
#endif
  else if(isnum(arg)){ /* retourner la valeur */
    *data = (XtArgVal)valnb(arg);
    return INTEGER;
  }
  else if(islist(arg)){
    *data = (XtArgVal)arg;
    return LISTE;
  }
  printf("OBJET LISP INCONNU. Mauvais Argument o=%d (bat = %d, hlst = %d)\n"
	 , (Int)arg, (Int)bat, (Int)hlst);
  return INCONNU;/* ? */
}

Int  getNameFromArg(ret_str , at)
     CHAR ** ret_str;
     Int *at;
{
  if(at == nil) return -1;
  if (isustr(at)){/* retourner la chaine */
    *ret_str = XtNewString(STR_FROM_STRLSP(at));
    return STRING;
  }
  else if(isatom(at)) { /* retourner le nom de l'atome */
    *ret_str = getAtomName((struct atome *)at); /* AVEC LE PACKAGE EN PREFIX ? */
    return NAME;
  }
  return 0;
}
/*
 * Cherche et retourne une fenetre 
 * dont le nom est dans a1 et le nombre
 * d'atomes du nom dans "n".
 * Peut retourner toutes les widgettes d'une interface
 * 
 */

Widget get_widget(n)
     Int *n;
{
  if(isWidget(car(a1))){
    Widget w =  GET_WIDGET_FROM_ATOME(car(a1));
    *n = 1; /* pour compatibilite */
    a1 = cdr(a1);
    return  w;
  } 
  *n = 0;
  return NULL;
}

/*
 * reccupere la chaine en a1, et avance dans la liste
 * d'arguments.
 */
Int  get_oneName(nom)
     CHAR **nom;
{
  Int *at = car(a1);

  if( getNameFromArg(nom, at) > 0 ){
    a1 = cdr(a1); /* avancer */
    return True;
  }
  /* DEBUG 
  printf("mauvais argument: un objet de type CHAINE attendu !\n"); */
  return False; 
}
/*
 * initialise un argument numerique avec
 * control de type. 
 * Si succes, la fonction avance la liste d'arguments.
 */

init_oneNBR(nbr)
     Int *nbr;
{
  Int type;
 
 if((type = get_oneNBR(nbr)) == STRING || type == NAME){
    XtFree((String) nbr);
    return ERROR;
  }
  if(type == INTEGER || type == FLOAT)
    a1 = cdr(a1);
  return  type;
}

/*
 * initialise un argument numerique avec
 * control de type. 
 * Si succes, la fonction avance la liste d'arguments.
 */

init_oneINT(data)
     Int *data;
{
  Int type, *arg;

  if(isnil(a1)) return VIDE;
  arg = car(a1);
  if(isint(arg)){ /* retourner la valeur */
    *data = (XtArgVal)valnb(arg);
    return INTEGER;
  }
  else return ERROR;
}
/*---------------------------------------------------------------
 * extrait des registres VLISP les arguments pour une fonction
 * appelee qui attend une chaine de caracteres et une fenetre d'interface
 ---------------------------------------------------------------*/
Int getNomArgWin(valnom, widgette_ptr, n)
     Widget *widgette_ptr;
     CHAR *valnom;
     Int *n;
{

  if(a1 == nil) return False;

  a5 = cdr(a1);
  a1 = car(a1);
  
  if(isatom(a1)) {
    a4=((struct atome *)a1)->pn; 
    strcpy(valnom,(CHAR *)(a4+ 2/2));
  }
  else if (isustr(a1))
    strcpy(valnom,(CHAR *)(a1 + 2/2));
  else 
    return False;
  
  a1 = a5;
  
  *widgette_ptr = get_widget(n); 
  return True;
}

/*
 * ramene le shell de l'interface
 */

Widget getApplicationWidget()
{
  Widget top_widget = NULL;
  
  if(isWidget(car(a1))){
    top_widget  = GET_WIDGET_FROM_ATOME(car(a1));
    a1 = cdr (a1);
  }
  return top_widget;
}
