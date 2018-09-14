
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	        Tortue.c
 *           implementation
 *  D'un Objet est utilise pour dessiner sur
 *      Un Widget  pour dessiner.
 *       Ernesto Sendoya, mai 92
 */


#include <vlisp.h>       
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <TortueP.h>

/**********************************************************************
 *
 *              Full class record constant
 *  Definition des methodes et fonctions publiques de la tortue
 ***********************************************************************/

#define offset(field) XtOffset(TortueObject, tortue.field)

/* Private Data */
static XtResource resources[] = {
  {XtNorientation, XtCOrientation,XtRInt,sizeof(Int), offset(orientation),XtRImmediate, (XtPointer)0},
  {XtNbase,        XtCBase,       XtRInt,sizeof(Int), offset(base),       XtRImmediate, (XtPointer)DEFAULT_BASE},
  {XtNlongeur,     XtCLongeur,    XtRInt,sizeof(Int), offset(longeur),    XtRImmediate, (XtPointer)DEFAULT_LONGEUR},
  {XtNlineWidth,   XtCLineWidth,  XtRInt,sizeof(Int), offset(line_width), XtRImmediate,(caddr_t) 0}, /* un pixel hardware */
  {XtNpencil,      XtCPencil,     XtRBoolean,sizeof(Boolean),offset(pencil),XtRImmediate, (XtPointer)TRUE},
  {XtNforeground,  XtCForeground, XtRPixel,sizeof(Pixel), offset(foreground_pixel), XtRString,(XtPointer)"XtDefaultForeground"},
  {XtNbackground,  XtCBackground, XtRPixel,sizeof(Pixel), offset(background_pixel), XtRString,(XtPointer)"XtDefaultBackground"},
  {XtNfonctionLogique, XtCFonctionLogique, XtRInt, sizeof(Int), offset(fonction_logique), XtRImmediate,(XtPointer) GXcopy},
   {XtNcallback,   XtCCallback, XtRCallback, sizeof(XtPointer),   offset(callbacks), XtRCallback, (XtPointer)NULL},
  {XtNbitmap,      XtCPixmap,   XtRBitmap, sizeof(Pixmap),offset(pixmap), XtRImmediate, (XtPointer)None},
};

#undef offset


static void  Initialize();
static XtSetValuesFunc SetValues();
static void Redisplay();
static XtSetValuesFunc SetValues();
static void  Destroy();

static void releaseTortueGCs();
static void makeTortueGCs();

TortueClassRec tortueClassRec = {
  {
    /* core_class fields */	
    /* superclass	  	*/	(WidgetClass)(&widgetClassRec),
    /* class_name	  	*/	"Tortue",
    /* widget_size	  	*/	sizeof(TortueRec),
    /* class_initialize   	*/	NULL, /* non ClassInitialize */
    /* class_part_initialize	*/	NULL, /* non ClassPartInitialize */
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	(XtInitProc)Initialize,
    /* initialize_hook		*/	NULL,
    /* realize		  	*/	XtInheritRealize,
    /* actions		  	*/	NULL,
    /* num_actions	  	*/	0,
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* compress_motion	  	*/	FALSE,
    /* compress_exposure  	*/	FALSE,
    /* compress_enterleave	*/	FALSE,
    /* visible_interest	  	*/	FALSE,
    /* destroy		  	*/	(XtWidgetProc)Destroy,
    /* resize		  	*/	XtInheritResize,
    /* expose		  	*/	(XtExposeProc)Redisplay,
    /* set_values	  	*/	(XtSetValuesFunc)SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,/* non GetValues */
    /* accept_focus	 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* tm_table		   	*/	XtInheritTranslations,
    /* query_geometry		*/	NULL,
    /* display_accelerator	*/	NULL,
    /* extension		*/	NULL
    },
    { /* methodes de la tortue */
      /* extension		*/	(Int)NULL
    }
};

WidgetClass tortueObjectClass = (WidgetClass)&tortueClassRec;

/* ARGSUSED */
static void  Initialize(request, new,  args, num_args)
     TortueObject request, new;
     ArgList args; 
     Cardinal * num_args ;
{
  makeTortueGCs(new);
  if(new->tortue.pixmap != None){
    Window root;
    int x, y;
    unsigned int width, height, bw, depth;
    if (XGetGeometry(XtDisplay(new), new->tortue.pixmap, &root, &x, &y,
		     &width, &height, &bw, &depth)) {
      new->core.width = height;
      new->core.width = width;
    }
  }
  if(!new->core.width) new->core.width = 1;
  if(!new->core.height) new->core.height = 1;
  if(new->tortue.pencil)
    afficheTortue(new, True);
}


