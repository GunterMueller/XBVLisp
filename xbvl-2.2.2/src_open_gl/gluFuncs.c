#if (OPEN_GL | MESA_GL)
#include <math.h>
#include "glXbvl.h"
#include "vlisp.h"

/* Implementation des librairies GLU */
/* D.Ploix & O.Blanc, 1995 */

/* reminissance de irix gl */
gl_sphere_mode() /* (GLsphmode "atribute" "value") SUBR2 */
{
  a1 = t ;
  derec;
}
 
/* Les GLU suivantes utilisent les Quadratics, leurs proprietes sont :
   - Normal      = Type de normale (GLU_NONE, GLU_FLAT, GLU_SMOOTH)
   - DrawStyle   = Style de dessin (GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT)
   - Texture     = Utilisation de la texture (GLU_TRUE, GLU_FALSE)
   - Orientation = Orientation Interieur/Exterieur (GLU_INSIDE, GLU_OUTSIDE)
*/
/* Valeurs par defaut : */

#define GLU_NORMAL_VALUE      0
#define GLU_DRAWSTYLE_VALUE   1
#define GLU_ORIENTATION_VALUE 2
#define GLU_TEXTURE_VALUE     3

typedef GLUquadricObj *GLUquadricObjPtr;

static GLenum GLU_Values[4] = {GLU_SMOOTH, GLU_LINE, GLU_OUTSIDE, (Int) GL_FALSE} ;
static GLUquadricObjPtr glu_quadric_object = NULL;

/* creation d'un quadratic */
static GLUquadricObjPtr
glu_newquadratic()
{
  GLUquadricObjPtr ret = gluNewQuadric();
  gluQuadricNormals    (ret, GLU_SMOOTH);
  gluQuadricDrawStyle  (ret, GLU_LINE);
  gluQuadricTexture    (ret, GLU_FALSE);
  gluQuadricOrientation(ret, GLU_OUTSIDE);
  return ret;
}

/* destruction d'un quadratic */
Int glu_delquad() /* (GLUdelquad [adr]) */
{
  if(!wobj || !isnum(a1)) { a1=nil; derec;}
  gluDeleteQuadric((GLUquadricObjPtr) valnb(a1));
  a1 = t;
  derec;
}

/* Changement des valeurs :
   what = "normal" || "drawstyle" || "texture" || "orientation"
*/
Int glu_setvalue() /* (GLUsetvalue what value) SUBR2 */
{
  if(!wobj || !isstr(a1) || !isstr(a2)) {a1 = nil; derec;}
  if(!glu_quadric_object) glu_quadric_object = glu_newquadratic();
  if(!strcmp(STR_FROM_STRLSP(a1), "normal"))        gluQuadricNormals    (glu_quadric_object, getGLvalue(STR_FROM_STRLSP(a2)));
  if(!strcmp(STR_FROM_STRLSP(a1), "drawstyle"))     gluQuadricDrawStyle  (glu_quadric_object, getGLvalue(STR_FROM_STRLSP(a2)));
  if(!strcmp(STR_FROM_STRLSP(a1), "texture"))       gluQuadricTexture    (glu_quadric_object, getGLvalue(STR_FROM_STRLSP(a2)));
  if(!strcmp(STR_FROM_STRLSP(a1), "orientation"))   gluQuadricOrientation(glu_quadric_object, getGLvalue(STR_FROM_STRLSP(a2)));
  a1=t;
  derec;
}

  

/* Dessin de sphere */
Int gl_sphere() /* si Open GL :(GLsphere radius [slices] [stacks]) : NSUBR */
{
  GLdouble radius ;
  GLint slices = 16, stack = 16 ;
  if(!wobj || !isnum(car(a1)) && !isflo(car(a1))) {a1 = nil ; derec ;}
  if(!glu_quadric_object) glu_quadric_object = glu_newquadratic();
  radius = (GLdouble) fvalue(car(a1)) ;
  a1 = cdr(a1) ;
  if(isnotnil(a1) && isnum(car(a1))) {
    slices = valnb(car(a1)) ;
    a1 = cdr(a1) ;
    if(isnotnil(a1) && isnum(car(a1)))
      stack = valnb(car(a1)) ;
  }
  gluSphere(glu_quadric_object, radius, slices, stack) ;
  a1 = t;
  derec;
}

