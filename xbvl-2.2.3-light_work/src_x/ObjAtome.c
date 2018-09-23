/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* fonctions pour interner des widgets et ressources comme des objets bVLISP
 *               ObjAtome.c
 *		Ernesto Sendoya
 *		  Mars 92
 */

#include <Objets.h>












/* (iswidget at ) subr1 */
/* (isressource at ) subr1 */
/* (isobjet at ) subr1 */

/* 
 * TEST POUR LES ATOMES REPRESSENTANTS DES OBJESTS EXTERIEURES
 *-----------------------------------------------------------
 * teste si l'atome est un Widget, et eventuellement la classe
 * d'appartenance du widget.
 */

bVlispFonc 
iswidget()    /* (iswidget at classe) subr2 */
{
  if(isWidget(a1)){
    CHAR *classe  ;
    if(getNameFromArg(&classe, a2) > 0){
      Widget widget = GET_WIDGET_FROM_ATOME(a1);
      WidgetClass widget_class = getWidgetClassFromString(classe) ;
      if(widget_class)
	a1 = XtIsSubclass(widget, widget_class) ? t : nil;
      else a1 = nil;
      XtFree(classe);
    }
    else a1 = t;
  }
  else a1 = nil;
  derec;
}

/*
 * teste si l'atome est une ressource
 */
bVlispFonc 
isressource()  /* (isressource at ) subr1 */
{
  a1 = isRessource(a1) ? t : nil; derec;
}

/*
 * teste si l'atome est un Objet.
 */
bVlispFonc 
isobjet()  /* (isobjet at ) subr1 */
{
  a1 = isObjet(a1) ? t : nil; derec;
}

/*ARGSUSED*/
void 
destroyRepWidget (w, atomo, call_data)
     Widget w;
     struct atome * atomo;
     caddr_t call_data;
{
  atomo->obj = nil;
  atomo->cval = undef;
}


/* interne un objet representant une widgette */
struct atome *  
interneObjetWidget(atw, widget)
     struct atome *atw;
     Widget widget;

{
  XtBvlWidget * w_obj  = XtNew(XtBvlWidget);
  
  w_obj->type = WIDGETTE;
  w_obj->widget = widget;

  atw->obj = SET_OBJ(w_obj);
  if(atw->cval == undef) /* autocoter l'atome */
    atw->cval = (Int *)atw;

  /* enlever la repressentation lisp si elimine phisiquement */
  XtAddCallback(widget, XtNdestroyCallback, 
		(XtCallbackProc) destroyRepWidget, atw);

  return atw;
}



/* genere un atome nouveau */
struct atome *  
genUniqueAtome(nom)
     CHAR * nom;
{
  extern Int DEJA;
  Int prefix = 0;
  struct atome * der_unique_obj;
  
  
  do{
    if(prefix == 0)
      der_unique_obj = (struct atome*) genat(nom);
    else{
      CHAR tmp[129];
      sprintf(tmp, "%s%d", nom, prefix);
      der_unique_obj = (struct atome*) genat(tmp);
    }
    prefix += 1;
  } while(DEJA&&isGraphicObjet(der_unique_obj));
  /* un prefixe pour le pn de l'objet representant le widget */
  return der_unique_obj;
}



/* retourne la widgette contenue dans 'a1' si elle existe */
Widget 
getWidgetFromAtome()
{/* dans a1 l'atome conteneur du  widget */
  Int *at_tmp ;
  
  if(islist(a1)) 
    at_tmp =  car(a1);
  else if(isatom (a1))
    at_tmp = a1;
  else return NULL;
  if(isWidget(at_tmp)){
    Widget wtmp =  GET_WIDGET_FROM_ATOME(at_tmp);
    a1 = cdr(a1);
    return wtmp;
  }
  return NULL;
}


/* possitionne une ressource de widget avec un objet de type ressource */
Int 
xsetRessource()  /* (creeRessource owidget oressource) subr2) */
{
  if(isWidget(a1) 
     && isRessource(a2)){
    Widget w =  GET_WIDGET_FROM_ATOME(a1);
    Arg *arg = GET_RESSOURCE_FROM_ATOME(a2);
    XtSetValues(w, arg, 1);
    a1 = t;
  }
  else a1 = nil;
  derec;
}

  


