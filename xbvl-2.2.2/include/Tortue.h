
             /* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	                  Tortue.h
 *  Declarations publiques d'un Objet utilise pour dessiner sur
 *             Un Widget  D'interaction avec xbVLISP.
 *                Ernesto Sendoya, mai 92
 */



#ifndef _Tortue_h
#define _Tortue_h

/***********************************************************************
 *
 *                        Object Tortue
 *
 ***********************************************************************/

#include <X11/Intrinsic.h>

#ifdef  XtSpecificationRelease
#include <X11/Object.h>
#else 
#include <X11/Core.h>
#endif
#include <X11/StringDefs.h>

#include <math.h>




/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
orientation          XtCInt             XtRInt               0
   x                 XtCPosition        XtRPosition          0
   y                 XtCPosition        XtRPosition          0
 base                XtCInt             XtRInt               0
longeur              XtCInt             XtRInt               0
 draw                XtCBoolean         XtRBoolien          TRUE
background           XtCPixel           XtRPixel      defaultBackground
foreground           XtCPixel           XtRPixel      defaultForeground
 font                XtCFont            XtRFont          
 fonction            XtCInt             XtRInt               0
 freres              XtCListe           XtRListe            nil


 ***********************************************************
 * definition de constantes symboliques pour les ressources
 */
#define DEFAULT_BASE 10
#define DEFAULT_LONGEUR 15

#define a_radianes(x)     (((float) x) * M_PI/180 )

#define XtNTortueObject      "Tortue"

#define XtNfreres            "freres"

#if  XtSpecificationReleas > 4
#define XtNorientation       "orientation"
#define XtCOrientation       "Orientation"
#define XtRWidget "Widget"
#endif

#define XtNwidget "widget"
#define XtCWidget "Widget"
#define XtNbase              "base"
#define XtCBase              "Base"

#define XtNlongeur           "longeur"
#define XtCLongeur           "Longeur"

#define XtNpencil            "pencil"
#define XtCPencil            "Pencil"

#ifndef XtRBitmap  
#define  XtRBitmap           "Bitmap"
#endif



/* Class record constants */

extern WidgetClass tortueObjectClass;

typedef struct _TortueClassRec * TortueObjectClass;
typedef struct _TortueRec *      TortueObject ;

/************************************************************
 
         Declarations des fonctions publiques 

 ************************************************************/


#ifdef STDC_HEADERS
extern void bougeTortue(TortueObject t, Int d, Int o);
extern void destroyTortue(Widget t);
#else
void bougeTortue();
extern void destroyTortue();
#endif

#endif /* _XawTextSrc_h */
/* DON'T ADD STUFF AFTER THIS #endif */