/* dessin d'un cylindre */
Int gl_cylinder() /*  si Open GL :(GLcylindre baseRadius topRadius height [slices] [stacks]) : NSUBR */
{
  GLdouble baseRadius, topRadius, height ;
  GLint slices = 16, stack = 16 ;
  GLenum drawStyle = GLU_LINE ;
  if(!wobj || !isnum(car(a1)) && !isflo(car(a1))) {a1 = nil ; derec ;}
  baseRadius = (GLdouble) fvalue(car(a1)) ;
  a1 = cdr(a1) ;
  if(!isnum(car(a1)) && !isflo(car(a1))) {a1 = nil ; derec ;}
  topRadius = (GLdouble) fvalue(car(a1)) ;
  a1 = cdr(a1) ;
  if(!isnum(car(a1)) && !isflo(car(a1))) {a1 = nil ; derec ;}
  height = (GLdouble) fvalue(car(a1)) ;
  if(!glu_quadric_object) glu_quadric_object = glu_newquadratic();
  a1 = cdr(a1) ;
  if(isnotnil(a1) && isnum(car(a1))) {
    slices = valnb(car(a1)) ;
    a1 = cdr(a1) ;
    if(isnotnil(a1) && isnum(car(a1)))
      stack = valnb(car(a1)) ;
  }
  gluCylinder(glu_quadric_object, baseRadius, topRadius, height, slices, stack) ;
  a1 = t;
  derec;
}

Int gl_disk() /*  si Open GL :(GLdisk inRadius outRadius [slices] [loops]) : NSUBR */
{
  GLdouble inRadius, outRadius ;
  GLint slices = 16, loops = 16 ;
  GLenum drawStyle = GLU_LINE ;
  if(!wobj || !isnum(car(a1)) && !isflo(car(a1))) {a1 = nil ; derec ;}
  inRadius = (GLdouble) fvalue(car(a1)) ;
  a1 = cdr(a1) ;
  if(!isnum(car(a1)) && !isflo(car(a1))) {a1 = nil ; derec ;}
  outRadius = (GLdouble) fvalue(car(a1)) ;
  a1 = cdr(a1) ;
  if(!glu_quadric_object) glu_quadric_object = glu_newquadratic();
  a1 = cdr(a1) ;
  if(isnotnil(a1) && isnum(car(a1))) {
    slices = valnb(car(a1)) ;
    a1 = cdr(a1) ;
    if(isnotnil(a1) && isnum(car(a1)))
      loops = valnb(car(a1)) ;
  }
  gluDisk(glu_quadric_object, inRadius, outRadius, slices, loops) ;
  a1 = t;
  derec;
}

#if OPEN_GL
# include "glXbvlaux.h"
#endif

Int gl_sphere_wire()  /*  si Open GL :(GLsphereWire inRadius) : SUBR1 */
{
  if(!wobj || !isnum(a1) && !isflo(a1)) {a1 = nil ; derec ;}
  auxWireSphere((GLdouble)fvalue(a1)) ;
  a1 = t ; derec ;
}

