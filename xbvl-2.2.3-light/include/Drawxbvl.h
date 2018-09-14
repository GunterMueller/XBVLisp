/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	    Drawxbvl.h
 *   Declarations Publiques pour le
 *  Widget feille de dessin de xbVLISP.
 *    Les Widgets appartenant 
 *    Ernesto Sendoya, mai 92
 */


#ifndef _Drawxbvl_h
#define _Drawxbvl_h

/****************************************************************
 *
 * Drawxbvl widget
 *
 ****************************************************************/

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 foreground	     Foreground 	RPixel		XtDefaultForeground
 lineWidth	     lineWidth		RInt     	0   ** correspond a 1 hardware **
 fonctionLogique     FonctionLogique	Pointer     	1   ** correspond ... **
 Callback            Callback		Pointer		NULL   ** a faire **
 tortues	     tortues		Liste     	nil

*/
/* les nouveaux noms de ressources */

#define XtNDrawxbvl             "Drawxbvl"

#ifndef XtNlineWidth
#define XtNlineWidth            "lineWidth"
#endif
#define XtNfonctionLogique      "fonctionLogique"
#define XtNtortues              "tortues"

#ifndef XtCLineWidth
#define XtCLineWidth            "LineWidth"
#endif
#define XtCFonctionLogique      "FonctionLogique"
#define XtCTortues             "Tortues"

/* declare specific DrawxbvlWidget class and instance datatypes */

typedef struct _DrawxbvlClassRec   *	DrawxbvlWidgetClass;
typedef struct _DrawxbvlRec     *	DrawxbvlWidget;

/* declare the class constant */

extern WidgetClass drawxbvlWidgetClass;

#ifdef STDC_HEADERS
extern void afficheMenuToDraw(Widget widget, String menu);
#else
extern void afficheMenuToDraw();
#endif

#endif /* _Drawxbvl_h */





