/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* fonctions pour interner des widgets et ressources comme des objets bVLISP
 *               ObjAtome.c
 *		Ernesto Sendoya
 *		  Mars 92
 * 03/93 D.P. : rajout du Type Pixel comme explicitement connu.
 */

#include <Objets.h>

Widget Xbvlisp_courr ;


/* pour les classes de base Xt */
XrmQuark  qkCore, qkComposite, qkConstraint, qkShell, qkTopLevelShell, qkTransientShell, qkOverrideShell, qkApplicationShell, qkSimple;

/* pour les widgets Athena */
XrmQuark  qkawTree, qkawMapTree, qkawRepeater, qkawPorthole, qkawPanner, qkawPaned, qkawDialog, qkawList, qkawAsciiText, qkawAsciiDisck,  qkawForm, qkawLabel, qkawCommand, qkawBox, qkawViewport, qkawScroll, qkawGrip, qkawClock, qkawLogo, qkawSme, qkawSmeBSB, qkawStripChart, qkawTextSink, qkawTextSrc, qkawSimpleMenu, qkawToggle, qkawSmeLine, qkawAsciiSrc ;

/* pour les widgets speciaux */
XrmQuark qkTortueObject, qkXbvlisp, qkDrawxbvl, qkbrTree;

#define RETURN(widget_class)   { InitializeWidgetClass(widget_class); return widget_class ; }

/*-----------------------------------------------------------------------
 * retourne une classe de widgets correspondant a la representation LISP
 *-----------------------------------------------------------------------*/

WidgetClass  getWidgetClassFromString(name)
     CHAR *name;
{
  extern void InitializeWidgetClass();
  XrmQuark qkcourrent;

  qkcourrent = XrmStringToQuark(name);

  /* classes de base du toolkit */
  if(qkcourrent == qkCore)  RETURN(widgetClass);
  if(qkcourrent == qkComposite) RETURN(compositeWidgetClass);
  if(qkcourrent == qkConstraint) RETURN(constraintWidgetClass);
  /* les widgets Shell */
  if(qkcourrent == qkShell) RETURN(shellWidgetClass);
  if(qkcourrent == qkTopLevelShell) RETURN(topLevelShellWidgetClass);
  if(qkcourrent == qkApplicationShell) RETURN(applicationShellWidgetClass);
  if(qkcourrent == qkOverrideShell) RETURN(overrideShellWidgetClass);
  if(qkcourrent == qkTransientShell) RETURN(transientShellWidgetClass);

  /* classes specifiques a bVLISP */
  if(qkcourrent == qkDrawxbvl) RETURN(drawxbvlWidgetClass);
  if(qkcourrent == qkXbvlisp) RETURN(xbvlispWidgetClass);
  if(qkcourrent == qkTortueObject)RETURN(tortueObjectClass); /* Objet */

  if(qkcourrent == qkawGrip ) RETURN(gripWidgetClass) ;
  if(qkcourrent == qkbrTree)   RETURN(brTreeWidgetClass);          

  /* les widgets Athena  */
  if(qkcourrent == qkawAsciiText || qkcourrent == qkawAsciiDisck)
#ifndef XtSpecificationRelease
    {
      if(qkcourrent == qkawAsciiDisck)
	RETURN(asciiDiskWidgetClass);  /* Core->Simple->Text->AsciiDisck */
      RETURN(asciiStringWidgetClass);  /* Core->Simple->Text->AscciText */
  }
  if(qkcourrent == qkawLogo ) RETURN(logoWidgetClass) ; 
  if(qkcourrent == qkawClock ) RETURN(clockWidgetClass) ; 
#else
  RETURN(asciiTextWidgetClass);

  if(qkcourrent == qkawSme ) RETURN(smeObjectClass);
  if(qkcourrent == qkawSmeLine) RETURN(smeLineObjectClass);
  if(qkcourrent == qkawSmeBSB ) RETURN(smeBSBObjectClass) ;
  if(qkcourrent == qkawTextSink ) RETURN(textSinkObjectClass) ;
  if(qkcourrent == qkawTextSrc) RETURN(textSrcObjectClass) ;
  if(qkcourrent == qkawAsciiSrc) RETURN(asciiSrcObjectClass);
  
  if(qkcourrent == qkawSimpleMenu ) RETURN(simpleMenuWidgetClass) ;
  if(qkcourrent == qkawStripChart) RETURN(stripChartWidgetClass);
  if(qkcourrent == qkawSimpleMenu ) RETURN(simpleMenuWidgetClass) ;

/* #if XtSpecificationRelease > 4 */
  if(qkcourrent == qkawPanner)RETURN(pannerWidgetClass);          /* Core->Simple->Panner */
  if(qkcourrent == qkawPorthole) RETURN(portholeWidgetClass);     /* Core->Composite->Porthole */
  if(qkcourrent == qkawRepeater)RETURN(repeaterWidgetClass);      /* Core->Composite->Porthole */
  if(qkcourrent == qkawTree)   RETURN(treeWidgetClass);           /* Core->Composite->Porthole */
/* #endif */
#endif
#ifndef CONTINU_MY_SYS_BIZARRE
  if(qkcourrent == qkawToggle ) RETURN(toggleWidgetClass) ;
  if(qkcourrent == qkawPaned)RETURN(panedWidgetClass);             /* Core->Composite->Constraint->Panel */
#endif
  
  if(qkcourrent == qkawForm)RETURN(formWidgetClass);               /* Core->Composite->Constraint->Form */
  if(qkcourrent == qkawLabel) RETURN(labelWidgetClass);            /* core->Simple->label */
  if(qkcourrent == qkawCommand)   RETURN(commandWidgetClass);      /* Core-> Simple->Command */
  if(qkcourrent == qkawBox)RETURN(boxWidgetClass);                 /* Core->Composite->Box */
  if(qkcourrent == qkawViewport)RETURN(viewportWidgetClass);       /* Core->Composit->Viewport */
  if(qkcourrent == qkawScroll)RETURN(scrollbarWidgetClass);        /* Core->Composit->Scroll */
  if(qkcourrent == qkSimple) RETURN(simpleWidgetClass);
  if(qkcourrent == qkawList) RETURN(listWidgetClass);
  if(qkcourrent == qkawDialog) RETURN(dialogWidgetClass);
  return NULL;
}
#if 0
/*
 * le systeme alloue un nom unique au widgets.
 * cette allocation permet de garder la coherence entre
 * les noms des widgets et les nom des atomes representants
 * les widgets comme des objets lisp.
 * Cela enleve beaucoup de souplesse de configuration par
 * des fichiers de config, ce qui n'est pas si grave puisque
 * le controle de ressources par programme lisp est complet.
 */

