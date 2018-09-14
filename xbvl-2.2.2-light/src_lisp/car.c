/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *                              Harald Wertz & Olivier Blanc, Avril 90 
 *                              Ernesto Sendoya, Fevrier 91
 *                              
 */

#include <vlisp.h>

#undef DDBBUUGG

Int *CAR(x)
     Int *x;
{
#ifdef DDBBUUGG
  printf("%x du CAR, son contenu: %x\n", x, *x);
#endif
#ifndef NOANNOT
  if(!isnum(*x)  &&  (((UInt)*x) & (UInt)ANNOT))
    {/* wst("\nle_CAR"); */
      return (Int *)(((UInt)*x) & (UInt)~ANNOT);
    }
  else 
#endif
    return(x);
}

Int *CDR(x)
     Int *x;
{
#ifdef DDBBUUGG
  printf("%x du CDR, son contenu: %x\n", x+1, *(x+1));
#endif
#ifndef NOANNOT
  if(!isnum(*(x+1)) && (((UInt)*(x+1)) & (UInt)ANNOT))
    {/* wst("\nle_CDR"); */
      return (Int *)(((UInt)*(x+1)) & (UInt)~ANNOT);}
  else
#endif
    return(x+1);
}

Int *car(x)
     Int *x;
{
#ifdef DDBBUUGG
  printf("%x du car, son contenu: %x\n", x, *x);
#endif
#ifndef NOANNOT
  if(!isnum(*x)  &&  (((UInt)*x) & (UInt)ANNOT))
    {
      A1= (Int *) (((UInt)*x) & (UInt)~ANNOT);return (Int *)(*A1);}
  else 
#endif
    return (Int *)(*x);}

Int *cdr(x)
     Int *x;
{
#ifdef DDBBUUGG
  printf("%x du cdr, son contenu: %x\n", x+1, *(x+1));
#endif
#ifndef NOANNOT
  if(!isnum(*(x+1))  && (((UInt)*(x+1)) & (UInt)ANNOT))
    {
      A1= (Int *)(((UInt)*(x+1)) & (UInt)~ANNOT);return (Int *)(*A1);}
  else 
#endif
    return (Int *)(*(x+1));
}
     
