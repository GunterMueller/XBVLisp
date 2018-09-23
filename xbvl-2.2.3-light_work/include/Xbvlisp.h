/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	XbvlispP.h
 *  Widget D'interaction avec xbVLISP
 *    Ernesto Sendoya, avril 92
 */




#ifndef _Xbvlisp_h
#define _Xbvlisp_h

/****************************************************************
 *
 * Xbvlisp widget
 *
 ****************************************************************/



/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 destroyCallback     Callback		Pointer		NULL
 displayPosition     TextPosition	int		0
 editType	     EditType		XtTextEditType	XttextRead
 file		     File		String		NULL
 font		     Font		XFontStruct*	Fixed
 foreground	     Foreground		Pixel		Black
 height		     Height		Dimension	font height
 insertPosition	     TextPosition	int		0
 leftMargin	     Margin		Dimension	2
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 selectTypes	     SelectTypes	Pointer		(internal)
 selection	     Selection		Pointer		empty selection
 sensitive	     Sensitive		Boolean		True
 string		     String		String		NULL
 textOptions	     TextOptions	int		0
 width		     Width		Dimension	100
 x		     Position		Position	0
 y		     Position		Position	0
 lignes              Int                Int             1
 colonnes            Int                Int             30
 pages               Int                Int             1

*/

#include <X11/Intrinsic.h>
#include <X11/Core.h>

#ifdef UPARIS8_BIZARRE
#include <Text.h>
#else
#ifndef XtSpecificationRelease
#include <X11/Text.h>
#else
#ifdef XAW3D
#include <X11/Xaw3d/Text.h>
#else
#include <X11/Xaw/Text.h>
#endif
#endif
#endif


/* define any special resource names here that are not in <X11/StringDefs.h> */

#define XtNlignes                "lignes"
#define XtNcolonnes              "colonnes"
#define XtNpages                 "pages"
#define XtCLignes                "Lignes"
#define XtCColonnes              "Colonnes"
#define XtCPages                 "Pages"

#define XtNXbvlisp               "Xbvlisp"


#define XtCXbvlispResource		"XbvlispResource"


#define DEFAULT_LIGNES         15
#define DEFAULT_COLONNES       80
#define DEFAULT_PAGES          3

#define MIN_COLONNES     20
#define MIN_LIGNES        1

/* declare specific XbvlispWidget class and instance datatypes */

typedef struct _XbvlispClassRec *	XbvlispWidgetClass;
typedef struct _XbvlispRec *		XbvlispWidget;


/* declare the class constant */

extern WidgetClass xbvlispWidgetClass;


/* pour recuperer les options utilisateur */
	/* et defauts */

typedef struct {
  XFontStruct *font;
  Int colonnes;       /* nbr. de colonnes */
  Int lignes;         /* nbr. de lignes */
  Int pages;          /* nbr. de pages */
}  xbVlispRess, * xbVlispRessPtr;

extern xbVlispRess xvl_data;

#ifndef XtSpecificationRelease
#define  XtSelectionCallbackProc void
#endif

/* prototypes */
#ifdef STDC_HEADERS

extern Boolean copyFiltre(CHAR * bfin,CHAR * ptr, CHAR *str, Int p);
extern Int InsertCharXbvlisp(Widget w, XEvent * e, CHAR ** ss, Int  cc);
extern Boolean getCursorCoordsXbvl(Widget w , Position * x_w, Position * y_w, Position * x_r, Position * y_r);
extern Int getXbvlColonnes(Widget w);
extern Boolean getWidgetFont(Widget widget, XFontStruct **font);
extern void getXbvlRessources(Widget widget , xbVlispRess * rdat);
extern void copyOldString(CHAR *dest, CHAR *src);
extern void getCursorPosXbvl( XbvlispWidget w, Int *ret_ligne, Int *ret_col);
extern void setCursorPosition(XbvlispWidget w, Int  ligne,  Int col);
extern Widget xCreateXbVlispWidget(CHAR *root, CHAR *name, Int l, Int c, Int p, Int x, Int y);
extern Widget xCreateManagedXbvl(CHAR *name, Widget root, Int lignes, Int colonnes, Int pages,  XFontStruct *fon);
extern CHAR * calculXbvlEditBuffer(Int lignes, Int colonnes, Int pages, CHAR * ptr);
extern Boolean xgetBufin(CHAR *bfin);
extern Int changeText(Widget w,  CHAR *  s); 
#else

extern Boolean copyFiltre();
extern Int InsertCharXbvlisp();
extern Boolean getCursorCoordsXbvl();
extern Int getXbvlColonnes();
extern Boolean getWidgetFont();
extern void getXbvlRessources();
extern void copyOldString();
extern void getCursorPosXbvl();
extern void setCursorPosition();
extern Widget xCreateXbVlispWidget();
extern Widget xCreateManagedXbvl();
extern CHAR * calculXbvlEditBuffer();
extern Boolean xgetBufin();
extern Int changeText();
#endif

#endif  /* _Xbvlisp_h */





