/*
 * glLights.c
 * definition des fonctions permettant l'utilisation des lumieres
 * et textures de GL
 */

#if (OPEN_GL | MESA_GL)

#include <math.h>

#include "vlisp.h"
#include "glXbvl.h"

#ifndef STDC_HEADERS
extern long getGLvalue();
#else
extern long getGLvalue(char *);
#endif


/* Dans Open GL, la definition d'objet (materiaux, lumiere ou model n'existe pas 
   => creation de GLlight, GLmaterial et GLmodel
*/

static float values[4] ;

static void 
getProp(GLenum *prop)
{
  Int i;
  if(!wobj || !isstr(a1)) {a1=nil; return; }
  *prop = getGLvalue(STR_FROM_STRLSP(a1));
  a1 = t;
  if(isnum(a2) || isflo(a2)) { values[0] = (float) valflo(a3); a2 = (Int *)1; }
  else if(islist(a2)) {
    for(i = 0; i < 4 && !isnil(a2); i++) { 
      values[i] = (float) valflo(car(a2)); 
      a2 = cdr(a2); 
    }
    a2 = (Int *)2;
  }
  else a1=nil;
}

Int gl_lumiere() /* (GLlight Light Prop Value) : specification de la propriete Prop pour la lumiere Light...: SUBR3 */
{
  GLenum lgt, pnam;
  if(!wobj || !isstr(a1)) {a1=nil; derec;}
  lgt = getGLvalue(STR_FROM_STRLSP(a1));
  a1 = a2;  a2 = a3;
  getProp(&pnam);
  if(a1 == t) {
    if((Int) a2 == 1) { glLightf(lgt, pnam, values[0]); a1=craflo((double)values[0]); }
    else        { glLightfv(lgt, pnam, &values[0]);
		  a1 = cons(craflo((double)values[0]),
			    cons(craflo((double)values[1]),
				 cons(craflo((double)values[2]),
				      craflo((double)values[3]))));
		}
  }
  derec;
}

Int gl_model() /* (GLmodel prop value) SUBR2 */
{
  GLenum prop;
  if(!wobj) derec;
  getProp(&prop);
  if(a1 == t) {
    if((Int) a2 == 1) glLightModelf(prop, values[0]);
    else        glLightModelfv(prop, &values[0]);
  }
  derec;
}

Int gl_materiel() /* (GLmaterial type prop value) SUBR3 */
{
  GLenum lgt, pnam;
  if(!wobj || !isstr(a1)) {a1=nil; derec;}
  lgt = getGLvalue(STR_FROM_STRLSP(a1));
  a1 = a2;  a2 = a3;
  getProp(&pnam) ;
  if(a1 == t) {
    if((Int) a2 == 1) { glMaterialf(lgt, pnam, values[0]); a1=craflo((double)values[0]); }
    else        { glMaterialfv(lgt, pnam, &values[0]);
		  a1 = cons(craflo((double)values[0]),
			    cons(craflo((double)values[1]),
				 cons(craflo((double)values[2]),
				      craflo((double)values[3]))));
		}
  }
  derec;
}

Int gl_permet() /* (GLenable cap) SUBR1 */
{
  if(!wobj || !isstr(a1)) {a1=nil; derec;}
  glEnable((GLenum) getGLvalue(STR_FROM_STRLSP(a1)));
  redessine();
  a1= t;
  derec;
}

Int gl_disable() /* (GLdisable cap) SUBR1 */
{
  if(!wobj || !isstr(a1)) {a1=nil; derec;}
  glDisable((GLenum) getGLvalue(STR_FROM_STRLSP(a1)));
  redessine();
  a1= t;
  derec;
}

Int gl_is_enable() /* (GLisenable cap) SUBR1 */
{
  if(!wobj || !isstr(a1)) {a1=nil; derec;}
  if(glIsEnabled((GLenum) getGLvalue(STR_FROM_STRLSP(a1)))) a1 = t;
  else a1 = nil;
  derec;
}

Int gl_fonction_blend() /* (GLblend Source-Factor Destination-Factor) SUBR2 */
{
  if(!wobj || !isstr(a1) || !isstr(a2)) { a1 = nil; derec; }
  glBlendFunc((GLenum) getGLvalue(STR_FROM_STRLSP(a1)),
	      (GLenum) getGLvalue(STR_FROM_STRLSP(a2)));
  a1 = t;
  derec;
}

#endif /* (OPEN_GL | MESA_GL) */

