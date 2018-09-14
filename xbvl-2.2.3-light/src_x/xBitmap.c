
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* implementation de l'utilisation des bitmaps pour le dessin
 *              xBitmap.c
 *		Damien Ploix
 *		  Avril  93		
 */

#include <vlisp.h>
#include <Objets.h>
#include <Xbvlisp.h>
#include <DrawxbvlP.h>
#include <XbvlispP.h>

#define GET_GC()     XtIsSubclass(w, drawxbvlWidgetClass) ? ((DrawxbvlWidget)w)->drawxbvl.gc \
                           : ( XtIsSubclass(w, xbvlispWidgetClass) ? \
			      ((XbvlispWidget)w)->text.gc :  ((XbvlispWidget)Xbvlisp_courr)->text.gc)

/* subrn : xDisplayBitmap, syntaxe d'appel : (xDisplayBitmap w fname x y mode) */

bVlispFonc xDisplayBitmap()
{
  Widget w;
  Display *dpy;
  GC gc ;
  XGCValues values;
  CHAR *fname;
  int width, height, x_hot, y_hot, x = 0, y = 0;
  Pixmap pixmap;
  Int *at = car(a1);

  if(isWidget(at) && (w = GET_WIDGET_FROM_ATOME(at)) &&
     XtIsRealized(w) && (a1 = cdr(a1))) {
    dpy = XtDisplay(w);
    gc = GET_GC();
    at = car(a1);
    if(isstr(at)) {
      fname = (CHAR *)(at + (2/2));
      if(!XReadBitmapFile(dpy, XtWindow(w), fname, 
			  &width, &height, &pixmap, 
			  &x_hot, &y_hot)) {
	values.function = -1;
	  if((a1 = (Int *) cdr(a1)) && isnum(at = car(a1))) {
	    x = valnb(car(a1));
	    if((a1 = (Int *) cdr(a1)) && isnum(at = car(a1))) {
	      y = valnb(car(a1));
	      if((a1 = (Int *) cdr(a1)) && isnum(at = car(a1))) {
		values.function = valnb(car(a1));
		XChangeGC(dpy, gc, (1L<<0) /* GCFunction */, &values);
	      }
	    }
	  }
	  XCopyPlane(dpy, pixmap, XtWindow(w), gc, 
		     0, 0, /* src x & y */
		     width, height, x, y, 1);
	  if(XtIsSubclass(w, drawxbvlWidgetClass))
	    XCopyPlane(dpy, pixmap, ((DrawxbvlWidget)w)->drawxbvl.pixmap, gc, 
		       0, 0, /* src x & y */
		       width, height, x, y, 1);
	  /* restore context */
	  if(values.function >= 0) {
	    values.function = 3; /* GXcopy */
	    XChangeGC(dpy, gc, (1L<<0) /* GCFunction */, &values);
	  }
	XFreePixmap(dpy, pixmap);
	a1 = cons(cranb(width), cons(cranb(height), cons(cranb(x_hot), cons(cranb(y_hot), nil))));
      }
      else a1 = nil;
    }
    else a1 = nil;
  }
  else a1 = nil;
  derec;
}

/* savegarde un bitmap a partir d'un dessin
 *
 * Lisp : (xSaveBitmap widget filename x y width height)
 * la widget doit etre Drawxbvl
 */

/* 

bVlispFonc xSaveBitmap()
{
  Int *at, x, y, w, h;
  char *fn;
  
  if(isWidget(at = car(a1)) &&
     XtIsSubclass((w = GET_WIDGET_FROM_ATOME(at)), 
		  drawxbvlWidgetClass)) {
    if(isstr(car(a1 = cdr(a1)))) {
      fn = (char *) (car(a1) + (2/2));
      if(isnum(car(a1 = cdr(a1)))) {
	x = valnb(car(a1));
	if(isnum(car(a1 = cdr(a1)))) {
	  y = valnb(car(a1));
	  if(isnum(car(a1 = cdr(a1)))) {
	    w = valnb(car(a1));
	    if(isnum(car(a1 = cdr(a1)))) {
	      h = valnb(car(a1));
	      XWriteBitmapFile(XtDisplay(w), 
			       ((DrawxbvlWidget)w)->drawxbvl.pixmap
			       )
*/	  
	
    
