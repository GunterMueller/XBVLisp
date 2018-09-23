/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	indinit.c	pour reconfigurer rapidement vlisp
 *			par Patrick Greussay  14 Novembre 1982
 */

#include <vlisp.h>

  /* Ernesto: on change la memoire lisp pour eviter le clash entre entiers et
     flotants : | entiers | flotants | chaines | atomes | listes | pile | Objets Open GL */

#if (OPEN_GL | MESA_GL)
#include <glXbvl.h>
#define GL_MEMORY (GL_OBJECT_MEMORY_SIZE * GL_OBJECT_SIZE)
#else
#define GL_MEMORY 0
#endif

indinit(){
#ifndef NO_VFLO
	bnum= (CHAR*)calloc(1, TNUM+40+TFLO+TSTR+40+TAT+TLST+40+TPIL+40+GL_MEMORY);
	init(TNUM,TFLO,TSTR,TAT,TLST, TPIL);
#else
	bnum=calloc(1, TNUM+TSTR+40+TAT+TLST +40+TPIL+40+GL_MEMORY);
	init(TNUM,TSTR,TAT,TLST ,TPIL);
#endif
}
