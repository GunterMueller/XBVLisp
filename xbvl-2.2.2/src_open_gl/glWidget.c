/*
 * glWidget.c
 * definition de la widget d'interaction entre xbvl et GL
 * ce fichier contient les callbacks adaptes a xbvl pour
 * la widget GLXDRAW.
 *
 * Damien Ploix, Decembre 93
 * Olivier Blanc et Damien Ploix, passage pour Open_GL Mars-Avril 95
 */
#if (OPEN_GL | MESA_GL)

#include <math.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#if MESA_GL
#include <GL/xmesa.h>
#endif
#include <GL/GLwDrawA.h>

/* liens avec Xbvl */
#include "vlisp.h"
/* définitions locales pour l'interface */
#include "glXbvl.h"

GLObject *wobj = NULL;               /* wobj désigne la fenêtre active */

/* *********************** */
/* Construction de l'image */
/* *********************** */

/* Construction de l'objet graphique correspondant aux transformations en cours ... */
/* Pout la matrice de projection */
static void 
build_projection(is_picking, mouse_x, mouse_y)
char is_picking;
Int  mouse_x, mouse_y;
{
  if(!is_picking){
    glViewport(0,0,wobj->width,wobj->height);
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity() ;

  if(is_picking) {
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    gluPickMatrix((double) mouse_x, (double) (vp[3]-mouse_y), 5.0, 5.0, vp);
  }

  if(wobj->mode & GL_MODE_ORTHO) 
    glOrtho(wobj->p_data[0], wobj->p_data[1], wobj->p_data[2], 
	    wobj->p_data[3], wobj->p_data[4], wobj->p_data[5]);
  else if(wobj->mode & GL_MODE_ORTHO2) 
    gluOrtho2D(wobj->p_data[0], wobj->p_data[1], wobj->p_data[2], 
	      wobj->p_data[3]);
  else if(wobj->mode & GL_MODE_PERSP) 
    gluPerspective(wobj->p_data[0] / 10.0,wobj->p_data[1], 
		   wobj->p_data[2], wobj->p_data[3]);
  else if(wobj->mode & GL_MODE_FURSTRUM)
    glFrustum(wobj->p_data[0], wobj->p_data[1], wobj->p_data[2], 
	      wobj->p_data[3], wobj->p_data[4], wobj->p_data[5]);

  if(!is_picking) glGetDoublev(GL_PROJECTION_MATRIX, wobj->Proj_Mat);
}
/* et pour la matrice du model (de transformations de l'image) */
static void
build_modelview()
{
  Int i;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  /* on met en premier lieu en place le lookat */
  if(wobj->mode & GL_MODE_LOOKAT) 
    gluLookAt( wobj->p_eye[0],  wobj->p_eye[1],
	       wobj->p_eye[2],  wobj->p_eye[3],
	       wobj->p_eye[4],  wobj->p_eye[5],
	       wobj->p_eye[6],  wobj->p_eye[7],
	       wobj->p_eye[8]);
  /* puis on effectu les transformations d'image */
  for(i = 0; i < 3; i++) 
    if(wobj->t_rotate[wobj->t_rot_order[i]])
      glRotated(wobj->t_rotate[wobj->t_rot_order[i]] / 10., 
		(wobj->t_rot_order[i] == 0) ? 1.0 : 0.0,
		(wobj->t_rot_order[i] == 1) ? 1.0 : 0.0,
		(wobj->t_rot_order[i] == 2) ? 1.0 : 0.0);
  if(wobj->t_scale[0] != 1 || wobj->t_scale[1] != 1 || wobj->t_scale[2] != 1 || wobj->t_scale[3] != 1)
    glScaled(wobj->t_scale[0] / wobj->t_scale[3], 
	     wobj->t_scale[1] / wobj->t_scale[3], 
	     wobj->t_scale[2] / wobj->t_scale[3]);
  if(wobj->t_translate[0] || wobj->t_translate[1] || wobj->t_translate[2]) 
    glTranslated(wobj->t_translate[0], wobj->t_translate[1], wobj->t_translate[2]);
  glGetDoublev(GL_MODELVIEW_MATRIX, wobj->Model_Mat);
}

/* recuperation des transformations de l'image */

Int get_image_transformation() /* (GLgetImageMatrix atom) */
{
  GLdouble *Mat;
  struct atome *at = (struct atome *) a1;
  Int i;

  if(!isatom(a1) || a1 == nil || !wobj) derec;
  if(at->obj != nil) Mat = (GLdouble *) at->obj;
  else at->obj = (Int *) (Mat = (GLdouble *) XtMalloc(sizeof(double) * 16));
  memcpy(wobj->Model_Mat, Mat, 16 * sizeof(double));
  derec;
}

void 
redessine()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(wobj->Proj_Mat);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd(wobj->Model_Mat);
  glCallList(wobj->root_list);
  glFlush();
  glXWaitGL();
  GLwDrawingAreaSwapBuffers((Widget)wobj->w);
}

