/*
 * Definitions des fonctions d'interface entre la librairie de fonts
 * de caracteres Hershey et gl-xbvl...
 */
#if (OPEN_GL | MESA_GL)

#include "vlisp.h"
#include "glXbvl.h"
/* #include <device.h> */
#include <math.h>


/* fixe le font */
Int glhfont() /* (GLfont name) SUBR1 */
{
  char *f_name;
  
  if(isatom(a1))     f_name = STR_FROM_STRLSP(((ATOMEPTR) a1)->pn);
  else if(isstr(a1)) f_name = STR_FROM_STRLSP(a1);
  else {a1=nil; derec;}
  a1=t;
  hfont(f_name);
  derec;
}

/* fixe le path de recherche des font */
Int glhpath() /* (GLfontpath path) SUBR1 */
{
  char *f_name;
  
  if(isatom(a1))     f_name = STR_FROM_STRLSP(((ATOMEPTR) a1)->pn);
  else if(isstr(a1)) f_name = STR_FROM_STRLSP(a1);
  else {a1=nil; derec;}
  a1=t;
  hsetpath(f_name);
  derec;
}

/* positionne aux coordonnees donnees dans a1 */
static Int moveToa1()
{
  short data[3];
  
  if(!getNumArgFroma1((char *) data,3,GLXBVL_SHORT)) return 0;
  glPushMatrix();
  glTranslatef((float) data[0], (float) data[1], (float) data[2]);
  return 1;
}

/* dessine un caractere */
Int glhdrawchar() /* (GLchar x y z char) */
{
  char c = 0;
  if(!wobj || !moveToa1()) {a1=nil; derec;}
  a1=car(a1);
  if(isatom(a1))     c = *((((ATOMEPTR) a1)->pn)+(2/2));
  else if(isstr(a1)) c = *(a1+(2/2));
  a1=nil;
  if(c) hdrawchar(c), a1=t;
  glPopMatrix();
  derec;
}

/* dessine une chaine de caracteres */
Int glhstr() /* (GLstr x y z str) NSUBR */
{
  char *str = NULL;
  if(!wobj || !moveToa1()) {a1=nil; derec;}
  a1=car(a1);
  if(isatom(a1))     str = STR_FROM_STRLSP(((ATOMEPTR) a1)->pn);
  else if(isstr(a1)) str = STR_FROM_STRLSP(a1);
  a1=nil;
  if(str) hcharstr(str), a1=t;
  glPopMatrix();
  derec;
}
 
/* dessine une chaine incluse dans une boite */
Int glhboxtext() /* (GLboxtext x y w h str) NSUBR */
{
  char *str;
  double data[4];
  if(!wobj || !getNumArgFroma1(data,4,GLXBVL_DOUBLE)) {a1=nil; derec;}
  a1=car(a1);
  if(isatom(a1))     str = STR_FROM_STRLSP(((ATOMEPTR) a1)->pn);
  else if(isstr(a1)) str = STR_FROM_STRLSP(a1);
  else {a1=nil; derec;}
  a1=t;
  hboxtext(data[0], data[1],  data[2],  data[3], (char *) str);
  derec;
}  

/* fixe l'echelle du text pour qu'il soit inclus dans une boite */
Int glhboxfit() /* (GLboxfit w h n) SUBR3 */
{
  if(!wobj) derec;
  hboxfit( valflo(a1), valflo(a2),valnb(a3));
  a1=t;
  derec;
}

/* fixe l'angle d'impression des caracteres */
Int glhtextangl() /* (GLtextangl a) SUBR 1 */
{
  if(!wobj) derec;
  htextang(valflo(a1));
  a1=t;
  derec;
}

/* taille des caracteres */
Int glhtextsize() /* (GLtextsize w h [ratio]) SUBR3 */
{
  Int r = (a3 != nil) ? valnb(a3) : 1;
  if(!wobj) derec;
  htextsize(valnb(a1) / (float) r, valnb(a2) / (float) r);
  a1=t;
  derec;
}

/* text de taille fixe ... */
Int glhfixedwidth() /* (GLfixedwidth t|nil) SUBR1 */
{
  if(!wobj) derec;
  hfixedwidth(a1==t);
  a1=t;
  derec;
}

Int glhcentertext() /* (GLcentertext t|nil) SUBR1 */
{
  if(!wobj) derec;
  hcentertext(a1==t);
  a1=t;
  derec;
}

Int glhrigthjustify() /* (GLrigthjustify t|nil) SUBR1 */
{
  if(!wobj) derec;
  hrightjustify(a1==t);
  a1=t;
  derec;
}

Int glhleftjustify() /* (GLleftjustify t|nil) SUBR1 */
{
  if(!wobj) derec;
  hleftjustify(a1==t);
  a1=t;
  derec;
}
#endif
