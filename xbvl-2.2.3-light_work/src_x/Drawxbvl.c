/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	   Drawxbvl.c
 *    Declarations Privees pour le
 *  Widget fueille de dessin de xbVLISP.
 *    Les Widgets appartenant 
 *    Ernesto Sendoya, mai 92
 */

#include <vlisp.h>
#include <X11/StringDefs.h>
#include <DrawxbvlP.h>



/* Private Data */
static XtResource resources[] = {
#define offset(field) XtOffset(DrawxbvlWidget, drawxbvl.field)
  {XtNforeground, XtCForeground, XtRPixel,sizeof(Pixel),
     offset(foreground_pixel), XtRString,(caddr_t)"XtDefaultForeground"},
  {XtNfonctionLogique, XtCFonctionLogique, XtRInt,sizeof(Int),
     offset(fonction_logique), XtRImmediate,(caddr_t) GXcopy},
  {XtNlineWidth, XtCLineWidth, XtRInt,sizeof(Int),
     offset(line_width), XtRImmediate,(caddr_t) 0}, /* un pixel hardware */
  { XtNfont, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
      offset(font),  XtRString, "Fixed"},
#undef offset
};


static void DrawxbvlAction(/* Widget, XEvent*, String*, Cardinal* */);

static char translations[] =
  "<Key>:drawxbvl()\n\
";

static XtActionsRec actions[] =
{
  /* {name, procedure}, */
  {"drawxbvl",	DrawxbvlAction},
};

static void Initialize();
static XtGeometryResult GeometryManager();
static void Redisplay();
static Boolean  SetValues();
/* static XtArgsProc GetValues(); */
static void  Destroy();
void DrawxbvlResize();

DrawxbvlClassRec drawxbvlClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass)&compositeClassRec, /*(WidgetClass) &widgetClassRec,*/
    /* class_name		*/	"Drawxbvl",
    /* widget_size		*/	sizeof(DrawxbvlRec),
    /* class_initiaize		*/	NULL,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	(XtInitProc)Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	actions,
    /* num_actions		*/	XtNumber(actions),
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	(XtWidgetProc)Destroy,
    /* resize			*/	(XtWidgetProc)DrawxbvlResize,
    /* expose			*/	(XtExposeProc)Redisplay ,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL, /* (XtArgsProc)GetValues, */
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	translations,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
    },{
/* composite_class fields */
    /* geometry_manager   */    GeometryManager,
    /* change_managed     */    NULL,
    /* insert_child	  */	XtInheritInsertChild,
    /* delete_child	  */	XtInheritDeleteChild,
				NULL,
  },{
    /* Drawxbvl fields */
    /* empty			*/	0
  }
};

WidgetClass drawxbvlWidgetClass = (WidgetClass)&drawxbvlClassRec;

     
static void init_DrawGC(dw)
     DrawxbvlWidget dw;
{
  XGCValues gcvalues;

  gcvalues.function = dw->drawxbvl.fonction_logique;
  gcvalues.line_width = dw->drawxbvl.line_width;
  gcvalues.background = dw->core.background_pixel;
  gcvalues.foreground = dw->drawxbvl.foreground_pixel;
  gcvalues.font = dw->drawxbvl.font->fid;

  /* un nouveaux GC, on ne le partage pas avec d'autres Widgets
     puisque on peut le modifier dynamiquement */
  /* et non, ca ne marche pas
  dw->drawxbvl.gc = XCreateGC(XtDisplay((Widget)dw), XtWindow((Widget)dw),
			      GCFunction|GCLineWidth|GCForeground|GCBackground|GCFont, 
			      &gcvalues);
  */
  dw->drawxbvl.gc = XtGetGC((Widget)dw,
			    GCFunction|GCLineWidth|GCForeground|GCBackground|GCFont, 
			    &gcvalues);
}

static void init_DrawPixmap(w)
     Widget w;
{
  GC invert_gc;
  XGCValues gcvalues;
  Display *d = XtDisplay(w);
  DrawxbvlWidget dw = (DrawxbvlWidget)w;

  dw->drawxbvl.pixmap = XCreatePixmap(d, RootWindowOfScreen(XtScreen(w)),
				      dw->core.width, dw->core.height,
				      DisplayPlanes(d,  DefaultScreen(d)));
  dw->drawxbvl.width = dw->core.width;
  dw->drawxbvl.height = dw->core.height;
  
  
  gcvalues.foreground =  dw->core.background_pixel;
  gcvalues.background =  dw->drawxbvl.foreground_pixel;
  
  invert_gc = XtGetGC(w,(GCForeground|GCBackground), &gcvalues);
  /* netoyer le pixmap */
  XFillRectangle(d, dw->drawxbvl.pixmap, invert_gc, 
		 0,0 , dw->core.width, dw->core.height);
  XtReleaseGC(w, invert_gc);
}