/* ******************************** */
/* gestion des widgets et objets GL */
/* ******************************** */

/* récupération d'une fenêtre GL à partir de son widget */
static GLObjectPtr 
getWObject(w)
Widget w;
{
  GLObjectPtr ret;
  for(ret = bgl; ret < hgl; ) {
	if((Widget)ret->w == w) return ret ;
	ret++;
      }
  return NULL;
}

                /* ****************************** */
                /* callbacks liés aux fenêtres GL */
                /* ****************************** */

/* *************************************************** */
/* appelle a l'initialisation (au moment du XtRealize) */
/* *************************************************** */
static void  
gl_ginit_callback(w, client_data, call_data)
Widget w;
caddr_t client_data;
GLwDrawingAreaCallbackStruct *call_data ;
{
  GLObjectPtr list_shared = NULL;
  XVisualInfo *vi;
  Window windows[2];
  Display *gl_dpy; 
  Arg args[1];
  Int i;

  /* creation d'un nouvel objet pour la fenetre */
  wobj = (GLObjectPtr) client_data;
  gl_dpy = (Display *) wobj->root_list;

  /* partage des listes ? */
  if(wobj->w != nil) list_shared = getWObject(wobj->w);

  wobj->w = (Int *) w;
  wobj->mode = GL_MODE_ORTHO;    /* projection orthogonale */
  /* pas de callback... */
  wobj->callback_number = 0;

  /* Taille initiale */
  wobj->width = call_data->width;
  wobj->height = call_data->height;

  /* donnees de la projection Orthogonale par defaut */
  wobj->p_data[0] = -0.5;  wobj->p_data[1] = (double)call_data->width+0.5;
  wobj->p_data[2] = -0.5;  wobj->p_data[3] = (double)call_data->height+0.5;
  /* Z min et max */
  wobj->p_data[4] = (double)(-1 * (1 << 16))-0.5;
  wobj->p_data[5] = (double)(1 << 16)+0.5;

  /* données des transformation par défaut */
  wobj->t_translate[0] = wobj->t_translate[1] = wobj->t_translate[2] = 0;
  wobj->t_rotate[0] = wobj->t_rotate[1] = wobj->t_rotate[2] = 0;
  wobj->t_rot_order[0] = 0, wobj->t_rot_order[1] = 1; wobj->t_rot_order[2] = 2;
  wobj->t_scale[0] = wobj->t_scale[1] = wobj->t_scale[2] = wobj->t_scale[3] = 1;

  XtVaGetValues(w,GLwNvisualInfo, &vi, NULL);

  wobj->glx_context = glXCreateContext(gl_dpy, vi, 
				       (list_shared) ? list_shared->glx_context : NULL, 
#if OPEN_GL
				       /* le rendu est direct en cas d'utilisation d'OPEN_GL */
				       GL_TRUE
#else
                                       /* Et Via X pour Mesa */
				       GL_FALSE
#endif
				       );

  GLwDrawingAreaMakeCurrent((Widget)wobj->w, wobj->glx_context);

  build_projection(0,0,0);
  build_modelview();

  wobj->root_list = 0; /* pas de liste a priori (GLrootlist n) */

  glDrawBuffer(GL_BACK);  
  glShadeModel(GL_FLAT) ;
}

