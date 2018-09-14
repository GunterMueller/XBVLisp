/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *               xGetValuess.c
 * fonctions de base VLISP pour l'inspection de ressource
 *            des des widgets.
 *
 *               Ernesto Sendoya
 *                avril  91		
 * 03/93 D.P. : rajout du Type Pixel comme explicitement connu.
 */

#include <Objets.h>

/* fonction auxiliaire */
#ifdef STDC_HEADERS
static void cvtXValToBvlType(Widget w, char * ret_t, char * res_t, union u_XtArgVal * value);
#else
static void cvtXValToBvlType();
#endif

/* 
 * modification de ressources 
 */


void XteGetValue(widget, name, lvalue)
     Widget widget;
     String name;
     XtArgVal * lvalue;
{
  Arg arg[1];

  XtSetArg(arg[0], name, lvalue);
  XtGetValues(widget, arg, 1);
}

/* 
 * Ici on fait la conversion des valeurs des ressources a des types LISP demmandees.
 * 
ARGSUSED */
static void cvtXValToBvlType(widget, result_type, ressource_type, value)
     Widget widget;
     char *result_type, * ressource_type;
     union u_XtArgVal * value;
{
  XrmQuark qkcourrent = XrmStringToQuark(ressource_type);
  XtArgVal result;
  Int ret;
  
  /* travail a faire ici pour visualiser les differents types de
     repressentation de donnees (ressources)  X */
  if(qkcourrent == qkInt)
    a1 = (Int *)cranb(value->entier);
  else if(qkcourrent == qkPixel)
    a1 = (Int *)cranb(value->pixel);
  else if(qkcourrent == qkDimension)
    a1 = (Int *)cranb((Int)value->dimension);
  else if(qkcourrent == qkPosition)
    a1 = (Int *)cranb((Int)value->position);
  else if(qkcourrent == qkBoolean)
    a1 = (Int *)cranb((Int)value->boolean);
  else  if(strcmp(result_type, XtRString) == 0){
    result =  getValueString( (char *)value->argval, ressource_type, &ret);
    if(ret == ERROR) a1 = nil;
    else genstring(value->string);
  }
  else if(strcmp(result_type, XtRAtomeListe) == 0){
    a1 =  ConvertValueToAtomeListe( (char *)value->argval, ressource_type, &ret);
    if(ret == ERROR) a1 = 0;
  }
  else if( qkcourrent == qkWidget
	  || strcmp(result_type, XtRWidget) == 0){  /* defaut Widget -> atome */
    if((a1 = (Int *)lookfor_atome(XeWidgetName((Widget) value->argval))) == NULL) a1 = nil;
  }
#if 0
  else if( qkcourrent = qkGlxConfig ) {
    char mess[80];
    Int i;
    Display *dpy = XtDisplay(widget);
    Int ScrNum = DefaultScreen(dpy);
    GLXconfig *rep, *null_glx = {0,0,0};
    rep = GLXgetconfig(dpy,ScrNum,null_glx);
    for ( ; rep && (rep->buffer || rep->mode) ; rep++) {
      sprintf(mess, "Buffer %d Mode %d Value %d\0", rep->buffer, rep->mode, rep->arg);
      wst(mess);
    }
  }
#endif
  else {
    printf("Warning <%s -> %s>: type non explicitement suporte par xGetValues\n", ressource_type, result_type);
    a1 = (Int *) cranb(value->entier);
  }
}

/*-------------------------------------------------------------------------*/
/*  Retrouve la ressource d'un Widget                                      */
/* retrouve la ressource du widget contenue dans l'atome passe en argument */
/* si le type de donnee en sortie (type LISP) est omis, on supose qu'est   */
/* le meme type en entree (xbVLISP)                                        */

#include <X11/IntrinsicP.h>
#include <BrTreeP.h>

Int xGetValues()      /* (xGetValues widget res_name  res_type result_type) */
{
  CHAR *ressource_name, *ressource_type, *result_type;
  union u_XtArgVal u_x_val;
  Widget widget ;
  Int *at = car(a1);
  
  u_x_val.flotant = 0.0;
  
  
  if(isWidget(at) &&  (widget = GET_WIDGET_FROM_ATOME(at))){
    a1 = cdr(a1); /* avancons dans les arguments */
    if(get_oneName(&ressource_name) == False){
      a1 = nil;
      derec;
    }
    if(strcmp (ressource_name, XtNparent) == 0){
      a1 = at; /* a1 avec la valeur du widget */
      my_Exec_Flag = 1;
      xWidgetParent();  /* retrouver le parent */
      my_Exec_Flag = 0;
      derec;
    } 
    if(!strcmp (ressource_name, XtNchildren) ){
      /* doit etre un composite */
      if(XtIsComposite(widget)){
	/* la liste d'atomes images des widgets a l'interieur du Composite
	   est retournee a xbVLISP                                        */
	CompositeWidget to = (CompositeWidget)widget;
	GetWidgetChildren(widget, to->composite.children, to->composite.num_children, NULL);
	derec;
      }
      /* si c'est pas un composite, pas d'enfants */
      a1 = nil;
      derec;
    }
    if(!strcmp (ressource_name, XtNfreres) 
       && XtIsSubclass(widget, tortueObjectClass)){
      Widget w_parent = XtParent(widget);
      CompositeWidget to = (CompositeWidget)w_parent;
      GetWidgetChildren(w_parent, to->composite.children, to->composite.num_children, widget);
      derec;
    }
    if(!strcmp (ressource_name, XtNtreeChildren)) {
      BrTreeConstraints wc = TREE_CONSTRAINT(widget);
      GetWidgetChildren(widget, wc->tree.children, wc->tree.n_children, NULL);
      derec;
    }
    if(!strcmp (ressource_name, "value")) { /* class Dialog : recuperation de la valeur */
      char *val = XtDialogGetValueString(widget);
      if(val) genstring(val);
      derec;
    }
    if((ressource_type = getResTypeFromWidget(widget , ressource_name)) == NULL){
      a1 = nil;
      XtFree(ressource_name);
      derec;
    }
    if(get_oneName(&result_type) == False)
      result_type = ressource_type;
    
    XteGetValue(widget, ressource_name, & u_x_val.argval);
    
    a1 = nil; /* si l'opperation de reccuperation de la ressource
		 est bien reussie, a1 contiendra la valeur reccuperee,
		 si non 'nil' */
    
    /* passage du type de donnees C a type lisp. fonction generale */
    
    cvtXValToBvlType(widget, result_type, ressource_type, &u_x_val); 
    
    XtFree(ressource_name);
    if(ressource_type != result_type) XtFree(result_type);
    XtFree(ressource_type);
  } else a1 = nil;
  derec;
}

Int xGetWindow() /* (xGetWindow widget) subr1 */
{
  if(isWidget(a1)) {
    a1 = cranb(XtWindow(GET_WIDGET_FROM_ATOME(a1)));
  }
  else a1 = nil;
  derec;
}





