/* %i%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	flo.c		flottants pour vlisp
 *			par Patrick Greussay  Novembre 1982
 */

#include <vlisp.h>




#ifndef NO_VFLO

double valflo (x) Int *x;{
#if 0
	float *xx; float f; double d;
	xx=x; f= *xx; d=(double)f;
	return(d);
#endif
	if(isint(x) || (Int)x > (Int)hflo) return (double) valnb(x);
	return (double) * (float *) x;
}

Int *craflo(f) double f;{
	Int *x; float xxx; float *xx;

	if(!freeflo)gci();
	xxx=(float)f;
	x=freeflo; freeflo=car(freeflo); *(x+2)=0; xx=(float*)x; *xx=xxx;
	return(x);
}

Int *crasysflo(i, f) Int i; double f; {
  Int *x = (Int*)(bflo + i);
  *x = f;
  return x;
}

Int prflo(){
	CHAR *pch,*pch2;
	sprintf(ppn,"%g",valflo(a1));
	pch=index(ppn,'\0');
	if(*ppn=='.' || (*ppn == '-' && *(ppn+1)== '.'))
	{
	  for(pch2=pch; pch2 >= ppn; pch2--)*(pch2+1)= *pch2;
	  if(*ppn=='-')*(ppn+1)='0'; else *ppn='0';
	  ++pch;
	}
	else
	if(!index(ppn,'.')){*pch++ = '.'; *pch++ = '0'; *pch='\0';}

	while(pch > ppn)
	{
	  na1= *--pch;
	  pushf(pvch); push(na1); pushf(popa1);
	}
	derec;
}

Int veriflo(){/* pour savoir si chaine pointee par pchar (globale) est flo */
	CHAR *pp; Int seechif=0;
	pp=pchar;
			/* voyons d'abord le signe */
	if(*pp == '-')pp++;
	if(*pp == 0)return(0);
			/* voyons ensuite les chiffres */
	while(*pp >= '0' && *pp <= '9'){pp++; seechif=1;}
	if(*pp == 0)return(0);
			/* apres il faut un '.' ou 'e' */
	if(*pp == '.')
	{
	  pp++; while(*pp >= '0' && *pp <= '9'){pp++; seechif=1;}
	  if(*pp == 0)return(seechif ? 1 : 0);
			/* alors peut-etre c'est un exposant */
	}
	if(*pp == 'e')
	{
	  pp++;
	  if(*pp == '-' || *pp == '+')pp++;
	  while(*pp >= '0' && *pp <= '9')pp++;
	  if(*pp == 0 && seechif)return(1);
	}
	return(0);
}

#endif
