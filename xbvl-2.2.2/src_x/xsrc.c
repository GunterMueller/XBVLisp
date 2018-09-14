/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* les fonctions de base pour la creation des fenetres X
 *               Xsrc.c
 *		Ernesto Sendoya
 *		  janvier  92
 *              Essai de manipulation de sources de widgettes ascciText		
 */

#include <vlisp.h>




#ifdef XtSpecificationRelease /* Relais x > 3 */
#ifdef XAW3D
#include <X11/Xaw3d/Text.h>
#include <X11/Xaw3d/AsciiSink.h>
#else
#include <X11/Xaw/Text.h>
#include <X11/Xaw/AsciiSink.h>
#endif
#endif

extern Int xchgsrc(); /* change la source d'une widgette */

Int xchgsrc() /* (xchgsrc [ win | top win ] string) */
{
  CHAR *s ;
  Arg args[3];
  Int n;
  XtTextSource xsrc, nsrc;

  Widget widget = get_widget(&n);
  
  if(widget && get_oneName(&s) == True){
    char *tmp = NULL;
    Int edit_mode = XttextEdit;

    n = 0;
    XtSetArg(args[n], XtNeditType, &edit_mode); n ++;
    XtSetArg(args[n], XtNstring, &tmp); n ++;
    XtGetValues(widget, args, n);

    xsrc =  XtTextGetSource(widget);
    n = 0; /* on garde le type d'edition */
    XtSetArg(args[n], XtNeditType, edit_mode); n ++;
    XtSetArg(args[n], XtNstring, s); n ++;

#ifndef XtSpecificationRelease
    nsrc = XtStringSourceCreate(widget, args, n);
#else
    /* on peut acceder au sources de widgets d'interaction normalement,
       comme un objet graphique quelconque */
    nsrc = XtCreateWidget("sourceText", asciiSrcObjectClass, widget, args, n);
#endif

    XtTextSetSource(widget, nsrc, 0);

    XtStringSourceDestroy(xsrc);
    if(tmp)XtFree(tmp);
    a1 = t;
  }
  else a1 = nil;
  derec;

}

  
