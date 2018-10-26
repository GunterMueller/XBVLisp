/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	err.c		pour le traitement d'erreurs en vlisp
 *			Patrick Greussay   27 Oct 82
 *                      Harald Wertz  Janvier 83
 */

	/* ATTENTION: 4 blancs dans les chaines sur les machines 32 bits */

#include <vlisp.h>

/**************************************************************\
* 							       *
*   initerr(x) Int *x;{					       *
* 	char *y;					       *
* 	newppn();					       *
* 	y= (char *)*x;					       *
* 	while(*ppn++ = *y++);				       *
* 	*x=oppn; int_p1=oppn; *int_p1=ppn-oppn-sizeof(Int);    *
*   }							       *
* 							       *
\**************************************************************/

Int enable(){/*  (enable nom fonc)  fsubr */
	a2=car(a1); a1=cdr(a1); a1=car(a1);
	push(a2); rec(eval,enabl_1);
}

Int enabl_1(){
  a3=a1; pop_cast(a1,(Int *)); a2=aterror; pc=addprop;
}

Int disable(){/*  (disable nom)  fsubr */
	a1=car(a1); a2=aterror; pc=remprop;
}

Int erindvar(){/* (error-ubv v)  subr 1 */
	a1=cons(atubv,cons((Int *)Indvar,ncons(a1)));
	pc=error;
}

Int erindfun(){/* (error-ubf f args)   subr 2 */
	a1=cons(atubf,cons((Int *)Indfun,cons(a1,cons(a2,cons(a4,ncons(a5))))));
	pc=error;
}

#ifndef NO_ERROR_TYPE_CHECKING

Int ertyparg(){/* (error-type f args) subr2 */
  switch(ATOME_FTYP(a4)) {
  case SUBR0: a1=cons(attyp,cons((Int *)Errtyp,ncons(a4))); break;
  case SUBR2: a1=cons(attyp,cons((Int *)Errtyp,cons(a4, cons(a1, ncons(a2))))); break; 
  case SUBR3: a1=cons(attyp,cons((Int *)Errtyp,cons(a4, cons(a1,cons(a2,ncons(a3)))))); break;
  default: a1=cons(attyp,cons((Int *)Errtyp,cons(a4, ncons(a1))));
  }
  pc=error;
}

/* check error est lancé avant l'évaluation de la fonction
   proprement dite, les arguments ont été évalués et le numéro
   du type d'erreur à vérifier est sur le haut de la pile
*/
Int check_error()
{
  if(ATOME_ERR(*p) & ERR_ARG2) push(a2); 
  else if(ATOME_ERR(*p) & ERR_ARG3) push(a3); 
  else push(a1); 
  pc=check_arg;
}

Int check_arg()
{
  register Int *data, *func;
  pop_cast(data, (Int *)); /* la donnée à vérifier */
  pop_cast(a4,(Int *)); /* le type de vérification */
  if((ATOME_ERR_NUM(a4) == ERR_NONS  && (isnum(data) || isstr(data))) ||
     (ATOME_ERR_NUM(a4) == ERR_NONL  && (isnum(data) || islist(data))) ||
     (ATOME_ERR_NUM(a4) == ERR_ATOME && !isatom(data)) ||
     (ATOME_ERR_NUM(a4) == ERR_NOZERO && data == 0) ||
     (ATOME_ERR_NUM(a4) == ERR_NONUM && isnum(data)) ||
     (ATOME_ERR_NUM(a4) == ERR_NOSTR && isstr(data))) {
    pc=ertyparg;
  }
  else { popf; }
}
    
#endif    

Int erlec(){/* --- provisoire */
	a1=cons(atread,ncons((Int *)Errlec)); pc=error;
}

Int erpil(){/* --- provisoire, y'a quelque chose qui ne marche pas dans
	   le (enable error-pile ftn) */
	a2= (Int *)*(pbind-2);a2= (Int *)*(a2+2); /* pour avoir la derniere fonction */
	a1=cons(atpil,cons((Int *)Errpil,ncons(a2))); pc=error;
}

Int erself(){
	a2=car(a2);
	a1=cons(atself,cons((Int *)Errself,cons(a2,cons(a1,ncons(a4))))); 
	pc=error;
}

Int erexit(){/* --- provisoire */
	pbind=(Int *)0;p=(Int *)bpil;
	a1=cons(at_exit,ncons((Int *)Errexit)); pc=error;
}

Int ERRor(){pop(dski);
	derec;}

Int error(){/*  (error nom chaine . args)  subr n */
	push(a1); a1=car(a1); a2=aterror; gett(); pop_cast(a1,(Int *));
	if(isnotnil(a4))
	{ static Int *a5;
	  a2=a1; a5=cdr(a1);
	  a2=cons(car(a1),cdr(a5));
	  a1=car(a4);
	  if(car(a2) != atpil) 
		if(dski==0) pc=apply;
		else {push(dski);dski=0;rec(apply,ERRor);}
	  else
	  {
	    p=pbind; hpil+=30; 
	    rec(apply,error_0); 
	  }
	}
	else
	{
	  a2=car(a1); 
	  a3= (Int *)*(a1+1);a3= (Int *)*(a3); a1= (Int *)*(a1+1);a1=cdr(a1);
	  pushf(error_1); push(a1); a1=a3; rec(prstr,popa1);
	}
}

Int   error_0(){
	hpil-=30; pc=reent;
  }

Int   error_1(){
	if(islist(a1))
	{
	  pushf(error_1); push(cdr(a1)); a1=car(a1); rec(prini,popa1);
	}
	else rec(terpri,reent);
  }

#ifndef NO_VFLO
Int onflo(){/* pour trapper les debordements flottants */
	wst("debordement flottant\n");
	longjmp(savej,0);
}
#endif