/* ************************************************ */
/* Expose : appelle a chaque redessin de la fenetre */
/* ************************************************ */
static void  
gl_expose_callback(w, client_data,call_data)
Widget w;
caddr_t client_data;
GLwDrawingAreaCallbackStruct *call_data ;
{
  GLObjectPtr old_w = NULL;
  if(wobj != (GLObjectPtr) client_data) {
    old_w = wobj;
    wobj = (GLObjectPtr) client_data;
    GLwDrawingAreaMakeCurrent((Widget)wobj->w, wobj->glx_context);
  }
  redessine();
  if(old_w) {
    wobj = old_w;
    GLwDrawingAreaMakeCurrent((Widget)wobj->w, wobj->glx_context);
  }
}

/* *************************************************** */
/* Resize : appelle a chaque modification de la taille */
/* *************************************************** */
static void 
gl_resize_callback(w, client_data,call_data)
Widget w;
caddr_t client_data;
GLwDrawingAreaCallbackStruct *call_data ;
{
  GLObjectPtr gl_w = wobj;

  wobj = (GLObjectPtr) client_data;

  wobj->width = call_data->width;
  wobj->height = call_data->height;

  if(wobj->mode & GL_MODE_PERSP)  
    wobj->p_data[1] = (double) call_data->width / (double) call_data->height;

  else if(wobj->mode & GL_MODE_ORTHO) {
    wobj->p_data[1] = (double)call_data->width+0.5;
    wobj->p_data[3] = (double)call_data->height+0.5;
    wobj->width = call_data->width;
    wobj->height = call_data->height;
  }
  if(wobj == gl_w) build_projection(0,0,0);
  else wobj = gl_w;
}

/* ********************************** */
/* Destroy : appelle a la destruction */
/* ********************************** */
static void 
gl_destroy_callback(w, client_data,call_data)
Widget w;
caddr_t client_data;
GLwDrawingAreaCallbackStruct *call_data ;
{
  freegl((GLObjectPtr) client_data);
}

/* ******************** */
/* Callback des entrees */
/* ******************** */
#define CALL_BUFFER_SIZE   1024

#define incr_ptr(l)                { Int add_l = l; \
				     if(ptr+add_l >= end_callBuffer) {  \
					Int pos = (Int) (ptr - callBuffer); len += CALL_BUFFER_SIZE; \
					callBuffer = XtRealloc(callBuffer, (unsigned) len * sizeof(unsigned int)); \
					end_callBuffer = callBuffer+len; ptr = callBuffer+pos; } \
				     ptr += add_l; }
								       

