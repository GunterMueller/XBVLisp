/*
 *        glLib.c
 * Librairie d'interface entre gl (IRIS) 
 * Ce fichier contient plus specifiquement les fonction autres que 
 * les fonctions de dessin de la librairie GL.
 *     D.Ploix, novembre 93


 * 
 */

#if (OPEN_GL | MESA_GL)


#include "vlisp.h"

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "glXbvl.h"
#include "glTrans.h"

/* **************************************************************** */
/* 
 * getGLvalue : recherche dans la table de translation String -> Value
 * la chaine donnee en argument
 */
long 
getGLvalue(str)
String str;
{
  register Int a = 0, b = GL_TTABLE_SIZE - 1, i = GL_TTABLE_SIZE / 2, val, k;
  if(str[0] < 'A' || str[0] > 'Z') return -1;
  for(k = 0 ; ; k--) {
    if(!(val = strcmp(str,GLTTable[i].nom))) break;
    else if(val < 0) {
      b = i;
      i -= (i - a) / 2;
      if(b == i) break;
    }
    else {
      a = i;
      i += (b - i) / 2;
      if(a == i) break ;
    }
  }
  if(!val) return GLTTable[i].value;
  return -1;
}

/* recuperation d'arguments numeriques de a1 */
/* type = GLXBVL_SHORT : short */
/* type = GLXBVL_FLOAT : float */
/* type = GLXBVL_DOUBLE : double */
Int 
getNumArgFroma1(res, num, type) 
char *res;
short num;
char type;
{
  union { char *c; short *s; float *f; double *d; } val;
  Int *tmp, cnt=0;
  val.c = res;
  for( ; islist(a1) && (!num || cnt < num) ; a1=cdr(a1)) {
    tmp = car(a1);
    if(!isnum(tmp) && !isflo(tmp)) break;
    switch(type) {
    case GLXBVL_CHAR:   val.c[cnt++] = (char) fvalue(tmp); break;
    case GLXBVL_DOUBLE: val.d[cnt++] = (double) fvalue(tmp); break;
    case GLXBVL_FLOAT:  val.f[cnt++] = (float) fvalue(tmp); break;
    case GLXBVL_SHORT:  val.s[cnt++] = (short) fvalue(tmp); break;
    }
  }
  if(!num) return cnt;
  if(cnt >= num) return 1;
  return 0;
}
    
#endif
