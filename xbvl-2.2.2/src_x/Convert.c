/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *            Convert.c
 *    Convertiseurs pour faire le couplage
 *    d'objet LISP et d'objets X et Xt.
 *         Ernesto Sendoya   Mars. 1992
 *
 */

#include <vlisp.h>


static void _bCvtTextToStringList();
static void _bCvtStringListToAtomList();
void _bCvtAtomListToStringList();
static void _bCvtStringToStringList();
static void _bCvtStringToAtomList();

void  RegisterConverters()
{
  XtAddConverter (XtRText, XtRStringList,
		  _bCvtTextToStringList, NULL, 0);
  XtAddConverter (XtRStringList, XtRAtomeListe,
		  _bCvtStringListToAtomList, NULL, 0);
  XtAddConverter (XtRAtomeListe, XtRStringList,
		  _bCvtAtomListToStringList, NULL, 0);
  XtAddConverter (XtRString, XtRStringList,
		  _bCvtStringToStringList, NULL, 0);
  XtAddConverter (XtRString, XtRAtomeListe,
		  _bCvtStringToAtomList, NULL, 0);
}

/* ARGSUSED */
static void _bCvtTextToStringList( args, num_args, from, to)
    XrmValuePtr args;
    Cardinal *num_args;
    XrmValuePtr from, to;
{
    register Int i, count = 1;
    register CHAR *ch, *start = from->addr;
    static String *list;
    Int len;

    if (*num_args != 0) {
	XtErrorMsg("TextToStringList", "wrongParameters",
		   "XtToolkitError",
		   "String to string list conversion needs no extra arguments",
		   (String *) NULL, (Cardinal *) NULL);
    }
    if (to->addr != NULL && to->size < sizeof(String *)) {
	to->size = sizeof(String *);
	return ;
    }
    if (start == NULL || *start == '\0') list = NULL;
    else {
	for (ch = start; *ch != '\0'; ch++) {    /* Count strings */
	    if (*ch == '\n') count++;
	}
	list = (String *) XtCalloc(count+1, sizeof(String));

	for (i = 0; i < count; i++) {
	    for (ch = start; *ch != '\n' && *ch != '\0'; ch++) {}
	    len = ch - start;
	    list[i] = XtMalloc(len + 1);
	    (void) strncpy(list[i], start, len);
	    list[i][len] = '\0';
	    start = ch + 1;
	}
    }
    if (to->addr == NULL) to->addr = (caddr_t) &list;
    else *(String **) to->addr = list;
    to->size = sizeof(String *);
}


/*
 * conversion d'un tableau de chaines de caracteres ( de noms )
 * dans une liste dont les elements sont des atomes avec le p-name
 * du tableau des chaines.
 */


/* ARGSUSED */
static void _bCvtStringListToAtomList(args, num_args, from, to )
     XrmValuePtr args;
     Cardinal *num_args;
     XrmValuePtr from, to;
{
  register String *start = (String *)from->addr;
  static Int *liste_datomes;
  
  if (*num_args != 0) {
    XtErrorMsg("StringToStringList", "wrongParameters",
	       "XtToolkitError",
	       "String to Atom list conversion needs no extra arguments",
	       (String *) NULL, (Cardinal *) NULL);
  }
  if (to->addr != NULL && to->size < sizeof(Int *)) {
    to->size = sizeof(Int *);
    return ;
  }
  if (start == NULL || *start == '\0') liste_datomes = nil;
  else { /* construire la liste d'atomes */
    for (liste_datomes = nil ; * start ; start ++) { 
      Int * atome = GetListElement(*start);
      liste_datomes = cons(atome, liste_datomes);
    }
  }
  if(liste_datomes != nil) { /* reverse */
    Int * a1aux = liste_datomes, * a2aux = nil;
    while(islist(a1aux)){
      a2aux = cons(car(a1aux), a2aux); 
      a1aux=cdr(a1aux);
    }
    liste_datomes = a2aux;
  }
  if (to->addr == NULL) to->addr = (caddr_t) &liste_datomes;
  else *(Int **) to->addr = liste_datomes;
  to->size = sizeof(Int *);
}