static void 
gl_input_callback(w, client_data, call_data)
Widget w;
caddr_t client_data;
GLwDrawingAreaCallbackStruct *call_data ;
{
  char *callBuffer = XtMalloc(CALL_BUFFER_SIZE);
  char *ptr, *xbvl_mess, *end_callBuffer;
  register Int i = 0, len = CALL_BUFFER_SIZE;
  XEvent evnt;
  GLObjectPtr oldw = NULL;
  if(wobj != (GLObjectPtr) client_data) {
    oldw = wobj;
    wobj = (GLObjectPtr) client_data;
    GLwDrawingAreaMakeCurrent((Widget)wobj->w, wobj->glx_context);
  }
  
  /* pas de callbacks pour la fenetre */
  if(!wobj->callback_number) return;

  for(i = 0; i < wobj->callback_number; i++) 
    if(wobj->callback_event[i] == call_data->event->type) break;

  if(i >= wobj->callback_number) return;

  fixeBind(w);
  xbvl_mess = wobj->callback_expression[i];
  
  /* expession, le cas echeant, de l'expression avant de la passee a XBVL */
  /*
   * on insere les arguments remplacant le macros suivants :
   * $X par la positon en x
   * $Y par la positon en y
   * $B par le numero du bouton de la souris
   * $G par le(s) couple(s) (objet numero) correspondant a l'objet selectionne
   * $C par le caractere saisi au clavier
   & $W par l'atome correspondant a la fenetre dans laquelle la saisie a eut lieu
   */
  
  memset(callBuffer, '\0', CALL_BUFFER_SIZE); /* netoyer le buffer */
  ptr = callBuffer;
  end_callBuffer = ptr + CALL_BUFFER_SIZE;
  while( *xbvl_mess ) {
    if(*xbvl_mess == '$' )
      switch(*(xbvl_mess + 1)) {
        case 'W' :
  	  sprintf(ptr, "%s", (char *) (((struct atome *) wobj->atome)->pn + (2/2)));
	  incr_ptr(strlen(ptr));
	  xbvl_mess += 2;
	  break;
	case 'B' : 
	  sprintf(ptr, "%d", (call_data->event->xmotion.state >> 8));
	  incr_ptr(strlen(ptr));
	  xbvl_mess += 2;
	  break;
	case 'G' :
	  if(call_data->event->type == ButtonRelease || 
	     call_data->event->type == ButtonPress) {

	    /* envoi de la recherche de l'objet ... */
	    unsigned int pickbuff[BUFSIZ], *ui_ptr, numpicked, j, buf_pos;
	    int viewport[4];

	    glGetIntegerv(GL_VIEWPORT, viewport);

	    glSelectBuffer(BUFSIZ, pickbuff);
	    glRenderMode(GL_SELECT);
	    glInitNames();
	    glPushName(-1);

	    build_projection(1, call_data->event->xbutton.x, call_data->event->xbutton.y);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadMatrixd(wobj->Model_Mat);

	    glCallList(wobj->root_list);

	    numpicked =glRenderMode(GL_RENDER);

	    /* recherche de(s) objet(s) dans la memoire... */
	    *ptr = '\''; incr_ptr(1);
	    *ptr = '(';  incr_ptr(1);

	    ui_ptr = &pickbuff[0];
	    for(i = 0; i < numpicked; i++) {
	      buf_pos = *ui_ptr;
	      ui_ptr += 3;
	      for(j = 0; j < buf_pos; j++) {
		sprintf(ptr, "%u ", *ui_ptr++);
		incr_ptr(strlen(ptr));
	      }
	    }
	    *ptr = ')';  incr_ptr(1);
	    xbvl_mess += 2;
	  }
	  else { *ptr = *xbvl_mess++; incr_ptr(1); }
	  break;
	case 'C' :
	  if(call_data->event->type == KeyRelease) {
	    char buffer[BUFSIZ];
	    long other;
	    memset(buffer, '\0', BUFSIZ);
	    i = XLookupString((XKeyEvent *)call_data->event, 
			      (char *)buffer, BUFSIZ,
				(KeySym *) &other, NULL);
	    if(i) sprintf(ptr, "%s", buffer);
	    incr_ptr(strlen(ptr));	    xbvl_mess += 2;
	  }
	  else { *ptr = *xbvl_mess++; incr_ptr(1); }
	  break;
	case 'X' :
	  sprintf(ptr, "%d", call_data->event->xbutton.x);
	  incr_ptr(strlen(ptr));	  xbvl_mess += 2;
	  break;
	case 'Y' :
	  sprintf(ptr, "%d", call_data->event->xbutton.y);
	  incr_ptr(strlen(ptr));	  xbvl_mess += 2;
	  break;
	default: *ptr = *xbvl_mess++; incr_ptr(1); break;
	}
    else { *ptr = *xbvl_mess++; incr_ptr(1); }
  }
  communiqExprToVlisp(callBuffer);
  if(oldw) {
    wobj = oldw;
    GLwDrawingAreaMakeCurrent((Widget)wobj->w, wobj->glx_context);
  }
}

                                                /* *********************** */
                                                /* Fonctions d'interaction */
                                                /* *********************** */

