
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	TortueP.h
 *  Cet Objet est utilise pour dessiner sur
 *  Un Widget  D'interaction avec xbVLISP.
 *    Ernesto Sendoya, mai 92
 */




#ifndef _TortueP_h
#define _TortueP_h

/***********************************************************************
 *
 * Tortue Object Private Data
 *
 ***********************************************************************/

#include <X11/Intrinsic.h>

#include "Tortue.h"

#include "DrawxbvlP.h" /* pour l'utiliser avec les widgets de dessin */


/************************************************************
 *
 * New fields for the Tortue object class record.
 *
 ************************************************************/

typedef struct {
  /* les methodes de la classe _tortue ?*/
  Int extention;
} TortueClassPart;

typedef 
  struct _TortueClassRec {
    CoreClassPart     object_class;
    TortueClassPart     tortue_class;
  } TortueClassRec;

extern TortueClassRec tortueClassRec;

/* L`objet pour xbvl dessiner sur les widget drawxbvl */
/* on doit construir un VRAIE objet de type tortue */
typedef 
  struct _tortue {
    Int orientation ;       /* de 0 a 360 degres */
    Int base, longeur;      /* dimentions de la tortue */
    Boolean pencil;         /* on trace le chemin ou pas */
    /* atributs du gc */
    Int line_width;         /* largeur du trace */
    Pixel foreground_pixel; /* color du trace */
    Pixel background_pixel; /* color du fond */
    Int fonction_logique;   /* fonction logique du dessin */
    XtCallbackList callbacks;/* les callbacks */
    Pixmap	pixmap;      /* un pixmap pour la tourtue */

    /* donnee privee */
    GC gc ;                 /* contexte graphique pour dessiner */
    GC gc_Xor;              /* le contexte graphique de la tortue */
    Widget widget;          /* le widget auquel la tortue appartient 
			       Drawxbvl ou Xbvlisp */
  } TortuePart, *TortuePartPtr ;

typedef struct _TortueRec {
  CorePart	core;
  TortuePart	tortue;
} TortueRec;


/* declarations des fonctions privees */

#ifdef STDC_HEADERS
static void afficheTortue(TortueObject t , Int b );
#else
static void afficheTortue();
#endif


#endif /* _TortueP_h */


