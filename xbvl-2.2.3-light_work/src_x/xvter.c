
/* %I%, %R%, %B%, %D%,  %T%,  %M% */

/*
 *	xvter.c		vlisp multi-terminal
 *			Patrick Greussay  27 Oct 82
 *			rendu compatible termcap sur VAX, le 15 Fev 83
 *			completement remodele pour le multi-fenetrage:
 *			Harald Wertz 21. Mars 84
 *			re-amenage [hw] Mars 86
 *			Ernesto Sendoya, Oct 90	interface X systeme 
 */





#define INTLECT	25

#include <vlisp.h>
#include <xwin.h>






























char nomf[]="bVLISP.win";




#ifdef X_USE_LATIN1
/* prise en compte des accents */
char tabch1[256]={
#else
char tabch1[128]={
#endif
  0,  0,  0, 12,   0,  0,  0, 12,	/* nul  CA  CB  CC   CD  CE  CF  bel */
  0,  9,  9,  0,   0,  9,  0,  0,	/*  bs tab  lf  CK   CL  cr  CN  CO  */
  12,  0,  0, 12,   0, 12,  0,  0,	/*  CP  CQ  CR  CS   CT  CU  CV  CW  */
  12,  0, 12,  0,  12, 12, 12, 12,	/*  CX  CY  CZ esc   Cas C]  Cfl C_  */
  9, 12, 11, 13,  12, 12, 12, 10,	/* esp   !   "   #    $   %   &    ' */
  4,  5, 12, 12,  12, 12,  8, 12,	/*  (    )   *   +    ,   -   .   sl */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  0    1   2   3    4   5   6    7 */
  12, 12, 12,  1,  12, 12, 12, 12,	/*  8    9   :   ;    <   =   >    ? */
  12, 12, 12, 12,  12, 12, 12, 12,	/* at    A   B   C    D   E   F    G */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  H    I   J   K    L   M   N    O */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  P    Q   R   S    T   U   V    W */
  12, 12, 12,  6,  12,  7, 12, 12,	/*  X    Y   Z   [  antsl ]  vfl   _ */
  10, 12, 12, 12,  12, 12, 12, 12,	/*  `    a   b   c    d   e   f    g */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  h    i   j   k    l   m   n    o */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  p    q   r   s    t   u   v    w */
  12, 12, 12, 12,  12, 12, 12, 0		/*  x    y   z   {   |   }   ~   del */
#ifdef X_USE_LATIN1
  , 0,  0,  0,  0,   0,  0,  0,  0,
  0,  0,  0,  0,   0,  0,  0,  0,
  0,  0,  0,  0,   0,  0,  0,  0,
  0,  0,  0,  0,   0,  0,  0,  0,
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,     
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,
  12, 12, 12, 12,  12, 12, 12, 12
#endif
};

  /* 
   *         lire une ligne de la fenetre courante dans bufin 
   *         ------------------------------------------------
   *   rst est la vraie interface de communication en lecture
   *   avec les widgettes utilisees comme interfaces independantes
   */

static CHAR *encore;

rst() 
{
  XEvent x_event;
  register Int x;
  Int J;
  Int MCL;

  J=0;MCL=0;
  if(encore){
    /* si retour automatique avec
       coupage de ligne "encore" sauve
       le mot trunque                    */
    wst(encore);
    XtFree(encore);
    encore = NULL;
  }
  if(DML==1)
    J=0;
  else if(MARKED!=0 ){
    (void)xgetBufin(bufin);
    if((optop&01) != 0)wst(bufin);
    return;
  }
  x = xgetch1(&x_event);
  J=0;
  DML=0;
  while( 1 ){
    if(x == CALLBACK_INPUT)
      return;
    if(J==1){
      if(MCL!=1){
	if(xgetBufin(bufin) != False)
	  xinsch(&x_event, '\n', &encore);
      }
      J = 128;
      if((optop&01) != 0)wst(bufin);
      return;
    }
    if(tabch[x]==MACL){
      a1= (Int *)x;
      J=1;MCL=1;DML=2;
    }
    else{
      /* la ligne courante du buffer d'edition 
	 fait de tampon a bufin */
      if(x != '\n'){
	if(xinsch(&x_event, x, &encore) == RETOUR_AUTO)
	 J  = 1;
      }
      else J = 1; 
    }
      if(J == 0)   x = xgetch1(&x_event);
  }
}



/*------------------------------------------------------------------------
 * fonction d'acces a la variable "toggle" qui modifie le mode d'ecriture.
 ------------------------------------------------------------------------*/
Int toggle ; 
void toggle_mode()
{
 if(toggle) toggle = 0;
 else toggle = 1;
}

