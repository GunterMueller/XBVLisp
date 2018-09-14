/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*  Des actions globals de l'application
 *  	         xActions.c
 *        Ernesto Sendoya, avril 92
 */

#include <vlisp.h> 

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>


/* ARGSUSED */
void faire_beep(w, event, params, num_params)
     Widget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  if(*num_params == 1 && params[0][0] == '1')
    XBell(display, 50);
}

/* ARGSUSED */
void toggle_background_color( w, event , params, num_params)
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num_params;
{
  Arg marg[1];
  Pixel color;

  XtSetArg(marg[0], XtNbackground,&color );
  XtGetValues(w , marg, 1);

  XtSetArg(marg[0], XtNbackground, ~color );
  XtSetValues(w, marg, 1);
}

/* ARGSUSED */
void toggle_foreground_color( w, event , params, num_params )
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num_params;
{
  Arg marg[1];
  Pixel color;

  XtSetArg(marg[0], XtNforeground,&color );
  XtGetValues(w , marg, 1);

  XtSetArg(marg[0], XtNforeground, ~color );
  XtSetValues(w, marg, 1);
}

/* ARGSUSED */
void toggle_border_color( w, event, params, num_params )
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num_params;
{
  Arg marg[1];
  Pixel color;

  XtSetArg(marg[0], XtNborderColor,&color );
  XtGetValues(w , marg, 1);

  XtSetArg(marg[0], XtNborderColor, ~color );
  XtSetValues(w, marg, 1);
}

/*ARGSUSED */
void stop_x(w, event, params, num_params)
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num_params;
{
  stop();
}

/* ARGSUSED */
void cree_bVLISP_window(w, event, params, num_params)
     Widget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  cree_new_window(w, NULL, NULL);
}
/*-----------------------------------------------------------------------
 * communique a l'interprete les expressions en argument de de l'action.
 *-----------------------------------------------------------------------*/
/*ARGSUSED */
void communiqExpressions(w, event, params, num_params)
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num_params;
{
  register Int i = *num_params;
  while(i > 0)
    comToXbvl(w, params[*num_params - i--], NULL);
}