CHAR * newObjetName(name)
     CHAR *name;
{
  CHAR new_name[128];
  static Int cnt = 0;
 
  /* le nom de l'application est internee tel quel */
  if(strcmp(vlargv[0], name) == 0 || *name == '*') 
    return XtNewString(name); 
  sprintf(new_name, "W%d-%s", cnt++, name);
  return XtNewString(new_name);
}
#endif


/* la representation des types Xt */
XrmQuark   qkBoolean,  qkCallback,  qkCallProc,  qkColor,  qkDisplay,  qkEditMode,  qkFile,  qkFontStruct,  qkFunction,  qkImmediate,  qkInt,  qkJustify,  qkLongBoolean,  qkPixmap,  qkPointer,  qkShort,  qkString,  qkStringTable,  qkUnsignedChar,  qkTranslationTable,  qkWindow,   qkDimension ,  qkPosition, qkWidget, qkPixel;

void init_quarks()
{
  /* types X */

  qkBoolean = XrmStringToQuark(XtRBoolean);
  qkCallback = XrmStringToQuark(XtRCallback);
  qkCallProc = XrmStringToQuark(XtRCallProc);
  qkColor = XrmStringToQuark(XtRColor);
  qkDisplay = XrmStringToQuark(XtRDisplay);
  qkEditMode = XrmStringToQuark(XtREditMode);
  qkFile = XrmStringToQuark(XtRFile);
  qkFontStruct = XrmStringToQuark(XtRFontStruct);
  qkFunction = XrmStringToQuark(XtRFunction);
  qkImmediate = XrmStringToQuark(XtRImmediate);
  qkInt = XrmStringToQuark(XtRInt);
  qkJustify = XrmStringToQuark(XtRJustify);
  qkLongBoolean = XrmStringToQuark(XtRLongBoolean);
  qkPixmap = XrmStringToQuark(XtRPixmap);
  qkPointer = XrmStringToQuark(XtRPointer);
  qkShort = XrmStringToQuark(XtRShort);
  qkString = XrmStringToQuark(XtRString);
  qkStringTable = XrmStringToQuark(XtRStringTable);
  qkUnsignedChar = XrmStringToQuark(XtRUnsignedChar);
  qkTranslationTable = XrmStringToQuark(XtRTranslationTable);
  qkWindow = XrmStringToQuark(XtRWindow);
  qkWidget = XrmStringToQuark(XtRWidget);

  qkDimension = XrmStringToQuark(XtRDimension);
  qkPosition = XrmStringToQuark(XtRPosition);
  qkPixel = XrmStringToQuark(XtRPixel);
}