/* ARGSUSED */
static XtSetValuesFunc SetValues(current, request, new)
     TortueObject current, request, new;
{
  if(current->core.x != new->core.y 
     || current->core.y != new->core.y
     || current->tortue.pencil != new->tortue.pencil
     || current->tortue.base != new->tortue.base
     || current->tortue.longeur != new->tortue.longeur
     || current->tortue.orientation != new->tortue.orientation
     ){
    if(current->tortue.pencil)
      afficheTortue(current, False);
    if(current->tortue.foreground_pixel != new->tortue.foreground_pixel
       || current->tortue.background_pixel != new->tortue.background_pixel
       || current->tortue.fonction_logique != new->tortue.fonction_logique
       ){
      releaseTortueGCs(new);
      makeTortueGCs(new);
    }
    if(new->tortue.pencil)
      afficheTortue(new, True);
  }
}
/* 
 * Redisplay fait restitution de l'image sauvegarde dans le pixmap.
 */

static void Redisplay(w, event, region)
     TortueObject w;
     XExposeEvent *event;
     Region region;
{
  if(w->tortue.pixmap == None || event->type != Expose  || !XtIsRealized((Widget)w))
    return ;
  /* la copy est plus rapide avec XCopyArea, et ca marche mieux */
  XCopyPlane(XtDisplay((Widget)w) , w->tortue.pixmap, XtWindow((Widget)w), 
	    w->tortue.gc, event->x, event->y, event->width, event->height, event->x, event->y, 1);
}

#if 0
/* ARGSUSED */
static XtArgsProc GetValues(to, args ,  num_args)
     TortueObject to;
     ArgList args;
     Cardinal *num_args;
{
  register Int i;
  
  for(i = 0; i < *num_args; i++){
    if(strcmp(args[i].name, XtNfreres) == 0){
      DrawxbvlWidget pere = (DrawxbvlWidget)to->core.parent;
      TortueObject l_tortues = pere->drawxbvl.tortues;
      a1 = nil;
      
      for( ; l_tortues; l_tortues = (TortueObject)l_tortues->tortue.next){
	CHAR *name_tortue = l_tortues->core.name;

	if(strcmp(name_tortue, to->core.name)){
	  genat(name_tortue);
	  a1 = cons(a5, a1);
	}
      }
      *(Int *) (args[i].value) = (Int)a1;
    }
  }
}
#endif

/* liberation de la memoire allouee */
static  void Destroy(to)
     TortueObject to;
{
  /* liberer le lien avec le widget */
  
  /* pour debug */
extern Int DEJA;
#ifdef DEBUG
  char buf [100]; 
  sprintf(buf, "%s : detruite\n", to->core.name);
  wst(buf);
#endif
  
  if(to->tortue.pencil)
    afficheTortue(to, False);
  releaseTortueGCs(to);
  /* pour remplacer la callback que ne marchent pas
     sur ces objets, bug de la release 4 ?? */
  genat(to->core.name);
  if( ! DEJA ){
    if(((struct atome *) a5)->cval == a5)
      ((struct atome *) a5)->cval = undef;
    ((struct atome *) a5)->obj = nil;
  }

}

void destroyTortue(tort)
     Widget tort;
{
  Destroy((TortueObject) tort);
}

static void releaseTortueGCs(dw)
     TortueObject dw;
{
  XtReleaseGC((Widget)dw, dw->tortue.gc);
  XtReleaseGC((Widget)dw, dw->tortue.gc_Xor);
}

static void makeTortueGCs(dw)
     TortueObject dw;
{
  XGCValues gcvalues;
  Widget draw_widget = dw->core.parent;
  
  gcvalues.function = dw->tortue.fonction_logique;
  gcvalues.line_width = dw->tortue.line_width;
  gcvalues.background = dw->tortue.background_pixel;
  gcvalues.foreground = dw->tortue.foreground_pixel;

  /* un nouveaux GC, on ne le partage pas avec d'autres Widgets
     puisque on peut le modifier dynamiquement */
  dw->tortue.gc = XtGetGC(draw_widget,
			  GCFunction|GCLineWidth|GCForeground|GCBackground, 
			  &gcvalues);
  /* essayons un Xor different */
  gcvalues.function = GXxor;
  gcvalues.foreground = dw->tortue.foreground_pixel^ dw->tortue.background_pixel;
  dw->tortue.gc_Xor = XtGetGC(draw_widget,
			      GCFunction|GCLineWidth|GCForeground|GCBackground, 
			      &gcvalues);
}

