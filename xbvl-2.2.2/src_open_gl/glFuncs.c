/*
 *      glFuncs.c
 * definition des fonctions d'interactions avec GL
 * Ce fichier contient particulierement les routines de dessin de GL
 * D.Ploix novembre 93
 */

#if (OPEN_GL | MESA_GL)
#include <math.h>
#include "glXbvl.h"
#include "vlisp.h"

/* GESTION DE LA COULEUR */

/* procedure de changement de la couleur */
Int gl_couleur() /* (GLcolor ...) NSUBR (les arguments dependent du mode : */      
{          /* si cmode : nom, si RGB : 3|4 r g b [a] */
  float cv[4];
  Int v;
  v = valnb(car(a1));      a1 = cdr(a1); 
  cv[0] = (float)valnb(car(a1)) / 255.;  a1 = cdr(a1); 
  cv[1] = (float)valnb(car(a1)) / 255.;  a1 = cdr(a1); 
  cv[2] = (float)valnb(car(a1)) / 255.;  a1 = cdr(a1);
  if(!wobj) derec;
  if(v == 4) 
    {
      if(isnotlist(a1)) { a1=nil; derec; }
      cv[3] = (float)valnb(car(a1)) / 255.;
      glColor4fv(cv);
    }
  else  glColor3fv(cv);

  a1 = t ;
  derec;
}

Int gl_Couleur() /* (GLColor ...) NSUBR */
{
  unsigned short cv[4];
  if(getNumArgFroma1((char *)&cv[0], 0, GLXBVL_SHORT) == 4)
    glColor4usv(cv);
  else glColor3usv(cv);
  a1 = t;
  derec;
}

/* Style et Epaisseur de la ligne */

Int gl_epaisseur() /* (GLlineWidth n) SUBR1 */
{
  if(!isnum(a1)) {a1 = nil; derec;}
  glLineWidth((float) fvalue(a1));
  a1 = t;
  derec;
}

Int gl_style() /* (GLlineStipple stipple [factor]) SUBR2 */
{
  if(!isnum(a1)) { a1 = nil ; derec; }
  glLineStipple((isnum(a2) ? (Int) valnb(a2) : 1), (unsigned short) valnb(a1));
  a1 = t;
  derec;
}

/* ROUTINES DE DESSIN */

/* debut d'une sequence de vertex  */
Int gl_bgn() /* (GLbgn type) SUBR1 */
{
  char *type;
  long t_value;
  if(!isstr(a1) || !wobj) derec;
  type = STR_FROM_STRLSP(a1);

  a1=t;
  if((t_value = getGLvalue(type)) >= 0)  glBegin(t_value);
  else a1 = nil;
  derec; 
}

/* fin d'une sequence de vertex */
Int gl_fini() /* (GLend) SUBR0 */
{
  if(!wobj) derec;
  glEnd();
  a1=t;
  derec; 
}


/* dessin avec les vertex */
/* (GLvertex type ['(ratio)] data)  NSUBR */
/* type = 2 3 ou 4 */
/* data = x1 y1 [z1] [t1]  (n fois) */
#define datan(n) ((n == 4) ? &data4[0] : (n == 3) ? &data3[0] : &data2[0])
Int gl_vertex()
{
  Int dtype,i;
  float data2[2], data3[3], data4[4], *data, ratio;
  a2 = car(a1); a1 = cdr(a1);
  if(wobj && isnum(a2) && (dtype = valnb(a2)) > 1 && dtype < 5) {
    data = datan(dtype);
    if(islist(car(a1))) { ratio=fvalue(car(car(a1))); a1=cdr(a1); }
    else ratio=1.0;
    for( ;; ) {
      if(!getNumArgFroma1((char *) data, dtype, GLXBVL_FLOAT)) break;
      for(i=0;i<dtype;i++) data[i] /= ratio;
      switch(dtype) {
      case 2 : glVertex2f(data2[0], data2[1]); break;
      case 3 : glVertex3f(data3[0], data3[1], data3[2]); break;
      case 4 : glVertex4f(data4[0], data4[1], data4[2], data4[3]) ; break;
      }
    }
    a1 = t;
    derec;
  }
  else { a1 = nil; derec; }
}

