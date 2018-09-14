/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	strings.c	trucs de strings
 *			Harald Wertz, Patrick Sinz   Avril 82
 *			Patrick Greussay  29 Oct 82
 */

#include <vlisp.h>

#if 0
/* fonctions  */
Int comstr1(void);
Int finstr(void);
Int newppn(void);
void genstring(CHAR *);
void finstr_normal(void);

#ifndef STDC_HEADERS
Int comstr1(void);
Int finstr(void);
Int newppn(void);
void genstring(CHAR *);
void finstr_normal(void);
#endif
#endif

Int argcp(){/* (argc) subr 0 */
	a1=cranb(vlargc); derec;
}

Int userp(){/* (user) subr 0 */
	pchar=(CHAR *)getlogin(); comstr1();
}

Int ttyp(){/* (tty) subr 0 */
	pchar=(CHAR *)ttyname(2); comstr1();
}

Int getenp(){/* (getenv str) subr 1 */
	if(isatom(a1))a1=((struct atome *)a1)->pn;
	pchar=(CHAR *)getenv(a1+(2/2)); comstr1();
}

Int argvp(){/* (argv n) subr 1 */
	pchar=(CHAR *)vlargv[(isnil(a1)) ? 0 : valnb(a1)]; comstr1();
}

Int strcar(){/* (strcar e n) subr 2 */
	 /* ramene les n premier caractere d'une chaine */
	na1= (isnil(a2)) ? 1 : valnb(a2);
	if(isatom(a1))a1=((struct atome *)a1)->pn;
	pchar=STR_FROM_STRLSP(a1);	newppn();
	while(na1-- > 0)
	{
	  if ((*ppn++ = *pchar++) == 0){ppn--; break;}
	  if(ppn>hstr)gcstrsv();
	}
	*ppn++ = 0;
	finstr();
}

Int strcdrp(){/* (strcdr e n) subr 2 */
	 /* ramene la chaine sans les n premier element */
	na1= (isnil(a2)) ? 1 : valnb(a2);
	if(isatom(a1))a1=((struct atome *)a1)->pn;
	pchar=STR_FROM_STRLSP(a1); pchar++;
	while (--na1 && *pchar++);
	if(na1)pchar--;
	newppn();
	while(*ppn++ = *pchar++)if(ppn>hstr)gcstrsv();
	finstr();
}

Int strcatp_1(i)
Int i;           /* appelle de strcat ou reccursion */
{
  for(;;)
    {
      a2=car(a1);
make_strcat:
      if(isnum(a2))
	{
	  if(isflo(a2))
	    {
	      double fv=valflo(a2);
	      if(fv<0.0) {*ppn++ = '-'; fv=-fv;}
	      na1=(Int)fv; gsynum(); 
	      *ppn++='.';
	      na1=(Int)((fv - (Int) fv)*100000); /* six digits */
	      while(na1 && (na1/(Int)10)*10 == na1) na1 /=10; 
	      gsynum(); *ppn++=0;
	    }
	  else 
	    {
	      na1=valnb(a2); if(na1<0){*ppn++ = '-'; na1 = -na1;}
	      gsynum(); *ppn++ = 0;
	    }
	}
      else if(isatom(a2) || isstr(a2))
	{
	  if(isatom(a2)) a2=((struct atome *)a2)->pn;
	  pchar=STR_FROM_STRLSP(a2);
	  while(*ppn++ = *pchar++)if(ppn>hstr)gcstrsv();
	}
      else if(islist(a2)) 
	{
	  push(a1);
	  a1=a2;
	  *ppn++='(';
	  strcatp_1(1);
	  *(ppn - 1)=')';
	  *ppn=0;
	  ++ppn;
	  pop_cast(a1,(Int *));
	}
      if(a1 == a2 || cdr(a1) == nil) break;
      a1=cdr(a1); 
      ppn--; if(i) *ppn++ = ' ';
      if(isnotlist(a1)) 
	{ *ppn++ = '.'; *ppn++ = ' '; a2 = a1; goto make_strcat; }
    }
}