/* ARGSUSED */
#ifdef XtSpecificationRelease
static void  Initialize(request, new,  args, num_args)
     DrawxbvlWidget request, new;
ArgList args; 
Cardinal * num_args ;
#else
static void  Initialize(request, new)
     DrawxbvlWidget request, new;
#endif
{
  /* la taille plus grande que 0,0 */
  if(new->core.width == 0) new->core.width = 100;
  if(new->core.height == 0) new->core.height = 100;
  new->drawxbvl.fonction_logique = request->drawxbvl.fonction_logique;
  new->drawxbvl.line_width = request->drawxbvl.line_width;
  new->drawxbvl.tortues = NULL;
  init_DrawGC(new);
  init_DrawPixmap((Widget)new);
}

/*
  Une gestion de la geometry tres simple:
  Drawxbvl admet tout changement de geometry des enfants,
  */
static XtGeometryResult GeometryManager(w, desired, allowed)
     Widget w;
     XtWidgetGeometry * desired, * allowed;
{
#define Wants(flag)    (desired->request_mode & flag)
  Boolean change_geom = False;
  Position x, y;
  Dimension width, height, border_width;
  
  if(Wants(CWX)){
    if(desired->x != w->core.x)
      change_geom = True;
    x = desired->x;
  } else x = w->core.x;
  if(Wants(CWX)){
    if(desired->y != w->core.y)
      change_geom = True;
    y = desired->y;
  }  else y = w->core.y;
  if(Wants(CWWidth)){
    if(desired->width != w->core.width)
      change_geom = True;
    width = desired->width;
  } else width = w->core.width;
  if(Wants(CWHeight)){
    if(desired->height != w->core.height)
      change_geom = True;
    height = desired->height;
  }else height = w->core.height;
  if(Wants(CWBorderWidth)){
    if(desired->border_width != w->core.border_width)
      change_geom = True;
    border_width = desired->border_width;
  }else border_width = w->core.border_width;
  if(change_geom == True){
    XtConfigureWidget(w, x, y, width, height, border_width);
    w->core.x = x; w->core.y = y;
    w->core.width = width; w->core.height = height; 
    w->core.border_width = border_width;
  }
#undef Wants
  return XtGeometryDone;
}

/* 
 * Redisplay fait restitution de l'image sauvegarde dans le pixmap.
 */
static void Redisplay(w, event, region)
     DrawxbvlWidget w;
     XExposeEvent *event;
     Region region;
{
  register Int x, y;

  if(event->type != Expose || !XtIsRealized((Widget) w))
    return ;

  x = event->x;
  y = event->y;

  /* la copy est plus rapide avec XCopyArea, et ca marche mieux */
  XCopyArea(XtDisplay((Widget)w) , w->drawxbvl.pixmap, XtWindow(w), 
	    w->drawxbvl.gc, x, y, event->width, event->height, x, y);
}
/*
 * Resize : si besoin est ont change la taille du pixmap sous-jacent....
 */
void DrawxbvlResize(w)
     DrawxbvlWidget w;
{
  GC invert_gc;
  XGCValues gcvalues;
  Pixmap new;
  Display *d = XtDisplay(w);

  if(w->core.width <= w->drawxbvl.width && w->core.height <= w->drawxbvl.height) return;

  new = XCreatePixmap(d, RootWindowOfScreen(XtScreen(w)),
		      w->core.width, w->core.height,
		      DisplayPlanes(d,  DefaultScreen(d)));

  gcvalues.foreground =  w->core.background_pixel;
  gcvalues.background =  w->drawxbvl.foreground_pixel;
  
  invert_gc = XtGetGC((Widget)w,(GCForeground|GCBackground), &gcvalues);
  /* netoyer le pixmap */
  XFillRectangle(d, new, invert_gc, 
		 0,0 , w->core.width, w->core.width);
  XtReleaseGC((Widget)w, invert_gc);
  XCopyArea(d, w->drawxbvl.pixmap, new, w->drawxbvl.gc, 0, 0,
	    w->core.width, w->core.height, 0, 0);
  XFreePixmap(d, w->drawxbvl.pixmap);
  w->drawxbvl.pixmap = new;
  w->drawxbvl.width = w->core.width;
  w->drawxbvl.height = w->core.height;
}
/*
 * on modifie le GC si une des ressources qui lui est attachee change.
 */
