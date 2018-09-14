/* 
 *            gl.h
 *   Fichier de definition pour l'implementation du
 * mode GL (Graphic Librarie) de l'iris dans xbvlisp
 *      D.Ploix, novembre 93
 */

#if (OPEN_GL | MESA_GL)
#ifndef _gl_h_
#define _gl_h_

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "vlisp.h"
#include "xwin.h"

typedef struct _gl_object_ {
  Int  *atome;                /* reference a l'atome pere         */
  Int  *w;                    /* widget de dessin ou free list    */
  Int  root_list;           /* Liste utilisateur                */
  /* données du widget */
  GLXContext glx_context;     /* Context pour le widget           */
  Int  callback_number;       /* nombre de callbacks              */
  Int *callback_event;        /* evenements correspondants        */
  CHAR **callback_expression; /* expression des callbacks         */
  Int width, height;          /* pour la conservation de l'aspect */
  Int mode;   /* modes de la fenetre (RGB, BUF, ...)              */
  float  nurbs_data[4];       /* donnees des nurbs                */
  /* Données graphiques */
  double Proj_Mat[16];        /* la matrice de projection */
  double  p_data[6];            /* donnees de la projection */
  double  p_eye[9];             /* pour la position de l'oeil */
  double Model_Mat[16];      /* la matrice de transformations */
  double  t_translate[3];        /* translation */
  double  t_rotate[3];          /* rotation */
  double  t_scale[4];           /* echelle */
  Int     t_rot_order[3];       /* ordre des rotation */
} GLObject, *GLObjectPtr;

#define GL_OBJECT_SIZE   sizeof(GLObject)
#define GL_OBJECT_MEMORY_SIZE   128     /* 128 fenêtres GL différentes dans une meme exécution... */

#define isgl(x)    ((x) >= (Int *) bgl && (x) < (Int *) hgl)
#define isnotgl(x) ((x) < (Int *) bgl || (x) >= (Int *) hgl)

/* Definition des valeurs utilisées pour les modes des fenêtres GL */

#define GL_MODE_ORTHO      1
#define GL_MODE_ORTHO2     (1 << 1)
#define GL_AUTO_PERSP      (1 << 2)
#define GL_MODE_PERSP      (1 << 3)
#define GL_MODE_FURSTRUM   (1 << 4)
#define GL_RESET_PROJ     ~(GL_MODE_ORTHO | GL_MODE_ORTHO2 | GL_MODE_PERSP | GL_MODE_FURSTRUM)

#define GL_MODE_POLARV     (1 << 5)
#define GL_MODE_LOOKAT     (1 << 6)
#define GL_RESET_EYE      ~(GL_MODE_POLARV | GL_MODE_LOOKAT)

#define GL_NURBS_TOLERANCE (1 << 7)
#define GL_NURBS_DISPLAY   (1 << 8)
#define GL_RESET_NURBS    ~(GL_NURBS_TOLERANCE | GL_MODE_DISPLAY)

#define GL_KEEP_ASPECT     (1 << 9)
#define GL_RESET_KASPECT  ~(1 << 10)

/* types de données récupérées des arguments */

#define GLXBVL_CHAR  0
#define GLXBVL_SHORT 1
#define GLXBVL_FLOAT 2
#define GLXBVL_DOUBLE 3

/* types de NURBS */

#define NURBINDEX(prop) ((prop == GLU_SAMPLING_TOLERANCE)?0:\
			  (prop == GLU_DISPLAY_MODE)?1:\
			  (prop == GLU_CULLING)?2:3)

#define gentag()  glGenLists(1)

/* valeur d'un nombre en fonction de son type */
#define fvalue(x)  ((float)valflo(x))

GLObjectPtr bgl, hgl;      /* bas et haut de la memoire GL */
GLObjectPtr free_gl;       /* free list des objets */

extern GLObjectPtr wobj ; /* objet contenant le dessin actuel */

/* pour les images chargées à partir de fichiers */
#define alloc_image_data(width, height)  XtMalloc(sizeof(unsigned char) * 4 * (width+1) * (height+1))
#define free_image_data(image)    XtFree((image)->data)

#ifdef STDC_HEADERS
extern void fixeBind(Widget);

extern void redessine(void);
extern GLObjectPtr cregl(void);
extern void freegl(GLObjectPtr);
extern long getGLvalue(String);
#else
extern void fixeBind();

extern void redessine();
extern GLObjectPtr cregl();
extern void freegl();
extern long getGLvalue();
#endif   /* STDC_HEADERS */


#endif   /* _gl_h_   */
#endif   /* OPEN_GL | MESA_GL */


