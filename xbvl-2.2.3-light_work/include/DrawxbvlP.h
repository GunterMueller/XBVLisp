/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	   DrawxbvlP.h
 *    Declarations Privees pour le
 *  Widget feille de dessin de xbVLISP.
 *    Les Widgets appartenant 
 *    Ernesto Sendoya, mai 92
 */


#ifndef _DrawxbvlP_h
#define _DrawxbvlP_h




#include <X11/IntrinsicP.h>   
#include "Drawxbvl.h"
#include "TortueP.h"



typedef struct {
  Int extention;
} DrawxbvlClassPart;

typedef struct _DrawxbvlClassRec {
  CoreClassPart	        core_class;
  CompositeClassPart composite_class;
  DrawxbvlClassPart	drawxbvl_class;
} DrawxbvlClassRec;

extern DrawxbvlClassRec drawxbvlClassRec;
typedef   struct {
  /* atributs du gc: modifient dynamiquement le contexte graphique  */
  Int fonction_logique;   /* fonction logique de dessin */
  Int line_width ;       /* largeur du trace */
  Pixel foreground_pixel;/* la couleur du dessin */
  /* on herite du background du core */
  XFontStruct *font;      /* police de caracteres */
  /* donnees prives */
  GC  gc;             /* pour dessiner sur la widgette */
  Pixmap pixmap;      /* pour sauvegarder les dessins */
  Int width, height;  /* pour les modifications de taille */
  TortueObject tortues;   /* pour gerer des tortues */
} DrawxbvlPart;

typedef struct _DrawxbvlRec {
  CorePart	core;
  CompositePart composite;
  DrawxbvlPart   drawxbvl;
} DrawxbvlRec;

#endif  /* _DrawxbvlP_h */