/* Swaptmesh */
Int gl_swap_tmesh() /* (GLswaptmesh) SUBR0 */
{
  a1=t;
  derec;
}

/* dessin des normales (utilisation des lumieres) *
/* (GLnormal [(ratio)] data) NSUBR */
/* ratio = diviseur des donnees */
/* data = x1 y1 z1 ... (nfois) */
Int gl_normal()
{
  Int dtype;
  float data[3];
  float ratio;

  if(wobj) {

    if(islist(car(a1))) {
      ratio=fvalue(car(car(a1)));
      a1=cdr(a1);
    }


    if(isnum(a2) && (dtype = valnb(a2)) > 1 && dtype < 5) 
      for( ;; ) {
	if(!getNumArgFroma1((char *) data,3,GLXBVL_FLOAT)) break;
	glNormal3fv(data);
      }
    a1 = t;
  }
  derec;
}  

/* Utilisation des surfaces */

/* Beziers */

static Int my_dummy_length(l)
Int *l;
{
  Int ret = 0;
  while(islist(l)) { ++ret; l = cdr(l); }
  return ret;
}

/* Une dimension */

Int gl_bezier_map_1() /* (GLmap1 Target u1 u2 control_points) NSUBR */
{
  Int stride = 4, n_ctrl_pt = 0, i, j;
  long map_param;
  float u1, u2, *ctrl_pts = NULL;

  if(a1 == nil) derec;

  /* recherche du type */
  a2 = car(a1);  a1 = cdr(a1);

  if(!isstr(a2) || !wobj) { a1 = nil; derec; }
  if((map_param = getGLvalue((char *) (a2+(2/2)))) < 0) { a1 = nil; derec; }
 
  /* recuperation de la plage */
  a2 = car(a1);  a1 = cdr(a1);  if(!isnum(a2)) { a1 = nil; derec; }  u1 = fvalue(a2);
  a2 = car(a1);  a1 = cdr(a1);  if(!isnum(a2)) { a1 = nil; derec; }  u2 = fvalue(a2);

  /* recuperation des points de control */
  a1 = car(a1);
  n_ctrl_pt = my_dummy_length(a1);
  ctrl_pts = (float *) XtMalloc(n_ctrl_pt * 4 * sizeof(float));

  for(i = 0; a1 != nil && i < n_ctrl_pt; i++) {
    a2 = car(a1);  a1 = cdr(a1);
    for(j = 0; j < 4 && a2 != nil; j++, a2 = cdr(a2))
      ctrl_pts[(i * 4) + j] = (float) fvalue(car(a2));
  }
  /* finalement : execute Map1... */
  if(n_ctrl_pt) {
    glMap1f(map_param, u1, u2, stride, n_ctrl_pt, ctrl_pts);
    XtFree((char *)ctrl_pts);
    a1 = t;
  }
  derec;
}

Int gl_bezier_eval_1() /* (GLeval1 val) SUBR1 */
{
  float val;
  if(!isnum(a1) || !wobj) { a1 = nil ; derec; }
  val = (float) fvalue(a1);
  glEvalCoord1f(val);
  a1 = t;
  derec;
}
    
Int gl_bezier_grid_1() /* (Ggrid1 n u1 u2) SUBR3 */
{
  Int n;
  float u1, u2;
  if(!isnum(a1) || !isnum(a2) || !isnum(a3) || !wobj) { a1 = nil ; derec; }

  n = valnb(a1);
  u1 = (float) fvalue(a2); u2 = (float) fvalue(a3);
  glMapGrid1f(n, u1, u2);
  a1 = t;
  derec;
}

