 
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	Objets.h
 *    Ernesto Sendoya, Avril 1992
 */


#ifndef _Objets_h_
#define _Objets_h_

#include "vlisp.h"


/* Defines pour les objets graphiques du systeme */

/* la repressentation d'une ressource Xt */
typedef
  struct {
    Int      type ;         /* ressource ou widget */
    char    * rep_type ;    /* le type de la ressource */
    Arg      arg_val[1];    /* le nom de la ressource et sa valeur */
    Boolean  toFree ;       /* taille en octets de la ressource */
    Int      cnt ;          /* alloue a cnt widgets */
  } XtBvlRessource;

/* la repressentation d'un widget */
typedef
  struct {
    Int       type ;   /* ressource ou widget */
    Widget    widget;
  }  XtBvlWidget;

typedef
  struct  {
    Int type;      /* le type de l'objet */
  } BvlBasicObjet;

/* pour tou integrer dans la meme structure de donnees */

union   graphicObjet {
  BvlBasicObjet  basic_object ;
  XtBvlRessource objet_ressource;
  XtBvlWidget objet_widget ;
};


/* ---------------------------------------------------- */
/* Prototypage des fonctions de manipulation des OBJETS */
/* ---------------------------------------------------- */

#ifdef STDC_HEADERS

/* ObjAtome.c */
extern void destroyRepWidget (Widget w, struct atome * atomo, caddr_t call_data);
extern struct atome *  interneObjetWidget(struct atome *atw, Widget widget);
extern struct atome *  genUniqueAtome(CHAR * nom);
extern Widget getWidgetFromAtome(void);

/* ObjInit.c */
extern WidgetClass  getWidgetClassFromString( CHAR *name);
extern CHAR * newObjetName( CHAR *name );

/* Drawxbvl.c */
extern void DrawxbvlResize(DrawxbvlWidget w) ;

/* xCrObjet.c */
extern struct atome * interneObjetRessource(CHAR *nom_res, CHAR *r_type, XtArgVal val, CHAR *a, Int fr);
extern XtArgVal  getValueRessource(CHAR * val_res, CHAR * xrep_type, Int lisp_typeval, Int * ret);

/* xGetRes.c */
extern Int getArgListForWidget(Widget w, ArgList *argg, Int * nbr)  ;
extern Int getArgList(WidgetClass p_w_c, WidgetClass w_c,  ArgList *argg, Int * nbr)  ;
extern Int getXtArgFromLispArgs(WidgetClass p_w_c, WidgetClass w_c, ArgList agl);
extern XtArgVal getValueRessource(CHAR * v_r, CHAR *xr_t, Int lsp_type, Int * ret);
/* extern Int get_oneValRes(XtArgVal * ret_val); */
extern XtArgVal  getValueListe(CHAR *strPtr, CHAR *resType, Int *tofree);
extern XtArgVal getValueString(CHAR *sPtr, CHAR * rType, Int * rflg);
extern XtArgVal getValueInteger(Int entier, CHAR * rType, Int * rflg);
extern XtArgVal getValueFloat(float *fPtr, CHAR * rType, Int * rflg);
extern String getResTypeFromWidget(Widget w, String resource_name);
extern String getResTypeFromWidgetClass(WidgetClass p_w_c, WidgetClass w_c, String resource_name);
extern void lookEveryWhere(String * ret_type, String ress_name);

/* XtToLISP.c */
extern void GetWidgetChildren(Widget w, WidgetList wl, Int nc, Widget cw);

#else

/* ObjAtome.c */
extern void destroyRepWidget ();
extern struct atome *  interneObjetWidget();
extern struct atome *  genUniqueAtome();
extern Widget getWidgetFromAtome();

/* ObjInit.c */
extern WidgetClass  getWidgetClassFromString();
extern CHAR * newObjetName();

/* xCrObjet.c */
extern struct atome * interneObjetRessource();
extern XtArgVal  getValueRessource();