static void afficheTortue(tortuga, mode)
     TortueObject tortuga;
     Int mode;
{
  
  XPoint points[5];
  short x = tortuga->core.x;
  short y = tortuga->core.y;
  short base_media = (short) tortuga->tortue.base/2;
  short longeur = tortuga->tortue.longeur;
  Int angle = tortuga->tortue.orientation;
  Widget interface = tortuga->core.parent;
  Display *display = XtDisplay(interface);
  GC gc = (mode == True) ? tortuga->tortue.gc : tortuga->tortue.gc_Xor;
  /* poInt de depart */
  points[0].x = x;
  points[0].y = y;
  
  points[1].x = x + (short)rint( base_media * cos(a_radianes((90-angle))));
  points[1].y = y + (short)rint( base_media * sin(a_radianes((90-angle))));
  
  points[2].x = x + (short)rint( longeur * cos(a_radianes(angle)));
  points[2].y = y - (short)rint( longeur * sin(a_radianes(angle)));
  
  points[3].x = x - (short)rint( base_media * cos(a_radianes((90-angle))));
  points[3].y = y - (short)rint( base_media * sin(a_radianes((90-angle))));
  
  /* on ferme la tortue */
  points[4].x = points[0].x;
  points[4].y = points[0].y;
  
  XFillPolygon(display, XtWindow(interface),
	       gc, points, 5, Nonconvex, CoordModeOrigin);
  /*  if(XtIsSubclass(interface, drawxbvlWidgetClass))
        XFillPolygon(display, ((DrawxbvlWidget)interface)->drawxbvl.pixmap,
	             gc, points, 5, Nonconvex, CoordModeOrigin);
		 */
}


/* pour les VRAIES Tortues */
Int AvanceTortue()  /* (AVANCE tortue pixels angle) sbr3 */
{
  Widget tortue;
  Int pixels, angle;

  if(isWidget (a1)
     && isint(a2) && isint(a3)){
    tortue = GET_WIDGET_FROM_ATOME(a1);
    pixels = valnb(a2);
    angle = valnb(a3);
    bougeTortue((TortueObject)tortue, pixels, angle);
    a1 = t;
  }
  else a1 = nil;
  derec;
}

void bougeTortue(tort, trajet, direction)
     TortueObject tort;
     Int trajet, direction;
{
  short x, y;
  Widget parent = tort->core.parent;
  
  /* effacer la tortue */
  if(tort->tortue.pencil)
    afficheTortue(tort, False);       /* elle fait un Xor sur elle meme */
  /* la tortue se deplace a la nouvelle possition */
  tort->tortue.orientation = ((tort->tortue.orientation + direction) % 360);
  x = tort->core.x + (Int)rint(trajet * cos(a_radianes(tort->tortue.orientation)));
  
  y = tort->core.y - (short)rint(trajet * sin(a_radianes(tort->tortue.orientation)));
  
  XDrawLine(XtDisplay(parent), XtWindow(parent),
	    tort->tortue.gc , tort->core.x, tort->core.y, x, y);
  if(XtIsSubclass(parent, drawxbvlWidgetClass))
    XDrawLine(XtDisplay(parent), ((DrawxbvlWidget)parent)->drawxbvl.pixmap,
	      tort->tortue.gc , tort->core.x, tort->core.y, x, y);
  
  /* appel du callback */
#ifdef XtSpecificationRelease
  XtCallCallbackList((Widget) tort, tort->tortue.callbacks, NULL);
#else
  XtCallCallbacks((Widget) tort, XtNcallback, NULL);
#endif
  XtMoveWidget((Widget)tort, x, y);  /* change la position et les champs x, y  */
  /* update des coordonnees */
  tort->core.x = x;
  tort->core.y = y;   
  if(tort->tortue.pencil)
    afficheTortue(tort, True); /* la tortue se dessine elle meme */
  return;
}

/* pour la compatibilite avec UNIX Systeme V 3.2 */

#ifdef SYSCINC
Int rint(nbr)
     float nbr;
{
  float piso = floor(nbr);
  return (Int)(nbr - piso) < 0.5 ? piso : ceil(nbr);
}
#endif

     