Int gl_bezier_mesh_1() /* (GLmesh1 type p1 p2) SUBR3 */
{
  long type;
  Int p1, p2;
  if(!isstr(a1) || !isnum(a2) || !isnum(a3) || !wobj ||
     (type = getGLvalue((char *) (a1 + (2/2)))) < 0) { a1 = nil; derec; }

  p1 = valnb(a2);  p2 = valnb(a3);

  glEvalMesh1(type, p1, p2);
  a1 = t;
  derec;
}  

/* Deux Dimensions */

Int gl_bezier_map_2() /* (GLmap2 Target u1 u2 v1 v2 values) NSUBR */
{
  Int u_order = 0, v_order = 0, i, j, k;
  long map_param;
  float u1, u2, v1, v2, *ctrl_pts = NULL;

  if(a1 == nil) derec;

  /* recherche du type */
  a2 = car(a1);  a1 = cdr(a1);

  if(!isstr(a2) || !wobj) { a1 = nil; derec; }
  if((map_param = getGLvalue((char *) (a2+(2/2)))) < 0) { a1 = nil; derec; }
 
  /* recuperation des plages */
  a2 = car(a1);  a1 = cdr(a1);  if(!isnum(a2)) { a1 = nil; derec; }  u1 = fvalue(a2);
  a2 = car(a1);  a1 = cdr(a1);  if(!isnum(a2)) { a1 = nil; derec; }  u2 = fvalue(a2);
  a2 = car(a1);  a1 = cdr(a1);  if(!isnum(a2)) { a1 = nil; derec; }  v1 = fvalue(a2);
  a2 = car(a1);  a1 = cdr(a1);  if(!isnum(a2)) { a1 = nil; derec; }  v2 = fvalue(a2);

  /* recuperation des points de control */
  a1 = car(a1);
  v_order = my_dummy_length(a1);
  u_order = my_dummy_length(car(a1));
  ctrl_pts = (float *) XtMalloc(v_order * u_order * 4 * sizeof(float));
  
  for(i = 0; i < v_order && a1 != nil; i++) {
    a2 = car(a1);  a1 = cdr(a1);
    for(j = 0; j < u_order && a2 != nil; j++) {
      a3 = car(a2); a2 = cdr(a2);
      for(k = 0; k < 4 && a3 != nil; k++) {
	a4 = car(a3); a3 = cdr(a3);
	ctrl_pts[(i * u_order * 4) + (j * 4) + k] = (float) fvalue(a4);
      }
    }
  }
  /* finalement : execute Map2... */
  glMap2f(map_param, 
	  u1, u2, 4, u_order,
	  v1, v2, 4 * u_order, v_order,
	  ctrl_pts);
  XtFree((char *)ctrl_pts);
  a1 = t;
  derec;
}

Int gl_bezier_eval_2() /* (GLeval1 u v) SUBR2 */
{
  float u, v;
  if(!isnum(a1) || !isnum(a2) || !wobj) { a1 = nil ; derec; }
  u = (float) fvalue(a1);
  v = (float) fvalue(a2);
  glEvalCoord2f(u, v);
  a1 = t;
  derec;
}
    
Int gl_bezier_grid_2() /* (Ggrid2 nu u1 u2 nv v1 v2) NSUBR */
{
  Int nu, nv;
  float u1, u2, v1, v2;
  if(!wobj) { a1 = nil ; derec; }

  a2 = car(a1); a1 = cdr(a1); nu = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); nv = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); u1 = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); u2 = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); v1 = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); v2 = valnb(a2);
  glMapGrid2f(nu, u1, u2, nv, v1, v2);
  a1 = t;
  derec;
}

Int gl_bezier_mesh_2() /* (GLmesh2 type p1 p2 q1 q2) NSUBR */
{
  long type;
  Int p1, p2, q1, q2;
  if(!wobj) { a1 = nil; derec; }

  a2 = car(a1); a1 = cdr(a1); type = getGLvalue((char *) (a2 + (2/2)));
  a2 = car(a1); a1 = cdr(a1); p1 = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); p2 = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); q1 = valnb(a2);
  a2 = car(a1); a1 = cdr(a1); q2 = valnb(a2);

  glEvalMesh2(type, p1, p2, q1, q2);
  a1 = t;
  derec;
}  

