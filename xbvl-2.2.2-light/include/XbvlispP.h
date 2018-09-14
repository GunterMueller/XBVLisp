
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	XbvlispP.h
 *  Widget D'interaction avec xbVLISP
 *    Ernesto Sendoya, avril 92
 */




/* Declarations Privees du widget d'interaction avec xbVLISP */

#ifndef _XbvlispP_h
#define _XbvlispP_h


/* include superclass private header file */
/* ce prototype est unes sous-classe de Text */

#include "Xbvlisp.h"
#include "actions.h"

#ifndef XtSpecificationRelease
#include <X11/AsciiTextP.h>   
#else
#ifdef XAW3D
#include <X11/Xaw3d/AsciiTextP.h>
#else
#include <X11/Xaw/AsciiTextP.h>
#endif
#endif


/*  structures de donnees pour la tortue */
/*  ------------------------------------- */

/* fonctions */

extern void bougetortue();
extern void drawtortue();


typedef 
struct tortues {
  Int orientation ;       /* de 0 a 360 degres */
  short x, y ;            /* coordonnees de la fenetre (en pixels) */
  short base, longeur;    /* dimention de la tortue */
  Boolean draw;           /* on trace le chemin ou pas */
  GC gc_draw ;            /* contexte graphique pour dessiner */
  GC gc_Xor;              /* le contexte graphique de la tortue */
  struct xinterface *interface;  /* la fenetre ou la tortue se deplace */
  struct tortue *next_tortue, *prev_tortue;
} TortueList ;

typedef struct {
  Int extention;
} XbvlispClassPart;

typedef struct _XbvlispClassRec {
 CoreClassPart	        core_class;
  SimpleClassPart	simple_class;
  TextClassPart	        text_class;
  AsciiStringClassPart ascii_string_class;
  XbvlispClassPart	xbvlisp_class;
} XbvlispClassRec;

extern XbvlispClassRec xbvlispClassRec;

typedef   struct {
  /* ressources */
  Int lignes;         /* lignes de text du widget d'iteraction */
  Int colonnes;       /* colonnes ... */
  Int pages;          /* pages de texte gerees par le widget d'iteraction */
  Boolean toggle;     /* si == True on inserre du texte avec decalage a droite */
  /* nouvelle variables internes */
  String string;      /* le buffer d'Interaction */
  String killbuf;     /* le dernier text efface */
  GC  dessin_gc;      /* pour dessiner sur la widgette */
  Pixmap pixmap;      /* pour sauvegarder les dessins */
  TortueList *tortues; /* pour gerer des tortues */
  Int n_bytes ;
} XbvlispPart;

typedef struct _XbvlispRec {
  CorePart	core;
  SimplePart	simple;
  TextPart	text;
  AsciiStringPart ascii;
  XbvlispPart   xbvlisp;
} XbvlispRec;


#define GetXbvlispString(w, ptr)   ptr=w->xbvlisp.string


/* procedure privees */
#ifdef STDC_HEADERS
#else
#endif

#ifdef STDC_HEADERS
Int xwrightXbvl(XbvlispWidget w, Int n);
Boolean InsertStringXbvlisp(Widget w, CHAR *buf);
XtTextPosition InsertTextXbvlisp(Widget w, CHAR * buf);
Int  clean_to_addrXbvl(XbvlispWidget w, Int t, CHAR ** _buf);
static CHAR * calculXbvlBuffer(XbvlispWidget w,  CHAR * p );
CHAR * calculXbvlEditBuffer(Int l,  Int c, Int p, CHAR *s);
Boolean scrollTextXbvlisp (Widget w, Int dici, Int nlignes);
#else
Int xwrightXbvl();
Boolean InsertStringXbvlisp();
XtTextPosition InsertTextXbvlisp();
static CHAR * calculXbvlBuffer();
CHAR  * calculXbvlEditBuffer();
Boolean scrollTextXbvlisp ();

#endif

#endif  /* _XbvlispP_h */




