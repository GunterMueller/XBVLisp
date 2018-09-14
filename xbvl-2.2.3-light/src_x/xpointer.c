/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *            xpointer.c
 * fonction de manipulation du pointeur de la souris.
 * 
 */

#include <vlisp.h>

/*--------------------------------------------------------------------------  
 *  Fait le repositionnement du pointeur sur la fenetre passee en parametre
 *  cette fonction genere les evenements de changement de focus,
 *  et declanche les actions qui lui sont attachees.
 *--------------------------------------------------------------------------*/

#define  fairePosPointer(dest, x, y)  (void)XWarpPointer(display, None, XtWindow(dest), 0,0,0,0, x, y)


/*--------------------------------------------------------------------------
 * (xgetpospointer wid) sbr1
 *    retourne la position du pointeur de la souris 
 --------------------------------------------------------------------------*/

Int xgetpospointer() 
{
  Window root_return, child_return, w;
  int root_x, root_y, win_x, win_y; 
  unsigned int mask_ret ;

  if(isWidget(a1)) w = XtWindow(GET_WIDGET_FROM_ATOME(a1));
  else w = XtWindow(Xbvlisp_courr);

  (void) XQueryPointer(display, w,
		       &root_return, &child_return,
		       &root_x, &root_y, &win_x, &win_y, &mask_ret);
  a1 = cons(cranb((Int)root_x),
	    (cons(cranb((Int)root_y),
		  (cons(cranb((Int)win_x),
			cons(cranb((Int)win_y),nil))))));
  derec;
}


/*--------------------------------------------------------------------------  
 *  Fait le repositionnement du pointeur sur la fenetre passee en parametre
 *  cette fonction genere les evenements de changement de focus et declanche
 *  les actions qui lui sont attachees.
 *                           NSUBR
 --------------------------------------------------------------------------*/

Int pospointer()/* (xpospointer  win posx posy ) */
{
  Int n;
  Widget dest = get_widget(&n);
  
  if(dest) {
    Int x, y;

    n = 0;
    get_ints(&x, &y, &n);
    if(n != 2){
      a1 = nil;
      derec;
    }
    fairePosPointer(dest, x, y );
    a1 = t;
  } else 
    a1 = nil;
  derec;
}

/*--------------------------------------------------------------------------
 * positionne le pointeur au centre de la widgette passee
 * en parametre.
 --------------------------------------------------------------------------*/
void  posPointerCentreWin(wid)
     Widget wid;
{
  Dimension w, l;
  Arg arg[3];
   
  XtSetArg(arg[0],  XtNwidth, &w );
  XtSetArg(arg[1],  XtNheight, &l );
  XtGetValues(wid, arg, 2);
  fairePosPointer(wid, (Int)w/2, (Int)l/2);
}