/* **************** Formes GL predefinies **************** */

/* ******** gestion des rectangles ******** */

/* **** Zones de l'ecran ... **** */

Int gl_rectcpy() /* (GLrcopy x0 y0 x1 y1 x-to y-to) NSUBR */
{
  a1 = t ;
  derec;
}

Int gl_rectzoom() /* (GLrzoom zx zy) SUBR2 */
{
  a1 = t ;
  derec;
}


Int gl_rect() /* (GLrect x1 y1 x2 y2) NSUBR */
{
  float data[4];
  if(!wobj || !getNumArgFroma1((char *) data,4,GLXBVL_FLOAT)) {a1=nil; derec;}
  glRectf(data[0],data[1],data[2],data[3]);
  a1 = t ;
  derec;
}  

Int gl_rectf() /* (GLrectf x1 y1 x2 y2) NSUBR */
{
  float data[4];
  if(!wobj || !getNumArgFroma1((char *) data,4,GLXBVL_FLOAT)) {a1=nil; derec;}
  glRectf(data[0],data[1],data[2],data[3]);
  a1 = t ;
  derec;
}

Int gl_polyg() /* (GLpoly [(ratio)] x1 y1 z1 x2 y2 z2 ...) NSUBR */
{
  GLfloat parray[100][3] ;
  GLfloat ratio = 1. ;
  Int nb_v, i;
  
  if(!wobj) { a1 = nil; derec; }

  a2 = car(a1), a1 = cdr(a1);
  if(islist(a2)) ratio = fvalue(car(a2));
  for(nb_v = 0; a1 != nil; nb_v++) {
    parray[nb_v][0] = fvalue(car(a1)) / ratio; a1 = cdr(a1);
    parray[nb_v][1] = fvalue(car(a1)) / ratio; a1 = cdr(a1);
    parray[nb_v][2] = fvalue(car(a1)) / ratio; a1 = cdr(a1);
  }
  glBegin(GL_POLYGON) ;
  for(i = 0 ; i < nb_v  ; ++i)
    glVertex3fv(parray[i]) ;
  glEnd() ;
  a1=t;
  derec;
}

Int gl_polym() /* (GLpolymode mode) SUBR1 || (GLpolymode face mode) SUBR2 (OpenGl)*/
{
  long mode, face;
  face = getGLvalue((char *) (a1+(2/2)));
  mode = getGLvalue((char *) (a2+(2/2)));
  if(!wobj) { a1 = nil; derec; }
  glPolygonMode(face, mode);
  a1=t;
  derec;
}

#define angle_to_m(a)  (a * M_PI / (float) 1800)
#define abs(x)         ((x) < 0 ? -(x) : (x))

Int gl_get_circle_pos() /* (GLgetXYangulaire cx cy r angle) NSUBR */
{
  float data[4], ret_x, ret_y;
  if(!wobj || !getNumArgFroma1((char *) data,4,GLXBVL_FLOAT)) {a1=nil;derec;}
  
  ret_x = data[0] + sin(angle_to_m(data[3])) * data[2];
  ret_y = data[1] + cos(angle_to_m(data[3])) * data[2];
  a1 = cons(cranb((Int) ret_x), cons(cranb((Int) ret_y), nil));
  derec;
}