/*
 * initialisation de Quarks representant les classes des widgets
 */
static void initWidgetQuarks()
{
  qkawSimpleMenu = XrmStringToQuark( XtNawSimpleMenu  );
  qkawSme = XrmStringToQuark( XtNawSme  );
  qkawSmeLine = XrmStringToQuark( XtNawSmeLine  );
  qkawSmeBSB = XrmStringToQuark( XtNawSmeBSB  );
  qkawStripChart = XrmStringToQuark( XtNawStripChart  );
  qkawTextSink = XrmStringToQuark( XtNawTextSink  );
  qkawToggle = XrmStringToQuark( XtNawToggle  );
  qkawTextSrc = XrmStringToQuark( XtNawTextSrc  );
  qkawLogo = XrmStringToQuark( XtNawLogo  );
  qkawGrip = XrmStringToQuark( XtNawGrip  );
  qkawClock = XrmStringToQuark( XtNawClock  );
  qkawSimpleMenu = XrmStringToQuark( XtNawSimpleMenu  );
  qkawToggle = XrmStringToQuark( XtNawToggle  );
  qkawAsciiSrc = XrmStringToQuark( XtNawAsciiSrc  );

  qkCore = XrmStringToQuark( XtNCore );
  qkComposite = XrmStringToQuark( XtNComposite );
  qkConstraint = XrmStringToQuark( XtNConstraInt );
  qkawAsciiText = XrmStringToQuark( XtNawAsciiText );
  qkawAsciiDisck = XrmStringToQuark( XtNawAsciiDisc );
  qkawLabel = XrmStringToQuark( XtNawLabel );
  qkawCommand = XrmStringToQuark( XtNawCommand );
  qkawBox = XrmStringToQuark( XtNawBox );
  qkawViewport = XrmStringToQuark( XtNawViewport );
  qkawScroll = XrmStringToQuark( XtNawScroll );
  qkawForm = XrmStringToQuark( XtNawForm );
  qkawList = XrmStringToQuark( XtNawList );
  qkawDialog = XrmStringToQuark( XtNawDialog );

  qkDrawxbvl = XrmStringToQuark( XtNDrawxbvl );
  qkXbvlisp = XrmStringToQuark( XtNXbvlisp );
  qkawPaned = XrmStringToQuark( XtNawPaned );
  qkTortueObject = XrmStringToQuark( XtNTortueObject );

/* #   if XtSpecificationRelease > 4 */
  qkawPorthole = XrmStringToQuark( XtNawPorthole );
  qkawPanner = XrmStringToQuark( XtNawPanner );
  qkawRepeater = XrmStringToQuark( XtNawRepeater );
  qkawTree = XrmStringToQuark( XtNawTree );
/* #  else
  qkawPorthole = NULLQUARK;
  qkawPanner = NULLQUARK;
  qkawRepeater = NULLQUARK;
  qkawTree = NULLQUARK;
#   endif */

  qkbrTree = XrmStringToQuark( XtNbrTree );

  qkShell = XrmStringToQuark( XtNShell );
  qkTopLevelShell = XrmStringToQuark( XtNTopLevelShell );
  qkApplicationShell = XrmStringToQuark( XtNApplicationShell );
  qkOverrideShell = XrmStringToQuark( XtNOverrideShell );
  qkTransientShell = XrmStringToQuark( XtNTransientShell );

  qkSimple = XrmStringToQuark( XtNawSimple );

}


/*
 * initialisation des choses dont les objets graphiques auront besoin
 */
void initObjets()
{
  initWidgetQuarks();
}







