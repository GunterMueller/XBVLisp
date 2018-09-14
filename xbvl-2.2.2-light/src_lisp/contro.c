/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	contro.c		definition et controle pour vlisp
 *				Patrick Greussay, Novembre 1982
 *				Harald Wertz, Janvier 1983
 */

#include <vlisp.h>

/* fonctions de definitions */

Int de(){/* (de at vars . corps)  fsubr */
	a3=(Int *)EXPR; pc=def;}

Int df(){/* (df at vars . corps)  fsubr */
	a3= (Int *)FEXPR; pc=def;}

Int dm(){/* (dm at vars . corps)  fsubr */
	a3= (Int *)MACRO; pc=def;}

Int dmo(){/* (dmo at vars . corps)  fsubr */
	a3= (Int *)MACOUT; pc=def;}

/* FOO : probleme peut-etre avec le strgci */

Int dmc(){/* (dmc c vars . corps)  fsubr */
	a2=car(a1);
	if(isstr(a2))a3= (Int *) ((Int)*STR_FROM_STRLSP(a2));
	else {pchar=(CHAR *)(((struct atome *)a2)->pn); a3= (Int *) ((Int)*(pchar+sizeof(Int)));}
	tabch[(Int)a3]=MAC; 
	tabfval[(Int)a3].ftyp=EXPR;
	tabfval[(Int)a3].fval=cdr(a1);
	a1=car(a1);
	derec;}

Int typech(){/* (typech c v)  fsubr */
	a2=car(a1);
	if(isstr(a2))a3= (Int *) ((Int)*STR_FROM_STRLSP(a2));
	else {pchar=(CHAR *)(((struct atome *)a2)->pn); a3= (Int *) ((Int)*(pchar+sizeof(Int)));}
	a2=cdr(a1);
	if(isnil(a2)) a1=(Int *)((Int)tabch[(Int)a3]);
	else {a2=car(a2);a1=(Int *)valnb(a2);tabch[(Int)a3]=(CHAR)((Int)a1);}
	derec;}

Int redmc(){/* pour re-activer le macro-caractere */
	/* (redmc x) fsubr */
	a2=car(a1);
	if(isstr(a2)) a3=(Int *) ((Int)*STR_FROM_STRLSP(a2));
	else {pchar=(CHAR *)((struct atome *)a2)->pn; a3= (Int *) ((Int)*(pchar+sizeof(Int)));}
	if(!tabfval[(Int)a3].ftyp) ;
	else if(isnil(cdr(a1))) tabch[(Int)a3]=MAC; 
	else {a2=cdr(a1);a2=car(a2);
	      if(a2==t) tabch[(Int)a3]=MACL;else tabch[(Int)a3]=valnb(a2);}
	a1=car(a1);
	derec;}

Int undmc(){/* pour enlever les definitions de macros*/
	/* (undmc "c") fsubr */
	a1=car(a1);
	if(isstr(a1))a3=(Int *) ((Int)*STR_FROM_STRLSP(a1));
	else {pchar=(CHAR *)((struct atome *)a1)->pn; a3= (Int *) ((Int)*(pchar+sizeof(Int)));}
	tabch[(Int)a3]=NOR; derec;}

Int dml(){/* (dml c (vars) . corps) fsubr */
	a2=car(a1);
	if(isstr(a1))a3=(Int *) ((Int)*STR_FROM_STRLSP(a1));
	else {pchar=(CHAR *)((struct atome *)a1)->pn; a3= (Int *) ((Int)*(pchar+sizeof(Int)));}
	tabch[(Int)a3]=MACL; 
	tabfval[(Int)a3].ftyp = EXPR;
	tabfval[(Int)a3].fval = cdr(a1);
	a1=car(a1);
	derec;}

Int def(){
	a2=cdr(a1); a1=car(a1);
	((struct atome *)a1)->oftyp=((struct atome *)a1)->ftyp; 
	((struct atome *)a1)->ofval=((struct atome *)a1)->fval;
	aux1=(Int)a3; ((struct atome *)a1)->ftyp=(char)aux1; ((struct atome *)a1)->fval=a2;
	if(*atcareful != (Int) nil){
	  func(); /* pour l'analyse */
	  a3= (Int *)find(100,((struct atome *)a1)->ival);
	  if(a3!=0){
	    a3=car(a3);a3=cdr(a3);
	    *CAR(a3)=(Int)t;}
	  else {
	    a3=cons(cons((Int *)100,ncons(t)),((struct atome *)a1)->ival);
	    ((struct atome *)a1)->ival=a3;}
	  if(infname() != (Int) nil) { /* n'est pas definie depuis l'entree standar...*/
	    a3=find(20,((struct atome *)a1)->ival);
	    if(a3!=0){
	      a3=car(a3);a3=cdr(a3);
	      *CAR(a3)=infname();}
	    else {
	      a3=cons(cons((Int *)20,ncons((Int *)infname())),((struct atome *)a1)->ival);
	      ((struct atome *)a1)->ival=a3;}}}
	derec;}

