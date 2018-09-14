/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	num.c		fonctions numeriques
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 23 Fev 83
 */

#include <vlisp.h>

#ifndef NO_VFLO
		/* pour placer ce qu'il faut dans les accus */
Int conver(){
	if(isflo(a1))
	{
	  fa1=valflo(a1);
	  if(isnil(a2)) /* bon nbr. d'arguments ? */
		a2 = 0;
	  else 
	  	fa2=(isnotflo(a2)) ? (double)valnb(a2) : valflo(a2);
	  return(0);
	}
	if(isflo(a2))
	{
	  fa2=valflo(a2);
	  fa1=(isnotflo(a1)) ? (double)valnb(a1) : valflo(a1);
	  return(0);
	}
	/* control de nbr. incorrect d'arguments, Ernesto */
	if(isnil(a1)) na1 = 0;
	else na1=valnb(a1); 
	if(isnil(a2)) na2 = 0;
	else na2=valnb(a2); return(1);
}

#endif

#if 0
window(){/* (window n) subr 1 */
	a1=valnb(a1);
	if(a1<2 || a1>20) a1=nil;
	else {DEBWIN=a1;a1=cranb(a1);
	      if(LIGNE<=DEBWIN){a2=1;pc=poscur;return;}}
	derec;}

ligne(){/* (ligne) subr 0 */
	a1=cranb(LIGNE);derec;}
#endif

Int add1(){/* (add1 n) subr 1 */
	a1=cranb(valnb(a1)+1); derec;}

Int plus(){/* (+ n1 n2) subr 2 */

#ifndef NO_VFLO
	a1= conver() ? cranb(na1+na2) : craflo(fa1+fa2);
#else
	a1=cranb(valnb(a1)+valnb(a2));
#endif
	derec;
}

Int power(){/* (** n1 n2)  subr 2 */
	double pow(); Int ipow();
#ifndef NO_VFLO
	a1= conver() ? cranb(ipow(na1,na2)) : craflo(pow(fa1,fa2));
#else
	a1=cranb(ipow(na1,na2));
#endif
	derec;
}

Int ipow(x,n)
     Int x, n;
{
	register i,pi = 1;
	for(i=1; i<=n; ++i)pi*=x;
	return(pi);
}

Int Abs(){/*  (abs n)  subr 1 */
#ifndef NO_VFLO
	a1= conver() ? cranb((na1<0) ? -na1 : na1) :
		       craflo((fa1<0) ? -fa1 : fa1);
#else
	a1=cranb((na1<0)? -na1 : na1);
#endif
	derec;
}

Int sub1(){/* (1- n) subr 1 */
	a1=cranb(valnb(a1)-1); derec;}

Int differ(){/* (- n1 n2) subr 2 */
	if(isnil(a2)){a2=a1; a1=0;}
#ifndef NO_VFLO
	a1= conver() ? cranb(na1-na2) : craflo(fa1-fa2);
#else
	a1=cranb(valnb(a1)-valnb(a2));
#endif
	derec;
}

Int eq(){/* (eq e1 e2) subr 2 */
	if(isnum(a1) && isnum(a2))
#ifndef NO_VFLO
	  a1 = conver() ? (na1==na2 ? t : nil) : (fa1==fa2 ? t : nil);
#else
	  a1=(valnb(a1) == valnb(a2)) ? t : nil;
#endif
	else
	a1=(a1 == a2) ? t : nil;
	derec;
}

Int zerop(){ /* (zerop n) subr 1 */
	a1=(valnb(a1)==0) ? 0 : nil; derec}


Int mul(){/* (* n1 n2)  subr 2 */

#ifndef NO_VFLO
	a1=(conver()) ? cranb(na1*na2) : craflo(fa1*fa2);
#else
	a1=cranb(valnb(a1) * valnb(a2));
#endif
	derec;
}

Int quo(){/* (/ n1 n2)  subr 2 */

#ifndef NO_VFLO
	a1=(conver()) ? cranb(na1 / na2) : craflo(fa1 / fa2);
#else
	a1=cranb(valnb(a1) / valnb(a2));
#endif
	derec;
}

Int rem(){/* (rem n1 n2)  subr 2 */
	a1=cranb(valnb(a1) % valnb(a2)); derec;}

Int le(){/* (le n1 n2) */
#ifndef NO_VFLO
	if(conver())a1= na1 <= na2 ? t : nil; else a1= fa1 <= fa2 ? t : nil;
#else
	a1=(valnb(a1) <= valnb(a2)) ? t : nil;
#endif
	derec;
}

Int ge(){/* (ge n1 n2)  subr 2 */
#ifndef NO_VFLO
	if(conver())a1= na1 >= na2 ? t : nil; else a1= fa1 >= fa2 ? t : nil;
#else
	a1=(valnb(a1) >= valnb(a2)) ? t : nil;
#endif
	derec;
}