Int strlenp()
{/* (strlen str) subr1 */
  if(isstr(a1)) 
    a1 = cranb(strlen(STR_FROM_STRLSP(a1)));
  else a1 = nil;
  derec;
}

Int strcatp()
{/* (strcat str1 str2 ... strN)   nsubr */

  newppn();
  strcatp_1(0);
  finstr();
}

Int strcmpp(){/* (strcmp e1 e2)  subr 2 */
	if(isatom(a1))a1=((struct atome *)a1)->pn; if(isatom(a2))a2=((struct atome *)a2)->pn;
	a1=cranb(strcmp((CHAR *)(a1+(2/2)),(CHAR *)(a2+(2/2))));
	derec;
}

Int dupl(){/* (dupl e n)  subr 2 */
	newppn();
	na1=valnb(a2);
	if(na1 <= 0)derec;
	if(isatom(a1))a1=((struct atome *)a1)->pn; pchar=STR_FROM_STRLSP(a1);
	while(na1>0)
	{
	  pchar2=pchar; while(*ppn++ = *pchar2++)if(ppn>hstr)gcstrsv();
	  ppn--; na1--;
	}
	++ppn;
	finstr();
}

Int comstr1(){/* suffixe de userp() */
	if(pchar==0){a1=nil; derec;}
	newppn();
	while(*ppn++ = *pchar++)if(ppn > hstr)gcstrsv();
	finstr();
}

Int finstr(){/* pour terminer une allocation de chaine */
	a1=(Int *)oppn; lpn=ppn-oppn-sizeof(Int); *a1=lpn; derec;
}

Int newppn(){/* prepare un ppn bien aligne */
	aux1=(Int)ppn; aligner(aux1); ppn=(CHAR *)aux1; oppn=ppn; ppn+=sizeof(Int);
}

Int stringi(){/* (strincp str1 str2) subr 2 */
#if 0
	CHAR str1[80],str2[80], *ii, *jj; Int i;
	if(isatom(a2)) a2=((struct atome *)a2)->pn;jj= &str2[0];
	strcpy(jj,(CHAR *)(a2 + 2/2));
	if(isatom(a1)) a1=((struct atome *)a1)->pn;jj= &str1[0];
	strcpy(jj,(CHAR *)(a1 + 2/2));
	if((ii=(index(str2,str1[0])))!=0){i=0;
		while(*ii == str1[i]){ii++;i++;}
		if(i>=strlen(jj)) a1=t; else a1=nil;}
	else a1=nil;
#endif
	CHAR *str1, *str2, *ii;
	if(isatom(a1)) a1 = ((struct atome *)a1)->pn;	str1 = (CHAR *) (a1+(2/2));
	if(isatom(a2)) a2 = ((struct atome *)a2)->pn;	str2 = (CHAR *) (a2+(2/2));
	while((ii = index(str2, (Int) *str1))!= 0) {
	  if(!strncmp(ii,str1,strlen(str1))) { a1=cranb((Int) (ii-str2)); derec; }
	  str2 = ii+1;
	}
	a1=nil;
	derec;
}

Int bufstr(){/* (bufstr)   subr 0 */
	CHAR *pp;
	pp=bufou;
	newppn();
	bufou[oubpt++]=0;
	while(*ppn++ = *pp++)if(ppn>hstr)gcstrsv();
	iniott();
	finstr();
}

/* 
 * positionne une chaine passee an argument  en a1.
 */
void genstring(str)
     CHAR *str;
{
  if(str){
    newstr(str);
    a1=(Int *)oppn;
  }
  else a1 = (Int *) cranb(0);
}

void newstr(str)
     CHAR *str;
{
  newppn();
  while( *ppn++ = *str++);     /* copie */
  *(Int *)oppn = ppn -oppn - sizeof( Int ); /* sauvegarde de la taille */
  if(ppn>hstr) gcstrsv();
}
/*
 *
 */

void finstr_normal(){/* pour terminer une allocation de chaine */
  a1=(Int *)oppn; 
  lpn=ppn-oppn-sizeof(Int); 
  *a1=lpn;
}

