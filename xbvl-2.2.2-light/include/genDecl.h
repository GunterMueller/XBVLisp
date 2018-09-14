
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	XbvlispP.h
 *  declarations des fonctions et donnees
 *  necessaires pour la communication du
 *  VLISP avec les widgets d'interaction
 *    Ernesto Sendoya, mai 92
 */









#include "Xbvlisp.h"

/* provisoire */
extern Int toggle;

/* le widget Xbvlisp d'interaction courrent avec xbVLISP */

extern Widget Xbvlisp_courr ;

/* pour faciliter la vie de tou le monde */

#define GetXbvlispColonnes(w, cols)  XteGetValue(w, XtNcolonnes, (XtArgVal *)&cols); cols -= 1
#define GetXbvlispString(w, ptr)   ptr=w->xbvlisp.string
/* XteGetValue(w, XtNstring, (XtArgVal *)&ptr) */

/* et les fonctions communess, sans prototyper, trop complique */

extern XtEventHandler handleFocusChange();

#if 0
#ifndef XtSpecificationRelease
extern XtEventHandler handleFocusChange(Widget  w, Widget no, register XFocusChangeEvent *event);
#else
extern XtEventHandler handleFocusChange(Widget  w, Widget no, register XFocusChangeEvent *event, Boolean * bidon);
#endif
#endif