Int gl_get_real_position() /* (GLgetposition x y z rx ry rz sx sy sz sw) */
{
  float data[10], x, y, z, w;
  float CM[4][4];
  Int i;

  if(!wobj) { a1 = nil; derec; }

  memset((void *)data, '\0', sizeof(float)*10);
  getNumArgFroma1((char *) data,10,GLXBVL_FLOAT);

  glPushMatrix();
  glLoadIdentity();
  glRotatef(data[3]/10.,1.0,0.0,0.0);
  glRotatef(data[4]/10.,0.0,1.0,0.0);
  glRotatef(data[5]/10.,0.0,0.0,1.0);
  for(i = 6; i < 10; i++) if(data[i] == 0.0) data[i]=1;
  glScalef(data[6]/data[9],data[7]/data[9],data[8]/data[9]);
  glTranslatef(data[0], data[1], data[2]);
  glGetFloatv(GL_MODELVIEW_MATRIX, (float *)CM);
  glPopMatrix();

  a1=cons(cranb((Int)CM[3][0]),cons(cranb((Int)CM[3][1]),cons(cranb((Int)CM[3][2]),nil)));
  derec;
}
					  

Int gl_get_matrix() /* (GLgetMatrix atom) SUBR1 */
{
  GLdouble *Mat;
  struct atome *at = (struct atome *) a1;

  if(!wobj || !isatom(a1) || a1 == nil) {a1 = nil; derec;}
  if(at->obj != nil) Mat = (GLdouble *) at->obj;
  else at->obj = (Int *) (Mat = (GLdouble *) XtMalloc(sizeof(GLdouble) * 16));
  glGetDoublev(GL_MODELVIEW_MATRIX, Mat);
  derec;
}

Int gl_matrix_mult() /* (GLmultMatrix atom) SUBR1 */
{
  struct atome *at = (struct atome *) a1;

  if(!wobj || !isatom(a1) || a1 == nil || at->obj == nil) { a1= nil; derec;}

  glMultMatrixd((GLdouble *)at->obj);
  derec;
}

Int gl_matrix_load()  /* (GLloadMatrix atom) SUBR1 */
{
  struct atome *at = (struct atome *) a1;

  if(!wobj || !isatom(a1) || a1 == nil || at->obj == nil) {a1 = nil; derec; }

  glLoadMatrixd((GLdouble *)at->obj);
  derec;
}

Int gl_get_matrix_value() /* (GLgetMatrixValue atom row col) SUBR3 */
{
  struct atome *at = (struct atome *) a1;

  if(!wobj || !isatom(a1) || a1 == nil || at->obj == nil) { a1 = nil; derec; }
  a1 = craflo((float) (((GLdouble **)at->obj)[valnb(a2)][valnb(a3)]));
  derec;
}

Int gl_set_matrix_value() /* (GLsetMatrixValue atom row col value) NSUBR */
{
  struct atome *at = (struct atome *) car(a1);
  Int row = valnb(car(cdr(a1))),
      col = valnb(car(cdr(cdr(a1))));
  float val = valflo(car(cdr(cdr(cdr(a1)))));

  if(!wobj || !isatom(a1) || a1 == nil || at->obj == nil) { a1 = nil; derec; }
  ((GLdouble **) at->obj)[row][col] = (GLdouble) val;
  derec;
}


Int gl_free_matrix() /* (GLfreeMatrix atom) SUBR1 */
{
  struct atome *at = (struct atome *) a1;

  if(!wobj || !isatom(a1) || a1 == nil || at->obj == nil) {a1 = nil; derec;}
  XtFree((char *) at->obj);
  at->obj = nil;
  derec;
}

Int gl_circle() /* (GLellipse x y rx ry b_ang e_ang) NSUBR */
{
  float v[3], theta;
  short data[6];
  float phi, incr;

  if(!wobj || !getNumArgFroma1((char *) data,6,GLXBVL_SHORT)) {a1=nil;derec;}
  if(data[4] == data[5]) ++data[5];
  incr = (((data[2] + abs(data[2] - data[3])) >> 2) & -4);
  if(incr < 20) incr = 20;
  else if(incr > 256) incr = 256;

  glBegin(GL_LINE_LOOP);
  for(phi = (float) data[4]; phi <= (float) data[5]; phi += (data[5] - data[4]) / incr) {
    v[0] = (float) data[0] + sin(angle_to_m(phi)) * (float) data[2];
    v[1] = (float) data[1] + cos(angle_to_m(phi)) * (float) data[3];
    v[2] = 0;
    glVertex3fv(v);
  }
  glEnd();
  a1=t;
  derec;
}