Int ival(){/* (ival at e) subr 2 */
	if(a2!=nil)((struct atome *)a1)->ival=a2;
	a1=((struct atome *)a1)->ival;
	derec;}

Int clrival(){/* (clrival at) subr 1 */
	((struct atome *)a1)->ival=nil;derec;}

Int fval(){/* (fval at e) subr 2 */
	if(a2 != nil)((struct atome *)a1)->fval=a2;
	if(isstr(a1)){
	  if(tabfval[(Int) (*(CHAR *)(a1+1))].fval) 
	    a1=tabfval[(Int) (*(CHAR *)(a1+1))].fval;
	}
	else if(!((struct atome *)a1)->fval && *((struct atome *)a1)->pn == 1) {
	  pchar=(CHAR *)((struct atome *)a1)->pn; 
	  a3=(Int *)((Int)*(pchar+sizeof(Int)));
	  if(tabch[(Int)a3] != NOR && tabfval[(Int)a3].fval) a1=tabfval[(Int)a3].fval;
	  else a1=((struct atome *)a1)->fval; 
	}
	else a1=((struct atome *)a1)->fval; derec;}

Int ftyp(){/* (ftyp at e)  subr 2 */
	if(isnotnil(a2)){aux1=(Int)a2; ((struct atome *)a1)->ftyp=(CHAR)aux1;}
	a1=(Int *)((Int)ATOME_FTYP(a1)); derec;}


Int progn(){/* (progn . l) fsubr ET (eprogn l) subr 1 */
	a2=cdr(a1);
	if(islist(a2)){pushf(progn); push(a2); pushf(popa1);}
	pc=evca1;}

Int prog1(){/* (prog1 . l)  fsubr */
	push(cdr(a1)); rec(evca1,prog1_1);}

Int   prog1_1(){
	xtopst(a1,via,a2); rec(progn,popa1);}

Int evlis(){/* (list . l) fsubr ET (evlis l) subr 1 */
	if(islist(a1)){push(cdr(a1)); rec(evca1,evlis_1);}
	else popf;}

Int   evlis_1(){xtopst(a1,via,a2); rec(evlis,evlis_2);}

Int   evlis_2(){
	pop_cast(a2,(Int *));
	a1=cons(a2,a1);
	derec;}

Int whenp(){/* (when e . esv)  fsubr */
	push(cdr(a1)); rec(evca1,when_1);
}

Int when_1(){
	pop_cast(a2,(Int *));
	if(isnotnil(a1))
	{
	  a1=a2; pc=progn;
	}
	else derec;
}

Int unlessp(){/* (unless e .esf)  fsubr */
	push(cdr(a1)); pushf(when_1); rec(evca1,nullp);
}

Int ifp(){/* (if e esv . esf)  fsubr */
	push(cdr(a1)); rec(evca1,ifp_1);}

Int ifp_1(){a2=a1; pop_cast(a1,(Int *)); pc=(isnil(a2)) ? (a1=cdr(a1),progn) : evca1;}

Int ifnp(){/* (ifn e esf . esv)  fsubr */
	push(cdr(a1)); rec(evca1,ifnp_1);}

Int ifnp_1(){a2=a1; pop_cast(a1,(Int *)); pc=(isnotnil(a2)) ? (a1=cdr(a1),progn) : evca1;}

Int cond(){/* (cond . l)  fsubr */
	a2=a1; pc=cond_1;}

Int   cond_1(){
	if(isnotlist(a2))derec;
	a1=car(a2); push(cdr(a2)); push(cdr(a1));
	rec(evca1,cond_2);}

Int   cond_2(){
	pop_cast(a3,(Int *)); pop_cast(a2,(Int *));
	if(isnil(a1)){pc=cond_1; return;}
	if(isnotlist(a3))derec;
	a1=a3; pc=progn;}

Int popa1(){pop_cast(a1,(Int *)); derec;}

Int whilep(){/* (while e . l)  fsubr */
	push(cdr(a1)); push(ncons(car(a1))); pc=while_2;}

Int   while_1(){
	if(isnil(a1)){p-=2; popf;}
	else{a1 = (Int *)*(p-1); rec(progn,while_2);}}