/*
 * fait la conversion inverse de _bCvtAtomListToStringList():
 * a partir d'une liste (d'atomes ou de chaines) genere un
 * tableau de chaines de caracteres termine par NULL.
 * La le tableau est alloue dynamica
 * Limitation:
 *            Les conversions sont faites sur des listes de
 *            premier niveau. 
 *           
 * Representations:
 *            Atomes: le pn avec  prefixes du package
 *            Chaines: la chaine elle meme.
 *            Nbrs: la chaine.
 *            Listes: "liste ?"
 */

/* ARGSUSED */
void _bCvtAtomListToStringList(args, num_args, from, to)
     XrmValuePtr args;
     Cardinal *num_args;
     XrmValuePtr from, to;
{
  Int i , count, * lisp_liste = (Int *)from->addr;
  static String *list;
  CHAR * err_liste = "liste ?";
  Int * ele;
  
  if (*num_args != 0) {
    XtErrorMsg("AtomListToStringList", "wrongParameters",
	       "XtToolkitError",
	       "Liste to string list conversion needs no extra arguments",
	       (String *) NULL, (Cardinal *) NULL);
  }
  if (to->addr != NULL && to->size < sizeof(String *)) {
    to->size = sizeof(String *);
    return ;
  }
  if (lisp_liste == nil){
    list = NULL;
    return ;
  }
  for (count = 0, ele = lisp_liste; ele != nil ; ele = cdr(ele)) {   
    count++; /* Conter le nbr. d'elements de la liste */
  }
  if(count){
    list = (String *) XtCalloc(count + 1, sizeof(String));
    /* reccuperer les pn des atomes, avec le nom du package en prefix ? */
    for (i = 0; lisp_liste != nil ; lisp_liste = cdr(lisp_liste)) {
      CHAR *image_ele = NULL, tab[128];
      
      /* trouver les represenations des objets LISP */
      if(isatom((Int)(ele = car(lisp_liste)))){
	image_ele = getAtomName((struct atome *) ele);
      }
      else if(isflo(ele)){
	sprintf(tab,"%f", (float)valflo(ele));
	image_ele = XtNewString(tab);
      }
      else if(isnum(ele)){
	sprintf(tab,"%d",valnb(ele));
	image_ele = XtNewString(tab);
      }
      else if(isustr(ele)){
	CHAR strname[128];
	if(!opti) sprintf(strname,"%s", STR_FROM_STRLSP(ele));
	else      sprintf(strname,"\"%s\"", STR_FROM_STRLSP(ele));
	image_ele = XtNewString(strname);
      }
      else if (!image_ele ){
	image_ele = XtNewString(err_liste); /* listes, a voir */
      }
      list[i++] = image_ele;
    }/* end for */
    list[i] = NULL; /* fin de tableaux */
    if (to->addr == NULL) to->addr = (caddr_t) &list;
    else *(String **) to->addr = list;
    to->size = sizeof(String *);
    return ;
  } 
}


/*
 * une valeur est convertie en une liste d'atomes:
 * le type de la valeur peut etre StringList (table de chaines)
 * ou liste de chaines.
 */
Int * ConvertValueToAtomeListe (strPtr, resType, tofree)
     CHAR *strPtr, *resType;
     Int *tofree;
{ 
  XrmValue from, to;

  if (strcmp(resType, XtRAtomeListe) == 0){
    *tofree = False ;
    return (Int *) strPtr;
  }
  from.size = sizeof(String *);
  from.addr = strPtr;
  to.addr = NULL;
  *tofree = False;
  XtConvert(top_widget, resType, &from, XtRAtomeListe, &to);
  if(to.size == 0){
    *tofree = ERROR;
    return NULL;
  }
  *tofree = True;
  return (Int *) *((XtArgVal *) to.addr);

}