/* ARGSUSED */
static Boolean SetValues(current, request, new)
     DrawxbvlWidget current, request, new;
{
  Boolean do_redisplay = False;
  XGCValues gcvalues;
  unsigned long mask = 0;
  
  
  if(new->drawxbvl.line_width != current->drawxbvl.line_width){
    gcvalues.line_width = new->drawxbvl.line_width;
    mask |= GCLineWidth;
  }
  if(new->drawxbvl.fonction_logique != current->drawxbvl.fonction_logique){
    gcvalues.function = new->drawxbvl.fonction_logique;
    mask |= GCFunction;
  }
  if(new->drawxbvl.foreground_pixel != current->drawxbvl.foreground_pixel){
    gcvalues.foreground = new->drawxbvl.foreground_pixel;
    mask |= GCForeground;
  }
  if(new->core.background_pixel != current->core.background_pixel){
    /* ici il faudrait modifier aussi le Pixmap */
    gcvalues.background = new->core.background_pixel;
    mask |= GCBackground;
  }
  if(new->drawxbvl.font != current->drawxbvl.font){
    gcvalues.font = new->drawxbvl.font->fid;
    mask |= GCFont;
  }
  if(mask){
    XtReleaseGC((Widget)new, new->drawxbvl.gc);
    init_DrawGC(new);
  }
  return do_redisplay;
}

#if 0
/* ARGSUSED */
static XtArgsProc GetValues(dw, args ,  num_args)
     DrawxbvlWidget dw;
     ArgList args;
     Cardinal *num_args;
{
#ifdef XtSpecificationRelease
  register Int i;
  
  for(i = 0; i < *num_args; i++){
    if(strcmp(args[i].name, XtNtortues) == 0){
      TortueObject l_tortues = dw->drawxbvl.tortues;
      a1 = nil;
      for( ; l_tortues; l_tortues = (TortueObject)l_tortues->tortue.next){
	genat(l_tortues->core.name);
	a1 = cons(a5, a1);
      }
      *(Int *) (args[i].value) = (Int)a1;
    }
  }
#endif
}
#endif

/* liberation de la memoire allouee */
static void Destroy(w)
     DrawxbvlWidget w;
{
  
  XFreePixmap(XtDisplay((Widget)w), w->drawxbvl.pixmap);
  XtReleaseGC((Widget)w, w->drawxbvl.gc);
}


#if 0
    /* c'est Plutot pour resize !!!!! */
    Pixmap pixmap = new->drawxbvl.pixmap;
    init_Pixmap((Widget)new);
    XCopyArea(XtDisplay((Widget)new) , pixmap, new->drawxbvl.pixmap,
	      new->drawxbvl.gc, 0, 0, new->core.width, new->core.height, x, y);
    XtFree(XtDisplay((Widget)new, Pixmap);
#endif

/* ARGUSED */
static void DrawxbvlAction(w, e, params, nparams)
     Widget w;
     XEvent * e;
     String * params;
     Cardinal * nparams;
{
  wst("et hop\n");
}

/* 
 * affiche bien centre en haut du widget de dessin,
 * utilisee par (menu widget chaine).
 */
void afficheMenuToDraw(widget, menu)
     Widget widget;
     char *menu;
{
  DrawxbvlWidget w = (DrawxbvlWidget) widget;
  XFontStruct  *font = w->drawxbvl.font;
  Int y, x, l =  strlen(menu);
  Int text_width = XTextWidth(font, menu, l);
  Int width = w->core.width;

  /* caluler d'abord la position de la chaine dans le widget */
  y = (Int)1.5 * FONTHEIGHT(font);
  x = (text_width >= width) ? 0 : (width - text_width)/2;
  /* realiser l'affichage */
  XDrawString(XtDisplay(widget), XtWindow(widget), w->drawxbvl.gc, x, y, menu, l);
  XDrawString(XtDisplay(widget), w->drawxbvl.pixmap, w->drawxbvl.gc, x, y, menu, l);
}