/* gestion des t-mesh : swaptmesh */
Int gl_swaptmesh() /* (GLswaptmesh) SUBR0 */
{
  derec;
}

/* gestion des ombres : GOURAUD ou FLAT */
Int gl_shademodel() /* (GLshademodel type) SUBR1 */
{
  long truc;
  if(!wobj || !isstr(a1)) derec;
  if((truc = getGLvalue((char *) (a1+(2/2)))) >= 0) { 
    glShadeModel(truc) ;
    a1 = t; 
  }
  else a1=nil;
  derec;
}

/* type de rendering sur les polygones */
Int gl_polymode() /* if Irix GL (GLpolymode type) SUBR1 
	         if Open GL (GLpolymode type mode) SUBR2 */
{
  long truc;
  if(!wobj || !isstr(a1)) derec;
  if((truc = getGLvalue((char *) (a1+(2/2)))) >= 0) { 
    long truc1 ;
    if(!isstr(a2)) derec;
    if((truc1 = getGLvalue((char *) (a2+(2/2)))) >= 0) { 
      glPolygonMode(truc, truc1) ;
      a1 = t ;
    }
    else a1 = nil ;
  }
  else a1=nil;
  derec;
}

/* setting du mode d'utilisation des matrices */
Int gl_mmode() /* (GLmmode mode) SUBR1 */
{
  long type;
  if(!wobj || !isstr(a1)) derec;
  if((type = getGLvalue((char *) (a1+(2/2)))) >= 0)
    { 
      glMatrixMode(type); 
      a1 = t; 
    }
  else a1 = nil;
  derec;
}


/* acces aux matrices de GL */
Int gl_pushmatrice() /* (GLpushmatrix) */
{
  if(!wobj) derec;
  glPushMatrix() ;
  a1=t;
  derec;
}

  

Int gl_popmatrice() /* (GLpopmatrix) */
{
  if(!wobj) derec;
  glPopMatrix() ;
  a1=t; 
  derec;
}

Int gl_loadidmatrix() /* (GLloadidmatrix) */
{ 
  if(!wobj) derec;
  glLoadIdentity() ;
  a1=t;
  derec;
}

/* Routines de transforamtion */

/* Scaling */
Int gl_echelle() /* (GLscale sx sy sz fsize) NSUBR */
{
  float data[4];

  if(!wobj || !getNumArgFroma1((char *) data,3,GLXBVL_FLOAT)) { a1=nil; derec; }
  if(a1 != nil) data[3] = fvalue(car(a1));
  else data[3] = 1;

  glScalef(data[0] /  data[3], data[1] /  data[3], data[2] /  data[3]);
  a1 = t;
  derec;
}

/* Translation */
Int gl_translat() /* (GLtranslate x y z) SUBR3 (sx, sy et sz entiers) */
{
  float x, y, z;
  if(!wobj || a1 == nil || a2 == nil || a3 == nil) { a1=nil; derec; }

  x = (float) fvalue(a1); y = (float) fvalue(a2); z = (float) fvalue(a3);
  glTranslatef(x, y, z) ;
  a1=t;
  derec;
}