/* xGetRes.c */
extern Int getArgListFromWidget();
extern Int getArgList();
extern Int getXtArgFromLispArgs();
extern XtArgVal  getValueListe();
extern XtArgVal getValueString();
extern XtArgVal getValueInteger();
extern XtArgVal getValueFloat();
extern String getResTypeFromWidget();
extern String getResTypeFromWidgetClass();
extern void lookEveryWhere();

/* XtToLISP.c */
extern void GetWidgetChildren();

#endif

extern struct atome *der_unique_obj;

#define isObjet(x)  (isatom(x)&&(((struct atome *) x)->obj != nil))
#define isGraphicObjet(x)  isObjet(x)

#define testWidget(x)      (((BvlBasicObjet *)((struct atome *) x)->obj)->type == WIDGETTE)
#define isWidget(x)        (isGraphicObjet(x) && testWidget(x))

#define testRessource(x)   (((BvlBasicObjet *)((struct atome *) x)->obj)->type == RESSOURCE)
#define isRessource(x)     (isGraphicObjet(x) && testRessource(x))

#define isNotGraphicObjet(x)     (isnotatom(x) || ((struct atome *) x)->obj == nil)
#define isNotObjet(x)            isNotGraphicObjet(x)
#define isNotWidget(x)           (isNotGraphicObjet(x) || testRessource(x))

#define SET_OBJ(x)   ((Int *)x)
#define GET_WIDGET_FROM_ATOME(at)    (((XtBvlWidget *)((struct atome *) at)->obj)->widget)
#define GET_RESSOURCE_FROM_ATOME(at)    (((XtBvlRessource *)((struct atome *) at)->obj)->arg_val)

/* pour acceder aux widget de base */
/* ------------------------------- */
#define XtNCore          "Core"
#define XtNComposite     "Composite"
#define XtNConstraInt    "Constraint"
#define XtNShell         "Shell"
#define XtNApplicationShell    "ApplicationShell"
#define XtNOverrideShell       "OverrideShell"
#define XtNTransientShell      "TransientShell"
#define XtNTopLevelShell       "TopLevelShell"

/* pour acceder aux athene widgets */
/* ------------------------------- */

#define XtNawSimple      "awSimple"
#define XtNawList        "awList"
#define XtNawDialog      "awDialog"
#define XtNawPaned       "awPaned"
#define XtNawPanner      "awPanner"
#define XtNawPorthole    "awPorthole"
#define XtNawRepeater    "awRepeater"
#define XtNawTree        "awTree"
#define XtNawAsciiText   "awAsciiText"
#define XtNawAsciiDisc   "awAsciiDisc"
#define XtNawForm        "awForm"
#define XtNawLabel       "awLabel"
#define XtNawCommand     "awCommand"
#define XtNawBox         "awBox"
#define XtNawViewport    "awViewport"
#define XtNawScroll      "awScroll"

#define XtNawSimpleMenu     "awSimpleMenu"
#define XtNawSme            "awSme"
#define XtNawSmeLine        "awSmeLine"
#define XtNawSmeBSB         "awSmeBSB"
#define XtNawStripChart     "awStripChart"
#define XtNawTextSink       "awTextSink"
#define XtNawToggle         "awToggle"
#define XtNawTextSrc        "awTextSrc"
#define XtNawLogo           "awLogo"
#define XtNawGrip           "awGrip"
#define XtNawClock          "awClock"
#define XtNawSimpleMenu     "awSimpleMenu"
#define XtNawToggle         "awToggle"
#define XtNawAsciiSrc       "awAsciiSrc"

#define XtNbrTree        "brTree"

