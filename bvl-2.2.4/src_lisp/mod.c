/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	mod.c		fonctions de modifications
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 23 Fevrier 83
 */

#include <vlisp.h>











Int monit_1(){*atcareful=(Int)t;pop_cast(a4,(Int *));pop_cast(a3,(Int *));pop_cast(a2,(Int *));pop_cast(a1,(Int *));derec;}

Int monit(){/* a1 = nom de la variable */
    Int *auxa1;
    if(*atcareful != (Int) nil){ /* if(*atcareful == t){ si == 1 ca boucle fix.ER */
	if(isnum(a1) || isstr(a1)){derec;}
	auxa1=find((Int)atentree,(Int *)((struct atome *)a1)->ival);
	if(auxa1==0) {derec;}
        push(a1);push(a2);push(a3);push(a4);
	*atcareful=(Int)nil;a1=car(auxa1);a1=cdr(a1);a1=car(a1);
	rec(eval,monit_1);
	return ;
      }
    derec; /* fix.ER */
  }

Int nconc(){/* (nconc e1 e2)  subr 2 */
	if(isnotlist(a1))a1=(isnil(a1)) ? a2 : cons(a1,a2);
	else{
	  a3=a1; while(islist(cdr(a3)))a3=cdr(a3);
	  *CDR(a3)=(Int)((islist(a2)) ? a2 : (isnil(a2) ? nil : ncons(a2)));}
	if (wherefrom==1) return(0); else derec;
}

Int setq_11(){pop_cast(a1,(Int *));if(islist(a2)){a1=a2;pc=setq;} else popf;}

Int setq(){/* (setq v1 e1 ... vN eN)  fsubr */
	push(car(a1)); a1=cdr(a1); push(cdr(a1)); rec(evca1,setq_1);}

Int setq_1(){pop_cast(a2,(Int *)); pop_cast(a3,(Int *)); ((struct atome *)a3)->cval=a1;
	 if(*atcareful==(Int) nil) {if(islist(a2)){a1=a2; pc=setq;}else popf;}
	 else {if(infname() != (Int) nil) 
		 {Int *tmp = find(20,((struct atome *)a3)->ival);
		  if(tmp != 0) {tmp=car(tmp);tmp=cdr(tmp);*CAR(tmp)=infname();}
		  else {tmp=cons(cons((Int*)20,ncons((Int*)infname())),((struct atome *)a3)->ival);
			((struct atome *)a3)->ival=tmp;}}
	       push(a1);a1=a3;rec(monit,setq_11);}}

Int set(){/* (set e1 e2)  subr 2 */
	*CAR(a1)=(Int)a2; a1=a2; derec;}

Int rplaca(){/* (rplaca e1 e2)  subr 2 */
	*CAR(a1)=(Int)a2; derec;}

Int rplacd(){/* (rplacd e1 e2)  subr 2 */
	*CDR(a1)=(Int)a2; derec;}

Int rplacb(){/* (rplacb e1 e2)  subr 2 */
	*CAR(a1)=(Int)car(a2); *CDR(a1)=(Int)cdr(a2); derec;}

Int nextl_1(){pop_cast(a1,(Int *));derec;}

Int nextl(){/* (nextl v)  fsubr */
	a2=a1;
	a2=car(a2);a1=((struct atome *)a2)->cval;a1=car(a1);
	((struct atome *)a2)->cval=cdr((Int *)((struct atome *)a2)->cval);
	if(*atcareful==(Int)nil) {derec;}
	else {push(a1);a1=a2;rec(monit,nextl_1);}}

Int newl(){/* (newl v e)  fsubr */
	push(car(a1)); a1=cdr(a1); rec(evca1,newl_1);}

Int   newl_2(){pop_cast(a1,(Int *));derec;}

Int   newl_1(){
	pop_cast(a2,(Int *)); ((struct atome *)a2)->cval=cons(a1, (Int *)((struct atome *)a2)->cval); 
	if(*atcareful==(Int) nil) {derec;}
	else {push(a1);a1=a2;rec(monit,newl_2);}}

Int attach(){/* (attach e l)  subr 2 */
	*CDR(a2)=(Int)cons(car(a2),cdr(a2)); 
	*CAR(a2)=(Int)a1;
	a1=a2; derec;
}

Int nreverse(){/* (nreverse l r)  subr 2 */
	while(islist(a1))
	{
	  a3=cdr(a1); /*cdr(a1)*/ *CDR(a1)=(Int)a2; a2=a1; a1=a3;
	}
	a1=a2; derec;
}

Int rplac(){/*  (rplac e1 e2 e3)  subr 3 */
	*CAR(a1)=(Int)a2;*CDR(a1)=(Int)a3; derec;
}

Int deset(){/*  (deset arbre e)  subr 2 */
	if(isnil(a1)){a1=t; derec;}
	if(isatom(a1)){((struct atome *)a1)->cval=a2; a1=t; derec;}
	push(cdr(a1)); push(cdr(a2)); a1=car(a1); a2=car(a2);
	rec(deset,deset_1);
}

Int   deset_1(){
	pop_cast(a2,(Int *)); pop_cast(a1,(Int *)); pc=deset;
  }

Int smash(){/* (smash l) subr 1 */
	*CAR(a1)=(Int)car(cdr(a1));
	*CDR(a1)=(Int)cdr(cdr(a1));
	derec;
}

Int cassq(){/*  (cassq at l)  subr 2 */
	while(islist(a2))
	{
	  if(car((Int *)((struct atome *)a2)->cval)==a1){a1=cdr((Int *)((struct atome *)a2)->cval); derec;}
	  a2=cdr(a2);
	}
	a1=nil; derec;
}

Int setcassq(){/* (setcassq at val l)  subr 3 */
	while(islist(a3))
	{
	  if(car((Int *)((struct atome *)a3)->cval)==a1)
		{*CDR((Int *)((struct atome *)a3)->cval)=(Int)a2; a1=((struct atome *)a3)->cval; derec;}
	  a3=cdr(a3);
	}
	a1=nil; derec;
}