/* Creation d'une fenetre */
Int gl_winopen() /* (GLwinopen nom widgetPere [listes partagees] <resource_list>) -> widget  NSUBR */
{
  Int n, type;
  CHAR *nom;
  Widget newWidget = NULL, w_pere;
  GLwDrawingAreaWidgetClass widget_class ;
  ArgList argg;
  struct atome *rep_atome;
  static Display *gl_dpy; 
  XtPointer c_data;


  /* setup par defaut pour les nouvelles fenetres */
  a2 = car(a1);
  if(!((type = getNameFromArg((CHAR **)&nom, a2)) == STRING || 
       type == NAME)) {a1 = nil; derec;}
  
  widget_class = (GLwDrawingAreaWidgetClass)glwDrawingAreaWidgetClass ;
  a2 = car((a1 = cdr(a1)));
  if(isWidget(a2)) {
    int erB, evB;
    w_pere = GET_WIDGET_FROM_ATOME(a2);
    gl_dpy = XtDisplay(w_pere);
    if(glXQueryExtension(gl_dpy, &erB, &evB) == False) {
      wobj = NULL;
      a1 = nil;
      derec;
    }
  }
  else {
    a1 = nil;
    derec;
  }

  /* il ne nous reste que faire la creation du nouveaux widgette */
  a1 = cdr(a1); 
  /* en premier lieu de verifier si la nouvelle widget OpenGL partage ses 
     listes avec une autre */
  if(isWidget(car(a1))) {
  /* verification d'un partage de listes entre deux fenetres */
    a2 = (Int *)GET_WIDGET_FROM_ATOME(car(a1));
    a1 = cdr(a1);
  }
  else a2 = nil;
  /* en reccuperant des eventuels ressources */
  getArgList(XtClass(w_pere), (WidgetClass) widget_class, (ArgList *) &argg, (Int *) &n); 
  argg = (ArgList) XtRealloc((String)argg, (unsigned) (n+3) * (unsigned) sizeof(Arg));

  XtSetArg(argg[n], GLwNdoublebuffer, True); n++; 
  XtSetArg(argg[n], GLwNrgba, True); n++;
  XtSetArg(argg[n], GLwNdepthSize, 1); n++;

  newWidget = XbvlCreateManagedWidget(nom, (WidgetClass) widget_class, w_pere, argg, n, &rep_atome);

  if(!newWidget){ 
    printf("ERRor, Impossible de creer le Widget <%s>\n", nom);
    a1 = nil;
    derec;
  }
  c_data = (XtPointer) cregl();  /* les callbacks recevrons l'objet GLObject correspondant a la fenetre */
  XtAddCallback(newWidget, GLwNginitCallback,  gl_ginit_callback,c_data);
  XtAddCallback(newWidget, GLwNinputCallback,  gl_input_callback,c_data);
  XtAddCallback(newWidget, GLwNexposeCallback, gl_expose_callback,c_data);
  XtAddCallback(newWidget, GLwNresizeCallback, gl_resize_callback,c_data);
  XtAddCallback(newWidget, "destroyCallback",  gl_destroy_callback,c_data);

  if(type == STRING) XtFree(nom); 
  /* un nouveau widget */
  interneObjetWidget(rep_atome, newWidget);
  ((GLObjectPtr) c_data)->atome = a1 = (Int *)rep_atome;
  /* informations transférées à la création */
  ((GLObjectPtr) c_data)->w = a2; /* partage de listes */
  ((GLObjectPtr) c_data)->root_list = (Int) gl_dpy;

  rep_atome->tags |= GL_WINDOW;

  if((Int *) *atcareful==t && ((Int) pbind>0||(Int *) *atcrewid!=nil)) {
    if(!pbind) a5=(Int *)*atcrewid;
    else{a5= (Int *)*(pbind-2); a5 = (Int *)*(a5+2);}
    adapteival(atcrewid,a1,a5);
  }
  derec;
}

/* Procedure d'ajout d'un callback a une fenetre de dessin :
 * Ceci consiste uniquement a ajouter l'evenements et l'expression 
 * dans la pile des callbacks de la fenetre 
 */