Int lt(){/* (< n1 n2)  subr 2 */
#ifndef NO_VFLO
	if(conver())a1= na1 < na2 ? t : nil; else a1= fa1 < fa2 ? t : nil;
#else
	a1=(valnb(a1) < valnb(a2)) ? t : nil;
#endif
	derec;
}

Int gt(){/* (> n1 n2)   subr 2 */
#ifndef NO_VFLO
	if(conver())a1= na1 > na2 ? t : nil; else a1= fa1 > fa2 ? t : nil;
#else
	a1=(valnb(a1) > valnb(a2)) ? t : nil;
#endif
	derec;
}

Int neq(){/* (neq n1 n2)  subr 2 */
	rec(eq,nullp);}

Int incr(){/* (incr v e)  fsubr */
	a2=car((Int *)((struct atome *)a1)->pval); a1=car(a1);
	if(isnil(a2)){na1=1; incdec();}
	else
	{push(a1); a1=a2; rec(eval,incr_1);}
}

Int   incr_1(){na1=valnb(a1); pop_cast(a1,(Int *)); incdec();}


Int decr(){/* (decr v e)  fsubr */
	a2=car((Int *)((struct atome *)a1)->pval); a1=car(a1);
	if(isnil(a2)){na1= -1; incdec();}
	else
	{push(a1); a1=a2; rec(eval,decr_1);}
      }

Int   decr_1(){na1= - valnb(a1); pop_cast(a1,(Int *)); incdec();}

Int   incdec_1(){a1=a4;derec;}

Int   incdec(){/* commune a incr et decr */
	a4=cranb(valnb(car(a1))+na1); *CAR(a1)=(Int)a4; 
	if(*atcareful==(Int)nil){a1=a4; derec;}
	rec(monit,incdec_1);}

Int plnum(){ /* longueur d'un nombre */
	Int i,j;
	j=valnb(a1);
	for(i=0;j=j/10;i++);
	a1=cranb(++i);
}

Int plength(){/* (plength at) subr1 */
	if(isnum(a1)) plnum();
	else
	{
	  if(isatom(a1))a1=((struct atome *)a1)->pn;
	  a1=cranb(*a1 - 1);
	}
	derec;
}

#ifndef NO_VFLO

/* un apport de Ernesto pour traiter correctement tant les entiers que
   les flotants dans les opperations aritmetiques de bvl */

#define valeurn(a)  (isflo(a) ? valflo(a) : valnb(a))

Int pfix(){/* (fix n)  subr1 */
	a1=cranb((Int)valeurn(a1)); derec;
}

Int pfloat(){/* (float n)  subr1 */
	a1=craflo((double)valeurn(a1)); derec;
}

Int pfixp(){/* (fixp n)  subr1 */
	a1=isnotflo(a1) ? t : nil;
	derec;
}

Int pfloatp(){/* (floatp n)  subr1 */
	a1=isflo(a1) ? t : nil;
	derec;
}

Int sqrtp(){/* (sqrt n)  subr 1 */
	a1 = craflo(sqrt(valeurn(a1)));
	derec;
}

Int expp(){/* (exp n)  subr 1 */
	double exp();
	a1=craflo(exp(valeurn(a1))); derec;
}

Int logp(){/* (log n)  subr 1 */
	double log();
	a1=craflo(log(valeurn(a1))); derec;
}

Int sinp(){/* (sin n)  subr 1 */
  a1=craflo(sin(valeurn(a1))); derec;
}

Int cosp(){/* (cos n)  subr 1 */
  a1=craflo(cos(valeurn(a1))); derec;
}

Int asinp(){ /* (asin n) subr 1 */
  a1=craflo(asin(valeurn(a1))); derec;
}

Int acosp(){ /* (acos n) subr 1 */
  a1=craflo(acos(valeurn(a1))); derec;
}

Int atanp(){ /* (atan n) subr 1 */
  a1=craflo(atan(valeurn(a1))); derec;
}

Int atan2p() {/* (atan2 x y) subr 2 */
  a1=craflo(atan2(valeurn(a1), valeurn(a2))); derec;
}

#undef valeurn
#endif

Int lognot(){/*  (lognot n)  subr 1 */
	a1=cranb(~valnb(a1)); derec;
}

Int logand(){/*  (logand n1 n2)  subr 2 */
	a1=cranb(valnb(a1) & valnb(a2)); derec;
}

Int logor(){/*  (logor n1 n2)  subr 2 */
	a1=cranb(valnb(a1) | valnb(a2)); derec;
}

Int logxor(){/*  (logxor n1 n2)  subr 2 */
	a1=cranb(valnb(a1) ^ valnb(a2)); derec;
}

Int logshift(){/*  (logshift n1 n2)  subr 2 */
	na1=valnb(a1); na2=valnb(a2);
	if(na2 < 0)na1>>= -na2; else na1<<= na2;
	a1=cranb(na1); derec;
}
