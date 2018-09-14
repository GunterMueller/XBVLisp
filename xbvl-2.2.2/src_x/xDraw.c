/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *            xDraw.c
 * fonctions pour la recuperation d`arguments
 * dans une liste.
 * 
 */

#include <vlisp.h>
#include <DrawxbvlP.h>
#include <XbvlispP.h>
#include <math.h>
#include <X11/Xlib.h>

#ifdef  XtSpecificationRelease
#define TEST_WIDGET ! XtIsSubclass(w, tortueObjectClass)
#else
#define TEST_WIDGET 1
#endif
/* calcul du gc a utiliser: 
 * si on a affaire a un widget de dessin ou a un widget d'interaction
 * on utilise les respectifs gc, si non on utilise le gc du widget
 * d'interaction.
 */
#define GET_GC()     XtIsSubclass(w, drawxbvlWidgetClass) ? ((DrawxbvlWidget)w)->drawxbvl.gc \
                           : ( XtIsSubclass(w, xbvlispWidgetClass) ? \
			      ((XbvlispWidget)w)->text.gc :  ((XbvlispWidget)Xbvlisp_courr)->text.gc)

/* interface lisp avec les primitives de dessin X  */
 
Int xdrawarc();       /* (xDrawArcs widget x y width height angle1 angle2 ...) nsubr */
Int xfillarc();       /* (xFillArcs widget x y width height angle1 angle2 ...) nsubr */
Int xDrawLine();      /* (xDrawLines widget x1 y1 x2 y2 ... [mode]) nsubr*/
Int xDrawRay();       /* (xDrawRay widget x y lenght angle ...) nsubr */
Int xDrawPoints();    /* (xDrawPoints widget x y ... [mode] )  nsubr */
Int xdrawrectangle(); /* (xDrawRectangles widget x y width height ...) nsubr */
Int xfillrectangle(); /* (xFillRectangles  widget x y width height ... ) nsubr */
Int xdrawstring();    /* (xDrawString widget x y string nsubr ) nsubr */
Int xgetstringdim();  /* (xGetStringDimension widget string) subr2 */
Int xcleararea();     /* (xClearArea widget x y width heigth ) nsubr  */
Int xFillPolygon();   /* (xFillPolygon widget shape mode x1 y1 x2 y2 x3 y3 ... ) */
Int xsetForeground(); /* (xSetForeground widget color) */
Int xsetBackground(); /* (xSetBackground widget color) */
Int xmodifyGC();      /* (xModifyGC widget attribut valeur) */
Int xgetGCvalue();    /* (xGetGCValue widget attribut) */


#ifdef STDC_HEADERS 
static void dessinPoints ( int (*)(Display * d, Drawable w, GC g, XPoint * a, int np, int mode));
static void dessinRectangles ( int (*)(Display * d, Drawable w, GC g, XRectangle * a, int narec));
static void dessinArcs( int (* )(Display * d, Drawable w, GC g, XArc * a, int narc));
static XPoint * getPointsFromArgs(Int * n);
static XRectangle * getRectanglesFromArgs(Int * n);
static XArc * getArcsFromArgs(Int * n);
#else
static void dessinPoints ();
static void dessinRectangles ();
static void dessinArcs();
static XPoint * getPointsFromArgs();
static XRectangle * getRectanglesFromArgs();
static XArc * getArcsFromArgs();
#endif


Int xdrawstring()    /* (xdrawstring top widget x y string ) */
{
  Widget w;
  CHAR *string;
  Int n, x, y;
  Int *at = car(a1);
  
  if(isWidget(at)                            /* le premier argument est un widget ? */
     && (w = GET_WIDGET_FROM_ATOME(at))      /* le reccuperer */
     &&  TEST_WIDGET                        /* un widget valide ? */
     && (a1 = cdr(a1))                       /* oui, avancer dans la liste d'arguments */
     && get_ints(&x, &y, &n) == 2            /* et reccuperer le reste d'arguments */
     && get_oneName(&string) == True){

    Window win =  XtWindow(w);
    Display *dpy = XtDisplay(w);
    GC gc = GET_GC();
    Int l = strlen(string);

    XDrawString( dpy, win, gc, x, y, string, l);
    if(XtIsSubclass(w, drawxbvlWidgetClass)) /* c'est un widget de dessin  */
      XDrawString( dpy, ((DrawxbvlWidget)w)->drawxbvl.pixmap, gc, x, y, string, l);
    a1 = t;
    XtFree(string);
  } else a1 = nil;
  derec;
}
 

Int xgetstringdim()    /* (xGetStringDimension win string) */
{
  Display *dpy;
  Widget w;
  GC gc;
  XFontStruct *fs;
  XGCValues gc_vals;

  if(isWidget(a1) &&
     (w = GET_WIDGET_FROM_ATOME(a1)) &&
     TEST_WIDGET &&
     (dpy = XtDisplay(w)) &&
     (gc = GET_GC()) && (isustr(a2)) &&
     XGetGCValues(dpy,gc,GCFont,&gc_vals) &&
     (fs = XQueryFont(dpy,gc_vals.font))) {
    XCharStruct chr_str;
    int dir, ascent, descent, wid, hei;
    int n_char = *a2;
    CHAR *string = (CHAR *) (a2+2/2);
    fs = (XFontStruct *)gc_vals.font;
    XTextExtents(fs, string, n_char, &dir, &ascent, &descent, &chr_str);
    wid = chr_str.width;
    hei = ascent + descent;
    a1 = cons(cranb(wid), cranb(hei));
  }
  else a1 = nil;
  derec;
}  

Int xFillPolygon()     /* (xFillPolygon win shape mode x1 y1 x2 y2 x3 y3)   */
{
  Widget w;
  XPoint *points;
  Int n, shape, mode;
  Int *at = car(a1);
  
  if(isWidget(at)                            /* le premier argument est un widget ? */
     && (w = GET_WIDGET_FROM_ATOME(at))      /* le reccuperer */
     && TEST_WIDGET                       /* un widget valide ? */
     && (a1 = cdr(a1))                       /* oui, avancer dans la liste d'arguments */
     && get_ints(&shape, &mode, &n) == 2
     && getPointsFromArgs(&n)){
    
    GC gc = GET_GC();

    if(mode != CoordModePrevious)
      mode = CoordModeOrigin;
    if(shape != Complex && shape != Convex)
      shape = Nonconvex;
    /* 
      si width ou bien heigth == 0, la fenetre sera effacee de la coordonnee 
      initial jusqu'au limite de la fenetre, le contenu du pixmap par contre
      restera intact .
      */
    XFillPolygon( XtDisplay(w), XtWindow(w), gc, points, n, shape, mode);
    if(XtIsSubclass(w, drawxbvlWidgetClass)){ /*  un widget de dessin  */
      XFillPolygon(XtDisplay(w), ((DrawxbvlWidget)w)->drawxbvl.pixmap,
		   gc, points, n, shape, mode); 
    } 
    a1 = cranb(n);
  }
  else a1 = nil;
  derec;
}


Int xcleararea()     /* (xClearArea win x y width heigth )   */
{
  Widget w;
  Int n, x, y, width, height;
  Int *at = car(a1);
  
  if(isWidget(at)                            /* le premier argument est un widget ? */
     && (w = GET_WIDGET_FROM_ATOME(at))      /* le reccuperer */
     &&  TEST_WIDGET                       /* un widget valide ? */
     && (a1 = cdr(a1))                       /* oui, avancer dans la liste d'arguments */
     && get_ints(&x, &y, &n) == 2            /* et reccuperer le reste d'arguments */
     && get_ints(&width, &height, &n) == 2){
    
    Window window =  XtWindow(w);
    
    /* 
      si width ou bien heigth == 0, la fenetre sera effacee de la coordonnee 
      initial jusqu'au limite de la fenetre, le contenu du pixmap par contre
      restera intact .
      */
    XClearArea(display, window,  x, y, width, height, False);
    if(XtIsSubclass(w, drawxbvlWidgetClass)){ /*  un widget de dessin  */
      /* a faire: effacer les tortues qui se trouvent a l'interieur de la region */
      /* recopy de la fenetre au pixmaps */
      XCopyArea(XtDisplay((Widget)w) ,  XtWindow(w), ((DrawxbvlWidget)w)->drawxbvl.pixmap,
		((DrawxbvlWidget)w)->drawxbvl.gc, x, y, width, height, x, y); 
    } 
  }
  else a1 = nil;
  derec;
}

Int xDrawLine()  /* (xDrawLines widget x1 y1 x2 y2 ...) */
{
  dessinPoints(XDrawLines);
  derec;
}

Int xDrawRay() /* (xDrawRays widget x y length angle ...) */
{
  Int *at = car(a1), x, y, l, a;
  double ang, si, co;
  XPoint *points = (XPoint *) XtCalloc((unsigned) 2, sizeof(XPoint));
  Widget w;

  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && TEST_WIDGET) {

    Window win =  XtWindow(w);
    Display *dpy = XtDisplay(w);
    GC gc = GET_GC();

    a1 = cdr(a1);
    while( 1 ) {
      if(init_oneINT(&x) == INTEGER) a1 = cdr(a1);
      else break;
      if(init_oneINT(&y) == INTEGER) a1 = cdr(a1);
      else break;
      if(init_oneINT(&l) == INTEGER) a1 = cdr(a1);
      else break;
      if(init_oneINT(&a) == INTEGER) a1 = cdr(a1);
      else break;
      ang = M_PI * (a / (double) 11520); /* 11520 = 180 * 64 */
      si=sin(ang);
      co=cos(ang);
/*      sincos(ang, &si, &co); */
      points->x = x, points->y = y;
      (points+1)->x = x+co*l, (points+1)->y = y+si*l;

      XDrawLines(dpy, win, gc, points, 2, CoordModeOrigin);
      if(XtIsSubclass(w, drawxbvlWidgetClass)) /*  widget de dessin  */
	XDrawLines(dpy, ((DrawxbvlWidget)w)->drawxbvl.pixmap, gc, points, 2, CoordModeOrigin);
    }
    a1 = t;
  } else a1 = nil;

  XtFree((String)points);
  derec;
}
	
      

Int xDrawPoints()/* (xDrawPoints widget x y... [mode] ) nsubr */
{
  dessinPoints(XDrawPoints);
  derec;
}

Int xdrawrectangle() /* (xDrawRectangles win x y width height ...) nsubr */
{
  dessinRectangles(XDrawRectangles);
  derec;
}

Int xfillrectangle() /* (xFillRectangles widget x y width height...) nsubr */
{
  dessinRectangles(XFillRectangles);
  derec;
}


Int xdrawarc()   /* (xDrawArcs win x y width height angle1 angle2 ...) */
{
  dessinArcs(XDrawArcs);
  derec;
}
Int xfillarc()   /* (xFillArcs win x y width height angle1 angle2 ...) */
{
  dessinArcs(XFillArcs);
  derec;
}

/********************************************************************
 * fonctions auxiliaires pour  optimise le code en generalisant les 
 *    appels au primitives graphiques quand c'est possibles.
 ********************************************************************/
static void dessinPoints(fonc)
     int (*fonc)();
{
  Widget w;
  XPoint * points;
  Int n, mode ;
  Int *at = car(a1);
  
  if(isWidget(at)                            /* le premier argument est un widget ? */
     && (w = GET_WIDGET_FROM_ATOME(at))      /* le reccuperer */
     &&  TEST_WIDGET                       /* un widget valide ? */
     && (a1 = cdr(a1))                       /* oui, avancer dans la liste d'arguments */
     && (points = getPointsFromArgs(&n))){   /* et reccuperer les points */
    
    Window win =  XtWindow(w);
    Display *dpy = XtDisplay(w);
    GC gc = GET_GC();

    init_oneINT(&mode);
    if(mode != CoordModePrevious)
      mode = CoordModeOrigin ;
    
    (*fonc)(dpy, win, gc, points, n, mode);
    if(XtIsSubclass(w, drawxbvlWidgetClass)) /*  widget de dessin  */
      (*fonc)(dpy, ((DrawxbvlWidget)w)->drawxbvl.pixmap, gc, points, n, mode);
    a1 = cranb(n);
    XtFree((String)points);
  } else a1 = nil;
}

static void dessinRectangles(fonc)
     int (*fonc)();
{
  Widget w;
  XRectangle * rectangles;
  Int n, *at = car(a1);
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && TEST_WIDGET
     && (a1 = cdr(a1))                       /* oui, avancer dans la liste d'arguments */
     && (rectangles = getRectanglesFromArgs(&n))){   /* et reccuperer les rectangles */
    
    GC gc = GET_GC();
    
    (*fonc)( XtDisplay(w),  XtWindow(w), gc, rectangles, n);
     if(XtIsSubclass(w, drawxbvlWidgetClass)) /*  un widget de dessin  */
       (*fonc)( XtDisplay(w),  ((DrawxbvlWidget)w)->drawxbvl.pixmap, gc, rectangles, n);
    XtFree((String)rectangles);
    a1 = cranb(n);
  }  else a1 = nil;
}

static void dessinArcs(fonc)
     int (*fonc)();
{
  Widget w;
  XArc * arcs;
  Pixmap pixmap = (Pixmap)NULL;
  Int n ;
  Int *at = car(a1);
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && TEST_WIDGET                        /* un widget valide ? */
     && (a1 = cdr(a1))                       /* oui, avancer dans la liste d'arguments */
     && (arcs = getArcsFromArgs(&n))){       /* et reccuperer les rectangles */
    
    GC gc = GET_GC();
    
    (*fonc)( XtDisplay(w),  XtWindow(w), gc, arcs, n);
    if(XtIsSubclass(w, drawxbvlWidgetClass)) /* un widget de dessin  */
      (*fonc)( XtDisplay(w),  ((DrawxbvlWidget)w)->drawxbvl.pixmap, gc, arcs, n);
    XtFree((String)arcs);
    a1 = cranb(n);
  }  else a1 = nil;
}
/*************************************************************************************
 *  Foncions pour extraire des arguments pour les primitives graphiques de 
 *  registres LISP. On cree dynamiquement des structures de donnees X.
 **************************************************************************************/
/* 
 * Utilisee par xDrawPoints
 * extrait autants des points que possible des arguments d'une nsbr
 * retourne une structure XPoints et le nbr. de points.
 */
static XPoint * getPointsFromArgs(n)
     Int * n;
{
  XPoint * points;
  Int x, y, i = 0;
  
  points = (XPoint *) XtCalloc((unsigned) 1,(unsigned) sizeof(XPoint));
  while( 1 ){
    Int *aux1 = a1;
    
    if(i)
      points = (XPoint *) XtRealloc((char *)points, (unsigned) (i+1) * sizeof(XPoint));
    if(init_oneINT(&x) == INTEGER)
      a1 = cdr(a1);
    else break;
    if(init_oneINT(&y) == INTEGER)
      a1 = cdr(a1);
    else { a1 = aux1;  break; }
    (points + i)->x = (short)x;
    (points + i)->y = (short)y;
    i += 1;
  }
  if( !i && points ) {
    XtFree((String)points);
    points = NULL;
  }
  *n = i;
  return points;
}

/*
 * extrait autants des rectangles que possible des arguments d'une NSBR 
 * retourne une structure XPoints et le nbr. de points.
 */
static XRectangle * getRectanglesFromArgs(n)
     Int * n;
{
  XRectangle * rectangles = NULL;
  Int i = 0, nb, x, y, width, height;
  
  rectangles = (XRectangle *) XtCalloc((unsigned) 1,(unsigned) sizeof(XRectangle));  
  while(get_ints(&x, &y, &nb) == 2 && get_ints(&width, &height, &nb) == 2){
    if(i)
      rectangles=(XRectangle *) XtRealloc((char *)rectangles, (unsigned)  (i+1) * sizeof(XRectangle));
    (rectangles + i)->x = (short)x;
    (rectangles + i)->y = (short)y;
    (rectangles + i)->width = (unsigned short) width;
    (rectangles + i)->height = (unsigned short) height;
    i += 1;
  }
  if(i == 0) {
    XtFree((String)rectangles);
    rectangles  = NULL;
  }
  *n = i;
  return rectangles;
}

static XArc *getArcsFromArgs(n)
     Int *n;
{
  Int i = 0, nb, x, y, width, height, angle1, angle2;
  XArc * arcs;

  arcs = (XArc *) XtCalloc((unsigned) 1,(unsigned) sizeof(XArc));  
  while(get_ints(&x, &y, &nb) == 2 
	&& get_ints(&width, &height, &nb) == 2
	&& get_ints(&angle1, &angle2, &nb) == 2	){
    if(i)
      arcs = (XArc *) XtRealloc((char *)arcs, (unsigned)  (i+1) * sizeof(XArc));
    (arcs + i)->x = (short)x;
    (arcs + i)->y = (short)y;
    (arcs + i)->width = (unsigned short) width;
    (arcs + i)->height = (unsigned short) height;
    (arcs + i)->angle1 = (unsigned short) angle1;
    (arcs + i)->angle2 = (unsigned short) angle2;
    i += 1;
  }
  if(i == 0) {
    XtFree((String)arcs);
    arcs  = NULL;
  }
  *n = i;
  return arcs;
}    

/*
  Changement des couleurs de dessin : xsetForeground & xsetBackground. (xSet....ground widget color) SUBR2
  Les couleurs peuvent etre exprimees en numerique ou caracteres (#xxx)
*/

long get_color_from_a2(dpy)
Display *dpy;
{
  if(isnum(a2)) return (long) valnb(a2);
  if(isstr(a2)) {
    CHAR *s_col = (CHAR *)(a2 + (2/2)); /* devrait etre #nnn (n=0-F) */
    XColor cell, rgb_db;
    Colormap cmap = DefaultColormap(dpy, DefaultScreen(dpy));
    if(XAllocNamedColor(dpy, cmap, s_col, &cell, &rgb_db)) 
      return cell.pixel;
  }
  return -1;
}

Int xsetForeground()
{
  Display *dpy;
  Widget w;
  long color ;

  if(isWidget(a1)) {
    w = GET_WIDGET_FROM_ATOME(a1);
    dpy = XtDisplay(w);
    if((color = get_color_from_a2(dpy)) >= 0) {
      GC gc = GET_GC();
      XSetForeground(dpy, gc, color);
      a1 = t;
    }
    else a1 = nil;
  }
  else a1 = nil;
  derec;
}

Int xsetBackground()
{
  Display *dpy;
  Widget w;
  long color ;

  if(isWidget(a1)) {
    w = GET_WIDGET_FROM_ATOME(a1);
    dpy = XtDisplay(w);
    if((color = get_color_from_a2(dpy)) >= 0) {
      GC gc = GET_GC();
      XSetBackground(dpy, gc, color);
      a1 = t;
    }
    else a1 = nil;
  }
  else a1 = nil;
  derec;
}

/* 
 * Fonction de modification du GC d'une widget....
 */

Int xmodifyGC() /* (xModifyGC widget attribut valeur) */
{
  Widget w;
  if(isWidget(a1) &&
     (w = GET_WIDGET_FROM_ATOME(a1)) &&
     TEST_WIDGET &&
     isstr(a2) &&
     isnum(a3)) {
    
    Display *dpy = XtDisplay(w);
    GC gc = GET_GC();
    XGCValues gcval;
    CHAR *attr = (CHAR *) (a2 + (2/2));
    Int val = valnb(a3);
    long mask = 0;

    if(!strcmp(attr, "mode")) 
      gcval.function = val, mask = (1L << 0); /* Fonction Logique */
    else if(!strcmp(attr, "line_width"))
      gcval.line_width = val, mask = (1L << 4);
    else if(!strcmp(attr, "line_style"))
      gcval.line_style = val, mask = (1L << 5);
    else if(!strcmp(attr, "fill_style"))
      gcval.fill_style = val, mask = (1L << 8);

    a1 = nil;
    if(mask) 
      XChangeGC(dpy, gc, mask, &gcval), a1 = t;
  }
  derec;
}

/* 
 * Fonction de recuperation des valeurs du GC d'une widget....
 */

Int xgetGCvalue() /* (xModifyGC widget attribut valeur) */
{
  Widget w;
  if(isWidget(a1) &&
     (w = GET_WIDGET_FROM_ATOME(a1)) &&
     TEST_WIDGET &&
     isstr(a2)) {
    
    Display *dpy = XtDisplay(w);
    GC gc = GET_GC();
    XGCValues gcval;
    CHAR *attr = (CHAR *) (a2 + (2/2));
    XGetGCValues(dpy,gc,GCFunction | GCLineWidth | GCLineStyle | GCFillStyle, &gcval);

    if(!strcmp(attr, "mode")) 
      a1 = cranb(gcval.function);
    else if(!strcmp(attr, "line_width"))
      a1 = cranb(gcval.line_width);
    else if(!strcmp(attr, "line_style"))
      a1 = cranb(gcval.line_style);
    else if(!strcmp(attr, "fill_style"))
      a1 = cranb(gcval.fill_style);
    else a1 = nil;
  }
  derec;
}