Int gl_addcallback() /* (GLcallback "EventType"  "Expression") SUBR2 */
{
  Int i = 0, eType;
  char *expr = (isstr(a2)?(char *)(a2+(2/2)):
		(isatom(a2)?(char *)(((struct atome *)a2)->pn+(2/2)):NULL)),
       *name = (isstr(a1)?(char *)(a1+(2/2)):
		(isatom(a1)?(char *)(((struct atome *)a1)->pn+(2/2)):NULL));

  if(!name || !expr || !wobj) { a1=nil; derec; }

  if((eType = getEventTypeByName(name)) < 0) { a1=nil; derec;}

  /* recherche d'un evenement de ce type pour la fenetre */
  if(!wobj->callback_number) {
    wobj->callback_expression =(CHAR **) XtMalloc(sizeof(CHAR *));
    wobj->callback_event =(Int *) XtMalloc(sizeof(Int));
    wobj->callback_event[0] = eType;
    ++wobj->callback_number;
  }
  else {
    for(i = 0; i < wobj->callback_number; i++) 
      if(wobj->callback_event[i] == eType) break;

    if(i >= wobj->callback_number) {
      wobj->callback_expression =(CHAR **) XtRealloc((char *)wobj->callback_expression,
						     ++wobj->callback_number*sizeof(char *));
      wobj->callback_event =(Int *) XtRealloc((char *)wobj->callback_event,
						wobj->callback_number*sizeof(Int));
      wobj->callback_event[i] = eType;
    }
    else XtFree(wobj->callback_expression[i]);
  }

  wobj->callback_expression[i] = strcpy(XtMalloc(strlen(expr)+1), expr);
  a1=t;
  derec;
}
  

/* Procedure de choix de la fenetre de dessin GL */
Int gl_winset() /* (GLwinset atome) SUBR1 */
{
  Display *dpy;
  Widget w;
  if(wobj && isWidget(a1) && (w = GET_WIDGET_FROM_ATOME(a1)) &&
     (Widget)wobj->w != w) {
      if(wobj = getWObject(w)) {
	GLwDrawingAreaMakeCurrent((Widget)wobj->w, wobj->glx_context);
	a1=t;
      } else a1=nil;
  } else a1=nil;
  derec;
} 

/* gestion des attributs */

Int gl_pousse_attrib()
{
  long mask = 0;
  Int max;

  if(a1 == nil) mask = GL_ALL_ATTRIB_BITS;
  else
    while(a1 != nil) {
      if(isstr(car(a1))) mask |= getGLvalue((char *) (car(a1)+(2/2)));
      a1 = cdr(a1);
    }
  if(mask != 0) { glPushAttrib(mask); a1 = t; }
  else a1 = nil;
  derec;
}

Int gl_tire_attrib()
{
  glPopAttrib();
  derec;
}

Int gl_copy_attrib()
{
  long mask = GL_ALL_ATTRIB_BITS;
  GLObjectPtr wobj1, wobj2;
  Display *dpy1, *dpy2;
  Widget w1, w2;

  if(isstr(a3)) mask = getGLvalue((char *) (a3+(2/2)));

  if(isWidget(a1) && (w1 = GET_WIDGET_FROM_ATOME(a1)) && (wobj1 = getWObject(w1)) &&
     isWidget(a2) && (w2 = GET_WIDGET_FROM_ATOME(a2)) && (wobj2 = getWObject(w2))) {
    dpy1 = XtDisplay(w1);
    glXCopyContext(dpy1, wobj1->glx_context, wobj2->glx_context, mask);
    a1 = t;
  }
  else
  a1 = nil;
  derec;
}
		  
/* clear de la fenetre GL courante */
Int gl_clear()
{
  wobj->root_list = 0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glFlush();
  redessine();
  derec;
}


/* ********************************* */
/* Gestion de la projection utilisée */
/* ********************************* */