/* Rotation */
Int gl_rotate() /* (GLrotate angle axe) SUBR2 (angle est en disieme de degre) */
{
  char axis;

  if(!wobj || !isnum(a1)) {a1=nil; derec; }
  if(isstr(a2)) {
    axis = isstr(a2) ? *(char *)(a2+(2/2)) : 'X';
    if(axis > 'Z') axis -= 'a' - 'A';
    if(axis < 'X' || axis > 'Z') {a1=nil; derec;}
  }
  else if(isnum(a2) && isnum(a3)) axis = 0;

  if(axis != 0) 
    glRotatef((float) fvalue(a1) / 10., 1.0 * !(axis-'X'), 1.0 * !(axis-'Y'), 1.0 * !(axis-'Z'));
  else {
    glRotatef((float) fvalue(a1) / 10., 1.0, 0.0, 0.0);
    glRotatef((float) fvalue(a2) / 10., 0.0, 1.0, 0.0);
    glRotatef((float) fvalue(a3) / 10., 0.0, 0.0, 1.0);
  }

  a1=t;
  derec;
}
     
Int gl_rotate_axis() /* (GLrotateAxis anglex angley anglez) */
{
  float RM[4][4], cx, sx, cy, sy, cz, sz;

#ifndef PI
#define PI 3.1592654
#endif
#define FromDeg(x) (((x)/3600.0)*PI)

  if(!wobj || !isnum(a1) || !isnum(a2) || !isnum(a3)) {a1=nil; derec; }

  cx = (float) cos(FromDeg((float)fvalue(a1)));  sx = (float) sin(FromDeg((float)fvalue(a1)));
  cy = (float) cos(FromDeg((float)fvalue(a2)));  sy = (float) sin(FromDeg((float)fvalue(a2)));
  cz = (float) cos(FromDeg((float)fvalue(a3)));  sz = (float) sin(FromDeg((float)fvalue(a3)));

  RM[0][0] = cy * cz;  RM[0][1] = -1 * sz; RM[0][2] = sy;           RM[0][3] = 0.0;
  RM[1][0] = sz;       RM[1][1] = cx * cz; RM[1][2] = -1 * sx * cz; RM[1][3] = 0.0;
  RM[2][0] = -1 * sy;  RM[2][1] = sx;      RM[2][2] = cx * cy;      RM[2][3] = 0.0;

  RM[3][0] = 0.0;  RM[3][1] = 0.0; RM[3][2] = 0.0; RM[3][3] = 1.0;

  glMultMatrixf((float *)RM);

  a1=t;
  derec;
}
/* ********************** */
/* Gestion des caracteres */
/* ********************** */
Int gl_string() /* (GLstring x y z str) NSUBR */
{
  if(!wobj) derec;
  glhstr(); /* utilisation des Hershey...: pas de gl string en OPEN GL */
}

/* Implementation des NURBS */

/* Modification des donnees de la fenetre pour les nurbs */

Int gl_set_nurbs() /* if Irix gl(GLsetnurbs "DISPLAY"|"TOLERANCE" valeur) SUBR2 
		  if Open GL(GLsetnurbs nurbs property valeur) SUBR3 */
{
  GLUnurbsObj *nObj ;
  GLenum property ;
  GLfloat value ;
  Int index ;

  if(!wobj || !isnum(a1) && !isstr(a2) && !isnum(a3) && !isflo(a3)) {a1 = nil ; derec ;}
  nObj =  (GLUnurbsObj *) valnb(a1) ;
  property = getGLvalue(STR_FROM_STRLSP(a2)) ;
  value = (float) fvalue(a3) ; 
  gluNurbsProperty(nObj, property, value) ;
  wobj->nurbs_data[NURBINDEX(property)] = value;
  wobj->mode |= property ;

  a1 = t ;
  derec;
}
  
/* recuperation du type d'une nurbs */
static long 
getNurbsType(str)
char *str;
{
  return getGLvalue(str);
}



