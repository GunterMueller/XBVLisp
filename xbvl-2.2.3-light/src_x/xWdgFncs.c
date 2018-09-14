/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *            xWdgFncs.c
 * les fonctions de base VLISP pour la manipulation 
 * des Widgets.
 *        Ernesto Sendoya janvier  91
 * Introduction des Widgets comme objests Lisp, Avril 92		
 *
 */

#include <Objets.h>

#ifdef STDC_HEADERS
static void  enleveClosure(Widget, XtCallbackList callback_rec, String callback_name);
#else
static void  enleveClosure();
#endif


/*---------------------------------------------------------------  
 * 
 * ---------------------------------------------------------------*/
bVlispFonc xRealize()       /* (xRealize top_nom) */
{
  Widget widget =  getApplicationWidget();

  a1 = nil;
  if(widget){
    a1 = t;
    XtRealizeWidget(widget);
  }
  derec;
}

/*---------------------------------------------------------------  
 * 
 * ---------------------------------------------------------------*/
bVlispFonc xUnrealize()       /* (xUnrealize top_nom) */
{
  Widget widget = getApplicationWidget();
  a1 = nil;
  if(widget){
    XtUnrealizeWidget(widget);
    a1 = t;
  }
  derec;
}

/*---------------------------------------------------------------  
 * 
 * ---------------------------------------------------------------*/
bVlispFonc xManage()     /* (xManage top_nom ) */
{
  Int n;
  Widget widget =  get_widget(&n);

  a1 = nil;
  if(widget /*&& n >= 2*/ && XtIsManaged(widget) == False){
    XtManageChild(widget);
    a1 = t;
  }
  derec;
}

/*---------------------------------------------------------------  
 * NSBR
 ---------------------------------------------------------------*/
bVlispFonc xUnmanage()    /* (xUnmanage top_nom ) */
{
  Int n;
  Widget widget = get_widget(&n);

  a1 = nil;
  if(widget /* && n >=  2*/ && XtIsManaged(widget) == True){
    XtUnmanageChild(widget);
    a1 = t;
  }
  derec;
}

/*---------------------------------------------------------------  
 * Unmape le(s) interface(s)  accroche(s) au top passe en argument(s)
 * (on n'a pas le droit de faire disparaitre la fenetre courante) 
 * NSBR
 ---------------------------------------------------------------*/

bVlispFonc UnMapWid() /* (unmap win ) */
{
  Int n;
  Widget widget =  get_widget(&n);

  a1 = nil;
  if(widget){
    XtUnmapWidget(widget);
    a1 = t;
  }
  derec;
}

/*---------------------------------------------------------------
 *	Mapping de la fenetre de dialogue (interface) passe
 *      identifiee par les noms passes en argument.
 *	NSBR
 ---------------------------------------------------------------*/

bVlispFonc MapWid()/* (mapsub nomtop )*/
{
  Int n;
  Widget widget = get_widget(&n);

  a1 = nil;
  if(widget){
    XtMapWidget(widget);
    a1 = t;
  }
  derec;
}
/*---------------------------------------------------------------
 *	fait circuler les Widgets qui sont dependants du
 *      widget passe en parametre.
 *	NSBR
 ---------------------------------------------------------------*/

bVlispFonc xCircSubWidgets()/* (xCircWidgets nom_widget {up/dow}) */
{
  Int n = 0;
  Widget widget = get_widget(&n);

  if(isint(a1)){ /* retourner la valeur */
    n = (Int) valnb(a1);
  }
  if (n) n = LowerHighest;
  else n = RaiseLowest;
  
  a1 = nil;
  if(widget){
    XCirculateSubwindows(display, XtWindow(widget), n);
    a1 = t;
  }
  derec;
}



/* ---------------------------------------------------------------*
 * auxiliaire de xAddCallback
 *           enleve une expression du Callback 
 *
 * ---------------------------------------------------------------*/

static void  enleveClosure(w, callback_list, callback_name)
     Widget w;
     XtCallbackList callback_list;
     String callback_name;
{
  String old_expr = (String)callback_list->closure;
  XtRemoveCallback(w, callback_name, callback_list->callback, callback_list->closure);
  /* ???? XtFree(old_expr); */
  return;
}
/* ----------------------------------------------------------------------------------------*
 * positione une expression dans le widget passe en parametre.
 * Cette expression sera communiquee a l'interprete a l'appel du callback.
 * Bug: l'expression est passee comme une chaine de caracteres de maximun 19 caracteres.
 *      La maniere correcte de le faire c'est de communiquer l'adresse de l'objet lisp a 
        evaluer par l'interprete. Cela supposerait la reecriture dans la machine virtuel
        des plussieurs fonctions de lecture : wst et xgetch1. A faire dans une prochaine
        version. 
 *----------------------------------------------------------------------------------------*/
bVlispFonc xaddCallback()  /* (xAddCallback  widgetat "callbackname" "<EXPRESSION>" ) */
{
  Widget widget;
  CHAR *callback, *expression;
  
  if(isWidget (a1)){
    Int i;
    XtCallbackList callback_List = NULL;
    widget =  GET_WIDGET_FROM_ATOME(a1);
    if(getNameFromArg(&callback, a2) > 0){
      if(getNameFromArg(&expression, a3) > 0){
	/* control des callbacks */
	XtCallbackStatus status = XtHasCallbacks(widget, callback);
	if(status == XtCallbackNoList){
	  a1 = nil;
	  derec;
	}
	/*
	  On reccupere la liste de Callbacks et on regarde si la fonction
	  a ete enregistree.
	  Si c'est le cas on libere la place alloue pour l'encien argument
	  et on met le nouveau en place.
	  */
	XteGetValue(widget, callback, (XtArgVal *)&callback_List);
	for(i = 0; callback_List && (callback_List + i)->callback; i++){
	  if((void *)(callback_List + i)->callback == (void *)comToXbvl) {
	    enleveClosure(widget, (callback_List + i), callback);
	    break;
	  }
	}
	XtAddCallback(widget , callback, (XtCallbackProc)comToXbvl, expression);
	a1 = t;
      }else a1 = nil;
      XtFree(callback);
    }else a1 = nil;
  }else a1 = nil;
  derec;
}

/*---------------------------------------------------------------
 *	xPopup   sbr2
 *      Fait l'affichage du popup passe en argunent.
 *      Le mode d'interaction peut etre modifie 
 *      par la valeur du deuxieme argument. 
 *      Le mode par default est non-modal.
 ---------------------------------------------------------------*/

bVlispFonc xPopup() /* (xPopup -popup- [mode])  subr2 */
{
  if(isWidget (a1)){
    Widget widget =  GET_WIDGET_FROM_ATOME(a1);
    if(IS_POPUP_WIDGET(widget)){
      Int grab_mode  ;
      if(isint(a2))
	grab_mode = valnb(a2);
      if(grab_mode != XtGrabNonexclusive && grab_mode != XtGrabExclusive)
	grab_mode = XtGrabNone ;
      XtPopup(widget, grab_mode);
      a1 = t;
    } else a1 = nil;
  } else a1 = nil;
  derec;
}
/*---------------------------------------------------------------
 *	xPodown   sbr1
 *      Fait disparaitre le popup passe en argunent.
 ---------------------------------------------------------------*/

bVlispFonc xPopdown() /* (xPopdown -popup-)  subr1 */
{
  if(isWidget (a1)){
    Widget widget =  GET_WIDGET_FROM_ATOME(a1);
    if(IS_POPUP_WIDGET(widget)){
      XtPopdown(widget);
      a1 = t;
    } else a1 = nil;
  } else a1 = nil;
  derec;
}

  
 
