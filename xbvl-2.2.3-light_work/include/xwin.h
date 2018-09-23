/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 	xwin.h
 * 	Ernesto Sendoya		 Sat Sep 29 13:42:41 WET DST 1990
 */

#ifndef xwin_h
#define xwin_h

#ifndef CHAR
#ifdef X_USE_LATIN1
#define CHAR unsigned char
#else
#define CHAR char
#endif
#endif

#include <stdio.h>
#include <string.h>


/*
Le 'define' suivant essaye de prévenir cette erreur qui survient des compilations
sous GNU/Linux.

/usr/include/X11/Xlib.h:74: conflicting types for `wchar_t'
/usr/local/lib/gcc-lib/i686-pc-linux-gnu/3.0/include/stddef.h:287: previous declaration of `wchar_t'

!!! Attention !!!
Ceci n'est pas portable.

Valvassori Moïse -- Sat Nov 17 21:40:46 CET 2001
 */
#define ISC
#include <X11/Intrinsic.h>
#undef ISC
#include <X11/Shell.h>
#include <X11/keysym.h>
#include <X11/StringDefs.h>


#ifndef XtSpecificationRelease
#include <X11/Label.h>
#include <X11/Command.h>
#include <X11/Form.h>
#include <X11/Box.h>
#include <X11/List.h>
#include <X11/AsciiText.h>
#include <X11/Viewport.h>
#include <X11/Scroll.h>
#include <X11/Dialog.h>
#include <X11/Logo.h> 
#include <X11/Grip.h>
#include <X11/Clock.h> 

#ifndef CONTINU_MY_SYS_BIZARRE
/* pour palier a ma vieille release X11 */
#include <X11/Paned.h>
#include <X11/Toggle.h>
#endif
#else 
#ifdef XAW3D
	/* esperant que la compativilite avec le Relais 4 sera maintenue */
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/Box.h>
#include <X11/Xaw3d/List.h>
#include <X11/Xaw3d/AsciiText.h>
#include <X11/Xaw3d/Viewport.h>
#include <X11/Xaw3d/Scrollbar.h>
#include <X11/Xaw3d/Dialog.h>
#include <X11/Xaw3d/Paned.h>

#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/Sme.h>
#include <X11/Xaw3d/SmeLine.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/StripChart.h>
#include <X11/Xaw3d/TextSink.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/TextSrc.h>
#include <X11/Xaw3d/Text.h>
#include <X11/Xaw3d/Grip.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/AsciiSink.h>


/* #if XtSpecificationRelease==5 */
#include <X11/Xaw3d/Porthole.h>
#include <X11/Xaw3d/Panner.h>
#include <X11/Xaw3d/Repeater.h>
#include <X11/Xaw3d/Tree.h>
#else
	/* esperant que la compativilite avec le Relais 4 sera maintenue */
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Paned.h>

#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Sme.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/StripChart.h>
#include <X11/Xaw/TextSink.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/TextSrc.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Grip.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/AsciiSink.h>


/* #if XtSpecificationRelease==5 */
#include <X11/Xaw/Porthole.h>
#include <X11/Xaw/Panner.h>
#include <X11/Xaw/Repeater.h>
#include <X11/Xaw/Tree.h>
#endif
#include "R3toR4.h"
/* #endif */

#include "BrTree.h"
#endif

#if (IRIS_GL | OPEN_GL)
#include <X11/Xirisw/GlxDraw.h>
#endif
#if MESA_GL
#include <GL/xmesa.h>
#include <GL/gl.h>
#include <GL/GLwDrawA.h>
#endif

#include "Xbvlisp.h"
#include "Drawxbvl.h"
#include "Tortue.h"

/* 
 * nom de ressources et nom de classes de ressources
 * definies par l'interface.
 */

#define XtNWinPages "winPages"
#define XtCWinPages "WinPages"
#define XtNWinLarge  "winLarge"
#define XtCWinLarge  "WinLarge"
#define XtNWinHaut   "winHaut"
#define XtCWinHaut   "WinHaut"

/*  defauts */

#define MIN_PAGES      1
#define MAX_PAGES     20
#define MIN_HAUT       1
#define MAX_HAUT      LINES
#define MIN_LARGE      20
#define MAX_LARGE     COLS
#define DEFAUT_HAUT    5
#define DEFAUT_LARGE  40
#define DEFAUT_PAGES   2

/* types de fenetres */

#define LISPWIN    1
#define DRAWIN     2

extern Display *display;
extern CHAR main_top_name[];
/* extern Atom xbvl_com_atome; :: non defini : definition en "dure" */
#define xbvl_com_atome    0    /* pas de valeur... */
extern Widget aide_wid;

extern Widget Xbvlisp_courr ;

/* hack pour reccuperer correctement les ressources. */

union u_XtArgVal {
  XtArgVal argval;
  Boolean boolean;
  Int entier;
  String string ;
  Pixel pixel;
  Dimension dimension;
  Position position;
  float flotant;
};


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


extern Widget top_widget, 	/* la racine */
  titre_widget;         	/* affichage du titre */
extern Int nbr_fenetres ;

/* pour l'affichage des menus */

extern Widget menuVlisp;


#define FONTHEIGHT(font)  ((font)->max_bounds.ascent+(font)->max_bounds.descent)
#define  FONTWIDTH(font)  (font)->max_bounds.width
#define  IS_POPUP_CLASS(clas)   (  (Int)clas == (Int)overrideShellWidgetClass\
				 ||(Int)clas == (Int)transientShellWidgetClass\
			         ||(Int)clas == (Int)topLevelShellWidgetClass\
				 ||(Int)clas == (Int)simpleMenuWidgetClass)

#define  IS_POPUP_WIDGET(w)     IS_POPUP_CLASS(XtClass(w))
#define RETOUR_AUTO 1
#define INSERT_DONE 0  
#define CARS_MARGE_DROITE  1


/* noms des objests graphiques  */

#define WINS_NAMES "bVLISP"   /* les fenetres crees automatiquement ont
				 un nom compose "bVLISPnn" ou 
				 "nn" est le  numero de la fenetre.
			       */

#define TOP_APP_CLASS  "XbVLISP"

#define XBVL_APP_CLASSE_NAME TOP_APP_CLASS /* "bVLISP" */
#define MENU_SHELL_NAME    "menu"

#define FORMS_NAME "Forme"
#define FORM_MENU_NAME  FORMS_NAME
#define FORM_INTERFACE_NAME FORMS_NAME
#define TAILLE_TAMPON BUFSIZ*10
#define  DIALOGUE   16

#define BUF_CIRCULAIRE_TAILLE 128

#define CALLBACK_INPUT          9999
#define XBVL_PROTOCOLE  "XBVLISP-PROTOCOLE"
#include "proto.h"
#include "Objets.h"

#ifndef  XtSpecificationRelease
#define  XtPointer       caddr_t
#endif

#endif



