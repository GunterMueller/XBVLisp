
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* implementation de l'attente d'un evenement pour une widget
 *              xEvent.c
 *		Damien Ploix
 *		  mars  93		
 */

#include <vlisp.h>
#include <Objets.h>
#include <Xbvlisp.h>

/* Table de correspondance entre les evenements et
   leurs nom pour la conversion depuis Lisp... 
*/

# define NUM_EVENT_TYPE_DEFINED 33	

#define NoEventMask			0L
#define KeyPressMask			(1L<<0)  
#define KeyReleaseMask			(1L<<1)  
#define ButtonPressMask			(1L<<2)  
#define ButtonReleaseMask		(1L<<3)  
#define EnterWindowMask			(1L<<4)  
#define LeaveWindowMask			(1L<<5)  
#define PointerMotionMask		(1L<<6)  
#define PointerMotionHintMask		(1L<<7)  
#define Button1MotionMask		(1L<<8)  
#define Button2MotionMask		(1L<<9)  
#define Button3MotionMask		(1L<<10) 
#define Button4MotionMask		(1L<<11) 
#define Button5MotionMask		(1L<<12) 
#define ButtonMotionMask		(1L<<13) 
#define KeymapStateMask			(1L<<14)
#define ExposureMask			(1L<<15) 
#define VisibilityChangeMask		(1L<<16) 
#define StructureNotifyMask		(1L<<17) 
#define ResizeRedirectMask		(1L<<18) 
#define SubstructureNotifyMask		(1L<<19) 
#define SubstructureRedirectMask	(1L<<20) 
#define FocusChangeMask			(1L<<21) 
#define PropertyChangeMask		(1L<<22) 
#define ColormapChangeMask		(1L<<23) 
#define OwnerGrabButtonMask		(1L<<24) 

static struct {
  char *name;    /* nom Lisp */
  Int type;      /* type X correspondant */
  Int mask;      /* mask X correspondant au type X */
} evntNameType[NUM_EVENT_TYPE_DEFINED] = {
 {"KeyPress", 		KeyPress,         KeyPressMask },     
 {"KeyRelease",   	KeyRelease,       KeyReleaseMask },
 {"ButtonPress",	ButtonPress,      ButtonPressMask  },  
 {"ButtonRelease", 	ButtonRelease,    ButtonReleaseMask },
 {"MotionNotify", 	MotionNotify,     PointerMotionMask }, 
 {"EnterNotify", 	EnterNotify,      EnterWindowMask },
 {"LeaveNotify", 	LeaveNotify,      LeaveWindowMask },  
 {"FocusIn", 		FocusIn,          FocusChangeMask },
 {"FocusOut", 		FocusOut,         FocusChangeMask },     
 {"KeymapNotify", 	KeymapNotify,     KeymapStateMask },
 {"Expose", 		Expose,           ExposureMask },       
 {"GraphicsExpose", 	GraphicsExpose,   ExposureMask },
 {"NoExpose", 		NoExpose,         ExposureMask },     
 {"VisibilityNotify", 	VisibilityNotify, VisibilityChangeMask },
 {"CreateNotify", 	CreateNotify,     SubstructureNotifyMask }, 
 {"DestroyNotify", 	DestroyNotify,    SubstructureNotifyMask | StructureNotifyMask },
 {"UnmapNotify", 	UnmapNotify,      SubstructureNotifyMask | StructureNotifyMask },  
 {"MapNotify", 		MapNotify,        SubstructureNotifyMask | StructureNotifyMask },
 {"MapRequest", 	MapRequest,       SubstructureRedirectMask },    
 {"ReparentNotify", 	ReparentNotify,   SubstructureNotifyMask | StructureNotifyMask }, 
 {"ConfigureNotify", 	ConfigureNotify,  SubstructureNotifyMask | StructureNotifyMask },
 {"ConfigureRequest", 	ConfigureRequest, SubstructureRedirectMask },    
 {"GravityNotify", 	GravityNotify,    SubstructureNotifyMask | StructureNotifyMask }, 
 {"ResizeRequest", 	ResizeRequest,    ResizeRedirectMask },
 {"CirculateNotify", 	CirculateNotify,  SubstructureNotifyMask | StructureNotifyMask }, 
 {"CirculateRequest", 	CirculateRequest, SubstructureRedirectMask },    
 {"PropertyNotify", 	PropertyNotify,   PropertyChangeMask },
 {"SelectionClear", 	SelectionClear,   NoEventMask },
 {"SelectionRequest", 	SelectionRequest, NoEventMask },
 {"SelectionNotify", 	SelectionNotify,  NoEventMask },
 {"ColormapNotify", 	ColormapNotify,   ColormapChangeMask }, 
 {"ClientMessage", 	ClientMessage,    NoEventMask },
 {"MappingNotify", 	MappingNotify,    NoEventMask }};

Int getEventTypeByName(name)
char *name;
{
  Int i;
  for(i = 0; i < NUM_EVENT_TYPE_DEFINED; i++) 
    if(!strcmp(evntNameType[i].name, name)) return evntNameType[i].type;
  return -1;
}

/*---------------------------------------------------------------------*
 *        xCheckEvent sbr2
 *        Verifie si l'evenement donne en argument a eut lieu pour la
 *        widget donnee.
 *---------------------------------------------------------------------*/

bVlispFonc xCheckEvent()
{
  Widget widget ;
  Window win ;
  Display *dpy ;
  CHAR *evntName ;
  Int evntType, i ;
  XEvent report ;

  if(isWidget(a1)) {
    widget = GET_WIDGET_FROM_ATOME(a1) ;
    dpy = XtDisplay(widget);
    win = XtWindow(widget) ;
    a1 = nil ; /* retour par default */
    if(getNameFromArg(&evntName, a2) > 0) {
      for(i = 0; i < NUM_EVENT_TYPE_DEFINED && strcmp(evntName, evntNameType[i].name); i++) ;
      if(i < NUM_EVENT_TYPE_DEFINED) {
	evntType = evntNameType[i].type;
	if(XCheckTypedWindowEvent(dpy, win, evntType, &report)) a1=t;
      }
    }
  }
  else a1 = nil ;
  derec ;
}

/* 
 * xIsRealized subr1
 * verify l'exposition de la widget donnee ... 
 */

bVlispFonc xisrealized()
{
  Widget w;
  
  if(isWidget(a1)) {
    w = GET_WIDGET_FROM_ATOME(a1);
    if(XtIsRealized(w)) a1 = t;
    else a1 = nil;
  }
  derec;
}

	