Int   while_2(){
	a1 = (Int *)*p; rec(evca1,while_1);}

Int until(){/* (until e . l)  fsubr */
  	push(cdr(a1)); push(ncons(car(a1))); pc=until_2;}

Int   until_1(){
	if(isnotnil(a1)){p-=2; popf;}
	else{a1 = (Int *)*(p-1); rec(progn,until_2);}}

Int   until_2(){
	a1 = (Int *)*p; rec(evca1,until_1);}

Int repeat(){/* (repeat n . l)  fsubr */
	push(cdr(a1)); rec(evca1,repea_1);}

Int   repea_1(){push(a1); pc=repea_2;}

Int   repea_2(){
	na1=valnb((Int *)*p);
	if(na1 <= 0){p-=2; popf;}
	else{*p=(Int)cranb(--na1); a1 = (Int *)*(p-1); rec(progn,repea_2);}
  }

Int or(){/* (or . l)  fsubr */
	a2=cdr(a1);
	if(islist(a2)){pushf(a2); pushf(or_1);}
	pc=evca1;}

Int   or_1(){if(isnotnil(a1)){--p; derec;}
	 pop_cast(a1,(Int *)); pc=or;}

Int and(){/* (and . l)  fsubr */
	if(islist(a1))pc=and_1; else{a1=t; derec;}}

Int   and_1(){a2=cdr(a1); if(islist(a2)){push(a2); pushf(and_2);}
	  pc=evca1;}

Int   and_2(){if(isnil(a1)){--p; derec;}
	  pop_cast(a1,(Int *)); pc=and_1;}

Int lambdap(){/* pour l'evaluation des lambda */
	a1=form; derec;}

Int selectq(){/* (selectq e c1 ... cN l)  fsubr */
	push(cdr(a1)); rec(evca1,seltq_1);}

Int   seltq_1(){a4 = (Int *)*p; *p= (Int) a1; pc=seltq_2;}

Int   seltq_2(){a3=car(a4); a4=cdr(a4);
	    if(islist(a4)){a1 = (Int *)*p; push(a4); a2=car(a3); push(cdr(a3));
			   pushf(seltq_4); pc=(islist(a2)) ? member : equal;}
	    else pc=seltq_3;}

Int   seltq_3(){--p; a1=a3; pc=progn;}

Int   seltq_4(){pop_cast(a3,(Int *)); pop_cast(a4,(Int *)); pc=(isnotnil(a1)) ? seltq_3 : seltq_2;}

Int llet(){/*  (let liaisons . e)  fsubr */
	a1=form;
	a2=car(((struct atome *)a1)->pval);
	if(isatom(car(a2)))
	{
/*	  car(a1)=cons(lambda,cdr(a1));
	  cdr(a1)=cdr(a2); cdr(a2)=nil; */
	  *CAR(a1)=(Int)cons(lambda,cdr(a1));*CDR(a1)=(Int)cdr(a2);
	  *CDR(a2)=(Int)nil;
	}
	else
	{
	  a3=car(a2); a5=cdr(a2);
	  a4=cdr(a3); /*cdr(a2)=a4;*/ *CDR(a2)=(Int)a4;
	  while(islist(a5))
	  {
	    /*cdr(a3)=*/ *CDR(a3)=(Int)car(a5); a3=cdr(a3);
	    /*cdr(a4)*/ *CDR(a4)=(Int)cdr(((struct atome *)a5)->cval); a4=cdr(a4);
	    a5=cdr(a5);
	  }
	  *CDR(a3)=(Int)nil;*CDR(a4)=(Int)nil;

	  a2=cdr(a1); a3=car(a2);
/*	  car(a1)=a3; cdr(a1)=cdr(a3);
	  car(a2)=car(a3); cdr(a3)=a2; car(a3)=lambda;*/
	  *CAR(a1)=(Int)a3;*CDR(a1)=(Int)cdr(a3);
	  *CAR(a2)=(Int)car(a3);*CDR(a3)=(Int)a2;*CAR(a3)=(Int)lambda;
	}
	pc=eval;
}

Int macexp(){/* (macroexpand e)  subr 1 */
	 /*
	  *	(de macroexpand (e)
	  *	  (ifn (= (ftyp (car e)) 9) e
	  *	       (eval [[lambda . (fval (car x))] [quote x]])))
	  */
	a2=car(a1);
	if(ATOME_FTYP(a2) == MACRO)
	{
	  a1=cons(cons(lambda,((struct atome *)a2)->fval),ncons(cons(quote,ncons(a1))));
	  pc=eval;
	}
	else derec;
}