/* Orthonormée */
Int gl_ortho() /* (GLortho x1 x2 y1 y2 [z1 z2]) NSUBR */
{
  Int cnt;

  /* on était en mode perspective : on repositionne normalement */
  if(wobj->mode | GL_AUTO_PERSP) {
    wobj->t_translate[2] -= wobj->p_data[2];
    build_modelview();
  }

  wobj->mode &= GL_RESET_PROJ;

  if(getNumArgFroma1((char *)wobj->p_data, 0, GLXBVL_SHORT) == 4) 
    wobj->mode |= GL_MODE_ORTHO2;
  else
    wobj->mode |= GL_MODE_ORTHO;
  build_projection(0,0,0);
  redessine();
  a1 = t;
  derec;
}
#define max(v0, v1)    ((v0 > v1) ? v0 : v1)
Int gl_auto_perspective()
{
  double zmin, zfar, ratio, fovy;
  double pix_size_x, pix_size_y;
  Int w, h, wmm, hmm;
  Display *dpy = XtDisplay((Widget)wobj->w);
  Screen *scr = XtScreen((Widget)wobj->w);
  
  w = (Int) scr->width;
  wmm = (Int) scr->mwidth;
  h = (Int) scr->height;
  hmm = (Int) scr->mheight;
  pix_size_x = w / (double) wmm;
  pix_size_y = h / (double) hmm;
  
  /* on était en mode perspective : on repositionne normalement */
  if(wobj->mode | GL_AUTO_PERSP) wobj->t_translate[2] -= wobj->p_data[2];

  wobj->mode &= GL_RESET_PROJ;
  wobj->mode |= GL_AUTO_PERSP;
  /* ZMIN : distance minimale = 750mm * npixels/mm */
  wobj->p_data[2] = 750 * max(pix_size_x, pix_size_y);
  /* ZFAR : a voir... */
  wobj->p_data[3] = 2 * zmin;
  /* ASPECT */
  wobj->p_data[1] = (double) wobj->width / (double) wobj->height; 
  /* FovY  = 2 atan((hauteur / 2) / distance) */
  wobj->p_data[0] = (2 * atan(wobj->height / (2.0 * wobj->p_data[2]))) * 900.0 / M_PI_2; /* résultat en 10ème de degré */
  /* on ajoute la translation en Z de Zmin pour regarder vers 0... */
  wobj->t_translate[2] += wobj->p_data[2];
  /* et on reconstruit les matrices ... */
  build_projection(0,0,0);
  build_modelview();
  redessine();
  a1 = t;
  derec;
}

/* Pespective */
Int gl_perspective() /* (GLperspective fovy [Znear] [Zfar]) (aspect calculated automatically)*/
{
  if(!isnum(a1)) { a1 = nil; derec; }
  
  /* on était en mode perspective : on repositionne normalement */
  if(wobj->mode | GL_AUTO_PERSP) {
    wobj->t_translate[2] -= wobj->p_data[2];
    build_modelview();
  }

  wobj->p_data[0] = (double)valnb(a1); /* fovy */
  wobj->p_data[1] = (double) wobj->width / (double) wobj->height; /* aspect */
  if(isnum(a2)) wobj->p_data[2] = (double)valnb(a2); /* zmin */
  if(isnum(a3)) wobj->p_data[3] = (double)valnb(a3); /* zfar */
  
  wobj->mode &= GL_RESET_PROJ;
  wobj->mode |= GL_MODE_PERSP;
  build_projection(0,0,0);
  redessine();
  a1 = t ;
  derec;
}

/* Window */
Int gl_frustum()
{
  Int cnt;

  /* on était en mode perspective : on repositionne normalement */
  if(wobj->mode | GL_AUTO_PERSP) {
    wobj->t_translate[2] -= wobj->p_data[2];
    build_modelview();
  }
  wobj->mode &= GL_RESET_PROJ;
  wobj->mode |= GL_MODE_FURSTRUM;
  for(cnt = 0, a2 = car(a1); ; a2=car(a1 = cdr(a1)), cnt++) {
    if(!isnum(a2)) { --cnt; break; }
    wobj->p_data[cnt] = (double) valnb(a2);
    if(isnotlist(a1)) break;
  }
  a1 = t;
  build_projection(0,0,0);
  redessine();
  derec;
}