Int gl_cube_wire()  /*  si Open GL :(GLcubeWire size) : SUBR1 */
{
 if(!wobj || !isnum(a1) && !isflo(a1)) {a1 = nil ; derec ;}
  auxWireCube((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}

Int gl_box_wire()  /*  si Open GL :(GLboxWire width height depth) : SUBR3 */
{
  if(!wobj || (!(isnum(a1) || isflo(a1)) ||
	       !(isnum(a2) || isflo(a2)) ||
	       !(isnum(a3) || isflo(a3))))    {a1 = nil ; derec ;}
  auxWireBox((GLdouble)fvalue(a1), (GLdouble)fvalue(a2), (GLdouble)fvalue(a3)) ;
 a1 = t ; derec ;
}

Int gl_torus_wire()  /*  si Open GL :(GLtorusWire innerRadius outerRadius ) : SUBR2 */
{
  if(!wobj || (!(isnum(a1) || isflo(a1)) ||
	       !(isnum(a2) || isflo(a2))))    {a1 = nil ; derec ;}
  auxWireTorus((GLdouble)fvalue(a1), (GLdouble)fvalue(a2)) ;
 a1 = t ; derec ;
}

Int gl_cylinder_wire()  /*  si Open GL :(GLcylinderWire radius height ) : SUBR2 */
{
  if(!wobj || !(isnum(a1) || isflo(a1)) ||
     !(isnum(a2) || isflo(a2)))    {a1 = nil ; derec ;}
  auxWireCylinder((GLdouble)fvalue(a1), (GLdouble)fvalue(a2)) ;
 a1 = t ; derec ;
}


Int gl_icosahedra_wire()  /*  si Open GL :(GLicosahedraWire radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxWireIcosahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}


Int gl_octahedra_wire()  /*  si Open GL :(GLoctahedraWire radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxWireOctahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}


Int gl_tetrahedra_wire()  /*  si Open GL :(GLtetrahedraWire radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxWireTetrahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}


Int gl_dodecahedra_wire()  /*  si Open GL :(GLdodecahedraWire radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxWireDodecahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}



Int gl_cone_wire()  /*  si Open GL :(GLconeWire base height ) : SUBR2 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1) &&
	       !isnum(a2) && !isflo(a2)))    {a1 = nil ; derec ;}
  auxWireCone((GLdouble)fvalue(a1), (GLdouble)fvalue(a2)) ;
 a1 = t ; derec ;
}


Int gl_sphere_solid()  /*  si Open GL :(GLsphereSolid inRadius) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1))) {a1 = nil ; derec ;}
  auxSolidSphere((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}

Int gl_cube_solid()  /*  si Open GL :(GLcubeSolid size) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1))) {a1 = nil ; derec ;}
  auxSolidCube((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}

Int gl_box_solid()  /*  si Open GL :(GLboxSolid width height depth) : SUBR3 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1) &&
	       !isnum(a2) && !isflo(a2) &&
	       !isnum(a3) && !isflo(a3)))    {a1 = nil ; derec ;}
  auxSolidBox((GLdouble)fvalue(a1), (GLdouble)fvalue(a2), (GLdouble)fvalue(a3)) ;
 a1 = t ; derec ;
}

Int gl_torus_solid()  /*  si Open GL :(GLtorusSolid innerRadius outerRadius ) : SUBR2 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1) &&
	       !isnum(a2) && !isflo(a2)))    {a1 = nil ; derec ;}
  auxSolidTorus((GLdouble)fvalue(a1), (GLdouble)fvalue(a2)) ;
 a1 = t ; derec ;
}

Int gl_cylinder_solid()  /*  si Open GL :(GLcylinderSolid radius height ) : SUBR2 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1) &&
	       !isnum(a2) && !isflo(a2)))    {a1 = nil ; derec ;}
  auxSolidCylinder((GLdouble)fvalue(a1), (GLdouble)fvalue(a2)) ;
 a1 = t ; derec ;
}


Int gl_icosahedra_solid()  /*  si Open GL :(GLicosahedraSolid radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxSolidIcosahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}


Int gl_octahedra_solid()  /*  si Open GL :(GLoctahedraSolid radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxSolidOctahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}


Int gl_tetrahedra_solid()  /*  si Open GL :(GLtetrahedraSolid radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxSolidTetrahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}


Int gl_dodecahedra_solid()  /*  si Open GL :(GLdodecahedraSolid radius ) : SUBR1 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1)))    {a1 = nil ; derec ;}
  auxSolidDodecahedron((GLdouble)fvalue(a1)) ;
 a1 = t ; derec ;
}



Int gl_cone_solid()  /*  si Open GL :(GLconeSolid base height ) : SUBR2 */
{
  if(!wobj || (!isnum(a1) && !isflo(a1) &&
	       !isnum(a2) && !isflo(a2)))    {a1 = nil ; derec ;}
  auxSolidCone((GLdouble)fvalue(a1), (GLdouble)fvalue(a2)) ;
 a1 = t ; derec ;
}



#endif
