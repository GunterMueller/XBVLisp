/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*      	cursor.c
 * fonctions pour la manipulation du curseur
 * des interfaces vlisp.
 * 	Ernesto Sendoya	 Feb. 91
 */

#include <vlisp.h>
#include <Xbvlisp.h>









/*  fonction lisp :  (getcursorpos {top { win }}) nsubr  )
 * retourne une liste avec la position du curseur
 * par rapport a la fenetre courrante et par rapport
 * a l'ecran. 
 */

Int xGetCursorpos(){/* (getcursorpos {top { win }}) nsubr  */
  Position x_win, y_win, x_root, y_root; 
  
  Widget w, widget = NULL;
  Int bidon;
  Int *at = car(a1);		/*  */
  Boolean done_tmp = False; 
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    getCursorCoordsXbvl(w , &x_win, &y_win, &x_root, &y_root);
    a1 = cons(cranb((Int)x_root), 
	      cons(cranb((Int)y_root), 
		   cons(cranb((Int) x_win),
			cons(cranb((Int) y_win), nil))));
  }  else a1 = nil;
  derec;
}