/* Positionnement de l'oeuil */
Int gl_lookat() /* (GLlookat vx vy vz px py pz [upx upy upz]) NSUBR */
{
  Int cnt_arg;
  if(a1 == nil) { /* si pas d'arguments on enlève le mode lookat */
    wobj->mode &= ~GL_MODE_LOOKAT;
  }
  else {
    if(!(cnt_arg = getNumArgFroma1((char *) wobj->p_eye,0,GLXBVL_DOUBLE)))
      {a1=nil; derec;}
    if(cnt_arg == 6) wobj->p_eye[6] = wobj->p_eye[8] = 0.0, wobj->p_eye[7] = 1.0;
    else if(cnt_arg != 9) { a1 = nil; derec; }
    wobj->mode |= GL_MODE_LOOKAT;
  }
  build_modelview();
  redessine();
  a1=t;
  derec;
}


Int gl_polarv() /* (GLpolarview dist azim inc twist) NSUBR */
{
  a1=t;
  derec;

}
  
/* ************************************************************* */
/* transformations de l'image : gestion de la matrice de l'image */
/* ************************************************************* */

/* ordre des rotations */
Int gl_rotate_order()
{
  if(!isnum(a1) || !isnum(a2) || !isnum(a3)) { a1 = nil; derec; }
  wobj->t_rot_order[0] = valnb(a1);
  wobj->t_rot_order[1] = valnb(a2);
  wobj->t_rot_order[2] = valnb(a3);
  a1 = t;
  derec;
}

/* Rotation de l'image */
Int gl_rotate_image() /* si (GLimrotate angle [axe|angle_y angle_z]) SUBR3 */
{
  char axis;

  if(!wobj || !isnum(a1)) {a1=nil; derec; }
  if(isstr(a2)) {
    axis =  *(char *)(a2+(2/2));
    if(axis > 'Z') axis -= 'a' - 'A';
    if(axis < 'X' || axis > 'Z') {a1=nil; derec;}
  }
  else if(isnum(a2) && isnum(a3)) axis = 0;

  if(axis) {
    wobj->t_rotate[(axis-'X')] = (double) fvalue(a1);
  }
  else {
    wobj->t_rotate[0] = (double) fvalue(a1);
    wobj->t_rotate[1] = (double) fvalue(a2);
    wobj->t_rotate[2] = (double) fvalue(a3);
  }
  build_modelview();
  redessine();
  a1=t;
  derec;
}

Int gl_scale_image() /* (GLimscale sx sy sz fsize) NSUBR */
{
  float data[4];
  if(!wobj || !getNumArgFroma1((char *)wobj->t_scale,3,GLXBVL_DOUBLE)) { a1=nil; derec; }
  if(a1 != nil) wobj->t_scale[3] = valnb(a1);
  else wobj->t_scale[3] = 1;
  build_modelview();
  redessine();
  a1 = t;
  derec;
}  

Int gl_translate_image() /* (GLimtranslate x y z) SUBR3 (sx, sy et sz entiers) */
{
  if(!wobj || !isnum(a1) || !isnum(a2) || !isnum(a3)) { a1=nil; derec; }
  
  wobj->t_translate[0] = (double) fvalue(a1);
  wobj->t_translate[1] = (double) fvalue(a2);
  wobj->t_translate[2] = (double) fvalue(a3);
  build_modelview();
  redessine();
  a1=t;
  derec;
}


/* flush du buffer graphique */
Int gl_flush() /* (GLflush) */
{
  if(!wobj) derec;
  redessine();
  derec;
}

/* récupération des valeurs */
Int gl_get_valeur()
{
  GLenum prop;     /* propriété */
  double values[4]; /* valeurs */
  if(!isstr(a1)) { a1 = nil ; derec; }
  prop = getGLvalue(STR_FROM_STRLSP(a1));
  memset(values, '\0', 4 * sizeof(double));
  glGetDoublev(prop, values);
  a1 = cons(craflo(values[0]), 
	    cons(craflo(values[1]), 
		 cons(craflo(values[2]), 
		      cons(craflo(values[3]), nil))));
  derec;
}
  

#endif