/* sous fonction de recuperations des valeurs pour nurbs curves et surfaces */
static Int 
getNurbsData(result, dim)
float *result; /* table des resultats */
Int *dim;       /* dimension (nombre de poInt resultant par donnee) */
{
  Int *tmp = NULL;
  Int nb_lus, indice = 0;

  for(nb_lus = 0; tmp != nil; nb_lus++, a1=cdr(a1)) {
    tmp = car(a1);
    if(tmp == nil) break;
    if(islist(tmp)) { /* liste de sous listes */
      result[indice++] = (float) fvalue(car(tmp)); tmp = cdr(tmp);
      result[indice++] = (float) fvalue(car(tmp)); tmp = cdr(tmp);
      if(tmp != nil) {
	result[indice++] = (float) fvalue(car(tmp)); tmp = cdr(tmp);
	if(tmp != nil)	result[indice++] = (float) fvalue(car(tmp)), *dim=4;
	else *dim=3,++indice;
      } 
      else *dim=2,indice += 2;
    }
    else result[indice++] = (float) fvalue(tmp);
  }
  return nb_lus;
}
	
/* les donnees fournies en argument sont :
   a1 = liste des points de controle (le nombre de donnees par poInt donnant le type de la courbe)
        example : ((x0 y0 z0) (x1 y1 z1) ... (xn yn zn))
   a2 = liste des points (nombre de points + dimension de la courbe) :
        donne la liste des poInt et la dimension de la courbe.
*/
Int gl_nurbs_curve() /* if Irix GL(GLnurbs liste-de-points-de-controle liste-de-points type) SUBR3 
		    if Open GL(GLnurbs nurbs liste-de-points-de-controle liste-de-points type) NSUBR */
{
  static float control_point[25][4];
  static float points[29];  /* maximum = 25 (points) + 4 (dimension) */
  Int ctrl_pts_nb = 0, dim = 2, pts_nb = 0, type;
  GLUnurbsObj *nObj ;
  if(!wobj || !isnum(car(a1)) || isnotlist(car(cdr(a1))) || isnotlist(car(cdr(cdr(a1))))) {a1=nil; derec;}
  nObj =  (GLUnurbsObj *) valnb(a1) ;
  a1 = cdr(a1) ; ctrl_pts_nb = getNurbsData(&control_point[0][0], &dim);
  a1 = cdr(a1) ; pts_nb = getNumArgFroma1((char *)&points[0],0,GLXBVL_FLOAT);
  a1 = cdr(a1) ; type = getGLvalue(STR_FROM_STRLSP(car(a1))) ;
  gluNurbsCurve(nObj, pts_nb, (float *)points, 4*sizeof(float), (float *)control_point, dim, type);
  a1=t;
  derec;
}
    
/* Definition des surfaces utilisant les nurbs
 * a1 : points de control (liste de sous liste de sous listes de points 1 ou 2)
 * a2 : liste de n_s sous listes contenant n_t sous listes de k points
 *       avec n_s = nombre de points en s
 *            n_t = nombre de points en t
 *              k = nombre de coordonnees par points
 * a3 : type de la surface (string)
 */
Int gl_nurbs_surface() /* (GLsurface pts ctrl_pts type) SUBR3  */
{
  static float control_points[1024];
  static float points[2][30];
  long s_nk, t_nk, nb_s, nb_t, s_order, t_order, type, indice;
  Int is_onek = 0, dim, *tmp; 
  
  if(wobj && isustr(a3)) type = getNurbsType((char *) (a3+(2/2)));
  else {
    a1=nil; derec;
  }
  
  tmp = car(cdr(a1));
  a1 = car(a1);
  s_nk = getNumArgFroma1((char *) &points[0][0], 0, GLXBVL_FLOAT);
  if(tmp != nil) {
    a1 = tmp;
    t_nk = getNumArgFroma1((char *)&points[1][0], 0, GLXBVL_FLOAT);
    is_onek = 1;
  }
  else t_nk = s_nk;

  a3 = a2;
  indice = 0;
  nb_s = 0;
  while(a3 != nil) {
    a1 = car(a3);
    nb_t = getNurbsData(&control_points[indice], &dim);
    indice += 4*nb_t;
    ++nb_s;
    a3 = cdr(a3);
  }
  
  s_order = s_nk - nb_s;
  t_order = t_nk - nb_t;
  
  a1 = t;
  derec;
}
	       


#endif