/*
 * Une chaine est convertie en un tableau de chaines.
 * Chaque mot est une chaine du nouvau tableau.
 */
/* ARGSUSED */
static void _bCvtStringToStringList(args, num_args, from, to)
     XrmValue * args;
     Cardinal *num_args;
     XrmValue * from;
     XrmValue * to;
{
  static String *rstring_list;
  String start = from->addr;
  
  if (*num_args != 0) {
    XtErrorMsg("StringToStringList", "wrongParameters",
	       "XtToolkitError",
	       "String to string list conversion needs no extra arguments",
	       (String *) NULL, (Cardinal *) NULL);
  }
  if (to->addr != NULL && to->size < sizeof(String *)) {
    to->size = sizeof(String *);
    return ;
  }
  cvtStrToStrLst(&rstring_list, start);
  if (to->addr == NULL) to->addr = (caddr_t) &rstring_list;
  else *(String **) to->addr = rstring_list;
  to->size = sizeof(String *);
}


/*
 * fait le travail de conversion d'une chaine vers un tableau de chaines.
 */

Int cvtStrToStrLst(ret_list, start)
     String ** ret_list , start;
{
  
  String *string_list = NULL, fin_mot;
  Int i = 0;

  if(!start) return 0;
  while(1){
    while(*start && *start == ' ') start += 1;
    if (!*start) break;
    
    if(!*string_list) 
      string_list = (String *) XtCalloc(2, sizeof(String));
    else
      string_list = (String *)XtRealloc((String)string_list, (i+2) * sizeof(String));
    
    if(fin_mot = index(start, ' ')){
      Int l = fin_mot - start;
	String new_mot = XtMalloc(l + 1);
	strncpy(new_mot, start, l);
	new_mot[l] = 0; /* fin chaine */
	string_list [i++] = new_mot;
	start = fin_mot;
      }else {
	string_list[i++] = XtNewString(start);
	break;
      }
    }
  if(i > 0) string_list[i] = NULL;
  *ret_list = string_list;
  return i;
}


/*
 * Une chaine est convertie en une liste d'atomes.
 * Chaque mot est un element de la liste.
 */
/* ARGSUSED */
static void _bCvtStringToAtomList( args, num_args, from, to)
     XrmValuePtr args;
     Cardinal *num_args;
     XrmValuePtr from, to;
{
  CHAR *fin_mot, *start = from->addr;
  static Int *liste_datomes ;


  if (*num_args != 0) {
    XtErrorMsg("StringToAtomList", "wrongParameters",
	       "XtToolkitError",
	       "String to string list conversion needs no extra arguments",
	       (String *) NULL, (Cardinal *) NULL);
  }
  if (to->addr != NULL && to->size < sizeof(String *)) {
    to->size = sizeof(String *);
    return;
  }
  liste_datomes = nil;
  if (start)
    while(1){
      CHAR new_mot[128];  Int *element;
      
      while(*start && *start == ' ') start += 1;
      if (!*start) break;
      
      if(fin_mot = index(start, ' ')){
	Int l = fin_mot - start;
	strncpy(new_mot, start, l);
	new_mot[l] = 0;
	element = GetListElement(new_mot);
	liste_datomes = cons(element, liste_datomes);
	start = fin_mot;
      }else {
	strcpy(new_mot, start);
	element = GetListElement(new_mot);
	liste_datomes = cons(element, liste_datomes);
	break;
      }
    }
  if(liste_datomes != nil) { /* reverse */
    Int * a1aux = liste_datomes, * a2aux = nil;
    while(islist(a1aux)){
      a2aux = cons(car(a1aux), a2aux); 
      a1aux=cdr(a1aux);
    }
    liste_datomes = a2aux;
  }
  if (to->addr == NULL) to->addr = (caddr_t) &liste_datomes;
  else *(Int **) to->addr = liste_datomes;
  to->size = sizeof(Int *);
}