#define NoEventMask			0L
#define KeyPressMask			(1L<<0)  
#define KeyReleaseMask			(1L<<1)  
#define ButtonPressMask			(1L<<2)  
#define ButtonReleaseMask		(1L<<3)  
#define EnterWindowMask			(1L<<4)  
#define LeaveWindowMask			(1L<<5)  
#define PointerMotionMask		(1L<<6)  
#define PointerMotionHintMask		(1L<<7)  
#define Button1MotionMask		(1L<<8)  
#define Button2MotionMask		(1L<<9)  
#define Button3MotionMask		(1L<<10) 
#define Button4MotionMask		(1L<<11) 
#define Button5MotionMask		(1L<<12) 
#define ButtonMotionMask		(1L<<13) 
#define KeymapStateMask			(1L<<14)
#define ExposureMask			(1L<<15) 
#define VisibilityChangeMask		(1L<<16) 
#define StructureNotifyMask		(1L<<17) 
#define ResizeRedirectMask		(1L<<18) 
#define SubstructureNotifyMask		(1L<<19) 
#define SubstructureRedirectMask	(1L<<20) 
#define FocusChangeMask			(1L<<21) 
#define PropertyChangeMask		(1L<<22) 
#define ColormapChangeMask		(1L<<23) 
#define OwnerGrabButtonMask		(1L<<24) 

/* Event names.  Used in "type" field in XEvent structures.  Not to be
confused with event masks.  They start from 2 because 0 and 1
are reserved in the protocol for errors and replies. */

#define KeyPress		2
#define KeyRelease		3
#define ButtonPress		4
#define ButtonRelease		5
#define MotionNotify		6
#define EnterNotify		7
#define LeaveNotify		8
#define FocusIn			9
#define FocusOut		10
#define KeymapNotify		11
#define Expose			12
#define GraphicsExpose		13
#define NoExpose		14
#define VisibilityNotify	15
#define CreateNotify		16
#define DestroyNotify		17
#define UnmapNotify		18
#define MapNotify		19
#define MapRequest		20
#define ReparentNotify		21
#define ConfigureNotify		22
#define ConfigureRequest	23
#define GravityNotify		24
#define ResizeRequest		25
#define CirculateNotify		26
#define CirculateRequest	27
#define PropertyNotify		28
#define SelectionClear		29
#define SelectionRequest	30
#define SelectionNotify		31
#define ColormapNotify		32
#define ClientMessage		33
#define MappingNotify		34
#define LASTEvent		35	/* must be bigger than any event # */
/* 
  et une classification pour exprimer la dependance
  de des widgets non directement crees par xbVLISP
   */

/* pour les representations de types */
extern XrmQuark qkBoolean,  qkCallback,  qkCallProc,  qkColor,  qkDisplay,  qkEditMode,  qkFile,  qkFontStruct,  qkFunction,  qkImmediate,  qkInt,  qkJustify,  qkLongBoolean,  qkPixmap,  qkPointer,  qkShort,  qkString,  qkStringTable,  qkUnsignedChar,  qkTranslationTable,  qkWindow, qkDimension ,  qkPosition, qkWidget, qkPixel;


/* pour les classes de base Xt */
extern XrmQuark  qkCore, qkComposite, qkConstraint, qkShell, qkTopLevelShell, qkTransientShell, qkOverrideShell, qkApplicationShell, qkSimple;

/* pour les widgets Athena */
extern XrmQuark  qkawTree, qkawRepeater, qkawPorthole, qkawPanner, qkawPaned, qkawDialog, qkawList, qkawAsciiText, qkawAsciiDisck,  qkawForm, qkawLabel, qkawCommand, qkawBox, qkawViewport, qkawScroll, qkawGrip, qkawClock, qkawLogo, qkawSme, qkawSmeBSB, qkawStripChart, qkawTextSink, qkawTextSrc, qkawSimpleMenu, qkawToggle, qkawSmeLine, awAsciiSrc ;

/* pour les widgets speciaux */
extern XrmQuark qkTortueObject, qkXbvlisp, qkDrawxbvl, qkbrTree ;

/* pour debug 
#define XtFree(obj)   printf("%d\n", (Int)obj) */

#ifndef XtSpecificationRelease

#ifdef STDC_HEADERS
    extern CHAR *XeWidgetName(Widget w);
#else
    extern CHAR *XeWidgetName();
#endif

#else 
#define XeWidgetName(w) (w ? XtName(w): NULL)
#endif

#endif








