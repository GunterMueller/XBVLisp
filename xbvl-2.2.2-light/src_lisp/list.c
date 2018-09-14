
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	list.c		trucs de listes
 *			Patrick Greussay  Decembre 1981
 *			Harald Wertz   Mai 82
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 23 Fevrier 83
 */

#include <vlisp.h>

extern Int gett();

Int carp(){/* (car e) subr 1   ET   (quote e) fsubr */
	a1=car(a1); derec;}

Int cdrp(){/* (cdr e) subr 1 */
	a1=cdr(a1); derec;}

Int fo_2(){/* voila, le car et le cdr sont faites, faisons le cons */
	a2=a1;pop_cast(a1,(Int *)); a1=cons(a1,a2);derec;}

Int fo_1(){/* le car est fait! reste le cdr */
	xtopst(a1,via,a3); rec(foo,fo_2);}

Int f_sp2(){/* faut faire l'append */
	a2=a1;pop_cast(a1,(Int *)); pc=append;}

Int f_splice(){/* on vient d'evaluer l'expression */
	   /* empile le car et depile le cdr */
	xtopst(a1,via,a3); rec(foo,f_sp2);}

Int foo(){/* fait le boulot pour quasiquote */
	if(isnotlist(a1)){derec;}
	else
	if(car(a1)==atunquo) {a1=cdr(a1); pc=evca1;}
	else
	{
	  a2=car(a1);
	  if(islist(car(a1)) && car(a2)==atsplice)
	  {
	    push(cdr(a1)); a1=cdr(a2); rec(evca1,f_splice);
	  }
	  else
	  {
	    push(cdr(a1)); a1=a2; rec(foo,fo_1);
	  }
	}
}

Int quasiquote(){/* (quasiquote a b c)  fsubr */
	pc=foo;}

Int caar(){/* (caar e) subr 1 */
	/* pour rouler C en double indirection */
/*	a1=car(a1->cval); derec;} PLUS VRAI AVEC LES ANOTATIONS*/
	a1=car(a1);a1=car(a1);
/*	a1=car(car(a1));*/derec;}

Int cadr(){/* (cadr e) subr 1 */
/*	a1=car(a1->pval); derec;} */
	a1=cdr(a1);a1=car(a1);
/*	a1=car(cdr(a1));*/derec;}

Int cdar(){/* (cdar e)  subr 1 */
/*	a1=cdr(a1->cval); derec;} */
	a1=car(a1);a1=cdr(a1);
/*	a1=cdr(car(a1));*/derec;}

Int cddr(){/* (cddr e)  subr 1 */
/*	a1=cdr(a1->pval); derec;} */
	a1=cdr(a1);a1=cdr(a1);
/*	a1=cdr(cdr(a1)); */derec;}

Int caaar(){/* (caaar e)  subr 1 */
/*	a1=car(*(a1->cval)); derec;} */
	a1=car(a1);a1=car(a1);car(a1);
/*	a1=car(car(car(a1))); */derec;}

Int caadr(){/* (caadr e)  subr 1 */
/*	a1=car(*(a1->pval)); derec;} */
	a1=cdr(a1);a1=car(a1);a1=car(a1);
/*	a1=car(car(cdr(a1))); */derec;}

Int cadar(){/* (cadar e)  subr 1 */
/*	a1=car(cdr(a1->cval)); derec;} */
	a1=car(a1);a1=cdr(a1);a1=car(a1);
/*	a1=car(cdr(car(a1))); */derec;}

Int caddr(){/* (caddr e)  subr 1 */
/*	a1=car(cdr(a1->pval)); derec;} */
	a1=cdr(a1);a1=cdr(a1);a1=car(a1);
/*	a1=car(cdr(cdr(a1))); */derec;}

Int cdaar(){/* (cdaar e)  subr 1 */
/*	a1=cdr(*(a1->cval)); derec;} */
	a1=car(a1);a1=car(a1);a1=cdr(a1);
/*	a1=cdr(car(car(a1))); */derec;}

Int cdadr(){/* (cdadr e)  subr 1 */
/*	a1=cdr(*(a1->pval)); derec;} */
	a1=cdr(a1);a1=car(a1);a1=cdr(a1);
/*	a1=cdr(car(cdr(a1))); */derec;}

Int cddar(){/* (cddar e)  subr 1 */
/*	a1=cdr(cdr(a1->cval)); derec;} */
	a1=car(a1);a1=cdr(a1);a1=cdr(a1);
/*	a1=cdr(cdr(car(a1))); */derec;}

Int cdddr (){/* (cdddr e)  subr 1 */
/*	a1=cdr(cdr(a1->pval)); derec;} */
	a1=cdr(a1);a1=cdr(a1);a1=cdr(a1);
/*	a1=cdr(cdr(cdr(a1))); */derec;} 

Int nth(){/* (nth n l) subr 2 */
	for(na1=valnb(a1); --na1 > 0 ; a2=cdr(a2))if(isnotlist(a2))break;
	a1=a2; derec;}

Int consp(){/* (cons e1 e2)  subr 2 */
	a1=cons(a1,a2); derec;}

Int mcons(){/* (mcons . l)  nsubr */
	if(isnotlist(cdr(a1))){a1=car(a1); derec;}
	a3=a2=ncons(car(a1));
	for(;;){a1=cdr(a1);
		if(isnotlist(cdr(a1)))
/*		   {cdr(a3)=car(a1); break;} */
		   {*CDR(a3)=(Int)car(a1); break;}
		*CDR(a3)=(Int)ncons(car(a1)); a3=cdr(a3);}
	a1=a2; derec;}

Int oblist(){/* (oblist)  subr 0 */
	for(a1=nil,a2=derat; a2 ; a1=cons(a2,a1),a2=(Int *)(((struct atome *)a2)->nexa));
	a1=cdr(a1);a1=cdr(a1);
	derec;}

Int nullp(){/* (null e)  subr 1 */
	a1=(isnil(a1)) ? t : nil; derec;}

Int append(){/* (append e1 e2)  subr 2 */
	if(isnotlist(a1))a1=(isnil(a1)) ? a2 : cons(a1,a2);
	else{
	  a3=ncons(0); push(a3);
	  while(islist(a1)){
	    /*cdr(a3)*/ *CDR(a3)=(Int)ncons(car(a1)); a3=cdr(a3); a1=cdr(a1);}
	  /*cdr(a3)*/ 
	  *CDR(a3)=(Int)((islist(a2)) ? a2 : (isnil(a2) ? nil : ncons(a2)));
	  pop_cast(a1,(Int *)); a1=cdr(a1);}
	derec;}

Int reverse(){/* (reverse e1 e2)  subr 2 */
	while(islist(a1)){a2=cons(car(a1),a2); a1=cdr(a1);}
	a1=a2; derec;}

Int length(){/* (length l)  subr 1 */
	for(na1=0; (islist(a1)) ; a1=cdr(a1),++na1);
	a1=cranb(na1); derec;}

Int atom(){/* (atom e)  subr 1 */
	a1=(isnotlist(a1)) ? t : nil; derec;}

Int atomp(){/* (atomp nom) subr1 */
  a1=(isatom(a1) ? a1 : (isstr(a1) ? cherchat(a1) : nil));
  if(!a1) a1 = nil;
  derec;
}

Int litatom(){/* (litatom e)  subr 1 */
	a1=(isatom(a1)) ? t : nil; derec;}

Int listp(){/* (listp e)  subr 1 */
	a1=(islist(a1)) ? t : nil; derec;}

Int numbp(){/* (numbp e)  subr 1 */
	a1=(isnum(a1)) ? t : nil; derec;}

Int stringp(){/* (stringp e)  subr 1 */
	a1=(isstr(a1)) ? t : nil; derec;}

Int equal(){/* (equal e1 e2)  subr 2 */
	savp=p; push(t); rec(equa2,popa1);}

Int   equa2(){
	if(islist(a1))
	  if(isnotlist(a2))goto e2;
	  else{push(cdr(a1)); push(cdr(a2)); a1=car(a1); a2=car(a2);
	       rec(equa2,equa2_1); return;}
	if(a1==a2)derec;

#ifndef NO_VFLO
	if(isflo(a1) && isflo(a2))
	  if(valflo(a1) != valflo(a2)) goto e2; else derec;
#endif

	if(isnum(a1) && isnum(a2))
	  if(valnb(a1) != valnb(a2)) goto e2; else derec;
	if(isatom(a1))a1=((struct atome *)a1)->pn; if(isatom(a2))a2=((struct atome *)a2)->pn;
	if(isstr(a1) && isstr(a2))
	  if(strcmp((CHAR *)(a1+(2/2)),(CHAR *)(a2+(2/2)))==0)derec;
  e2:
	p=savp; a1=nil; derec;}
	

Int   equa2_1(){pop_cast(a2,(Int *)); pop_cast(a1,(Int *)); pc=equa2;}

Int addprop(){/* (addprop at indic val)  subr 3 */
	/*cdr(a1)*/ *CDR(a1)=(Int)cons(a2,cons(a3,cdr(a1))); derec;}

Int remprop(){/* (remprop at indic)  subr 2 */
    gett(); if(isnotnil(a4))/*cdr(a5)*/ {*CDR(a5)=(Int)cdr(a4);} derec;}

Int put(){/* (put at indic1 val1 ... indicN valN)  nsubr  */
	a4=cdr(a1); a1=car(a1);
	while(isnotnil(a4)){
	  a2=car(a4); 
	  A1=cdr(a4);
	  a3=car(A1);
	  A1=cdr(a4);A1=cdr(A1);
	  push(A1);
	  gett();
	  if(isnil(a4))
		{/*cdr(a1)*/ *CDR(a1)=(Int)cons(a2,cons(a3,cdr(a1)));}
	  else /*car(a4)*/ {*CAR(a4)=(Int)a3;}
	  pop_cast(a4,(Int *));}
	derec;}

Int get(){/* (get at indic)  subr 2 */
  gett(); a1=car(a4); derec;}

Int gett(){/* vraie fonction C, d'appel gett(); */
	/* a1 = un atome, a2 = un indicateur */
	/* si non trouve, a4 = nil */
	/* sinon a4 = (oldval ...) , a5 = (x indic oldval ...) */
	int_p1=a4=a5=a1;
	for(;;){
	  a4=cdr(a4); if(isnotlist(a4))break;
	  a1=car(a4);
	  a1=((a1==a2) || (isnum(a1) && isnum(a2) && (valnb(a1)==valnb(a2))))
	     ? t : nil;
	  a4=cdr(a4); if(isnotnil(a1))break;
	  a5=a4;}
	a1=int_p1;}

Int member(){/* (member e1 e2)  subr 2 */
	if(isnotlist(a2)){a1=nil; derec;}
	push(a2); push(a1); a2=car(a2); rec(equal,membr_1);}

Int membr_1(){
	pc=(isnotnil(a1)) ? (--p, popa1) : (pop_cast(a1,(Int *)),pop_cast(a2,(Int *)),a2=cdr(a2),member);}

Int memq(){/* (memq e1 e2)  subr 2 */
	if(isnotlist(a2)){a1=nil; derec;}
	push(a2); push(a1); a2=car(a2); rec(eq,memq_1);}

Int memq_1(){
	pc=(isnotnil(a1)) ? (--p, popa1) : (pop_cast(a1,(Int *)),pop_cast(a2,(Int *)),a2=cdr(a2),memq);}

Int assoc(){/* (assoc e1 e2)  subr 2 */
	if(isnotlist(a2)){a1=nil; derec;}
	push(a2); push(a1); 
	A1=car(a2);
	a2=car(A1);
	rec(equal,assoc_1);}

Int assoc_1(){
	if(isnotnil(a1)){--p; pop_cast(a1,(Int *)); a1=car(a1); derec;}
	pop_cast(a1,(Int *)); pop_cast(a2,(Int *)); a2=cdr(a2); pc=assoc;}

Int assq(){/* (assq e1 e2)  subr 2 */
	if(isnotlist(a2)){a1=nil; derec;}
	push(a2); push(a1); a2=car(a2); a2=car(a2);
	rec(eq,assq_1);}

Int assq_1(){
	if(isnotnil(a1)){--p; pop_cast(a1,(Int *)); a1=car(a1); derec;}
	pop_cast(a1,(Int *)); pop_cast(a2,(Int *)); a2=cdr(a2); pc=assq;}

Int delq(){/* (delq e1 e2)  subr 2 */
	a3=ncons(nil); push(a3); push(a1); pc=delq_1;}

Int delq_1(){
	if(isnotlist(a2)){--p; pop_cast(a1,(Int *)); a1=cdr(a1); derec;}
	a1= (Int*)*p; push(cdr(a2)); a2=car(a2); push(a2); rec(eq,delq_2);}

Int delq_2(){
	pop_cast(a4,(Int *)); if(a1 == nil){/*cdr(a3)*/ *CDR(a3)=(Int)ncons(a4); a3=cdr(a3);}
	pop_cast(a2,(Int *)); pc=delq_1;}

Int delete(){/* (delete e1 e2)  subr 2 */
	a3=ncons(nil); push(a3); push(a1); pc=delet_1;}

Int delet_1(){
	if(isnotlist(a2)){--p; pop_cast(a1,(Int *)); a1=cdr(a1); derec;}
	a1= (Int*) *p; push(cdr(a2)); a2=car(a2); push(a2); rec(equal,delet_2);}

Int delet_2(){
	pop_cast(a4,(Int *)); if(a1 == nil){/*cdr(a3)*/ *CDR(a3)=(Int)ncons(a4); a3=cdr(a3);}
	pop_cast(a2,(Int *)); pc=delet_1;}

Int last(){/*  (last l n)  subr 2 */
	Int i;
	aux1=(isnil(a2))?1:valnb(a2);
	if(aux1<=0){a1=nil; derec;}
	for(;;){
	  i=aux1; a2=a1;
	  while(i--)a2=cdr(a2);
	  if(isnotlist(a2))derec;
	  a1=cdr(a1);}
}

Int pairlis(){/*  (pairlis l1 l2 a)  subr 3 */
	push(a3);
	a3=ncons(0); push(a3);
	while(islist(a1))
	{
	  /*cdr(a3)*/
	  *CDR(a3)=(Int)ncons(cons(car(a1),car(a2))); a3=cdr(a3);
	  a1=cdr(a1); a2=cdr(a2);
	}
	/*cdr(a3)*/ *CDR(a3)= *(p-1);
	pop_cast(a1,(Int *)); a1=cdr(a1); p--; derec;
}

Int copy(){/* (copy e)  subr 1 */
	if(islist(a1)){push(cdr(a1)); a1=car(a1); rec(copy,copy_1);}
	else if(wherefrom==0) {derec;} else return(0);
}

Int   copy_1(){
	pop_cast(a2,(Int *)); push(a1); a1=a2; rec(copy,copy_2);
  }

Int   copy_2(){	/* used by subst_2, subli_2 */
	pop_cast(a2,(Int *)); a1=cons(a2,a1); derec;
  }

Int cpa_4(){/* y'a rien de special */
	pop_cast(a2,(Int *));*CDR(a2)=(Int)a1;a1=a2;derec;
}

Int cpa_3(){/* le cdr */
	pop_cast(a2,(Int *));
	*CDR(a2)=((UInt)a1 | (UInt)ANNOT);
	a1=a2;derec;
}

Int cpa_2(){/* on a une copy du car */
	a3=a1;pop_cast(a1,(Int *));pop_cast(a2,(Int *));
	/* mettre l'attache */
	*CAR(a2)=((UInt)a3 | (UInt)ANNOT);	
	if(!isnum(*(a1+1)) && ((UInt)(*(a1+1)) & (UInt)ANNOT))
	{push(a2);
	 a1=(Int*)((((UInt)(*(a1+1))) & (UInt)~ANNOT));
	 rec(copyall,cpa_3);}
	else if(islist(cdr(a2)))
	{push(a2);a1=cdr(a1);rec(copyall,cpa_4);}
	else {a1=a2;derec;}
}

Int cpa_5(){/* une copie du car liste */
	a3=a1;pop_cast(a1,(Int *));pop_cast(a2,(Int *));*CAR(a2)=(Int)a3;
	if(islist(cdr(a2)))
	{push(a2);a1=cdr(a1);rec(copyall,cpa_4);}
	else {a1=a2;derec;}
}


Int cpa_1(){a2=a1;pop_cast(a1,(Int *));
	if(!isnum(*(a1)) && ((UInt)(*a1) & (UInt)ANNOT))
	{push(a2);push(a1);
	 a1=(Int*)(((UInt)(*a1)) & (UInt)~ANNOT);
	 rec(copyall,cpa_2);}
	else if(!isnum(*(a1+1)) && ((UInt)(*(a1+1)) & (UInt)ANNOT))
	{push(a2);
	 a1=(Int*)(((UInt)(*(a1+1))) & (UInt)~ANNOT);
	 rec(copyall,cpa_3);}
	else if(islist(car(a2)))
	{push(a2);push(a1);a1=car(a1);rec(copyall,cpa_5);}
	else if(islist(cdr(a2)))
	{push(a2);a1=cdr(a1);rec(copyall,cpa_4);}
	else {a1=a2;derec;}
}

Int copyall(){/* (copy-all e) subr 1 */
	if(islist(a1)){push(a1);rec(copy,cpa_1);}
	else derec;
} 

Int subst(){/* (subst e1 e2 e3) */
	push(a1); push(a2); push(a3);
	a1=a3; rec(equal,subst_1);
}

Int subst_1(){
	pop_cast(a3,(Int *)); pop_cast(a2,(Int *)); a4=a1; pop_cast(a1,(Int *));
	if(isnotnil(a4))derec;
	if(isnotlist(a3)){a1=a3; derec;}
	push(a1); push(a2); push(cdr(a3)); a3=car(a3);
	rec(subst,subst_2);
  }

Int subst_2(){
	pop_cast(a3,(Int *)); pop_cast(a2,(Int *)); a4=a1; pop_cast(a1,(Int *)); push(a4);
	rec(subst,copy_2);
  }

Int sublis(){/*  (sublis al e)  subr 2 */
	push(a1); push(a2); a2=a1; a1= (Int*) *p;
	rec(assoc,subli_1);
}

Int subli_1(){
	pop_cast(a2,(Int *)); a4=a1; pop_cast(a1,(Int *));
	if(isnotnil(a4)){a1=cdr(a4); derec;}
	if(isnotlist(a2)){a1=a2; derec;}
	push(a1); push(cdr(a2)); a2=car(a2); rec(sublis,subli_2);
  }

Int subli_2(){
	a4=a1; pop_cast(a2,(Int *)); pop_cast(a1,(Int *)); push(a4); rec(sublis,copy_2);
  }

extern Int vmemq_1(),vmemq_2();

Int vmemq(){/*  (vmemq at l)  subr 2 */
	a4=p;
	push(nil); pushf(popa1); pc=vmemq_1;
}

Int   vmemq_1(){
	if(isnotlist(a2)){derec;}
	if(isnotatom(car(a2)))
	{
	  push(cdr(a2)); a2=car(a2); pushf(vmemq_2); pc=vmemq_1; return;
	}
	if(car(a2)==a1){a1=a2; p=a4; derec;}
	a2=cdr(a2); pc=vmemq_1;
}

Int   vmemq_2(){
	pop_cast(a2,(Int *)); pc=vmemq_1;
}

Int nmemq(){/*  (nmemq at l)  subr 2 */
	a3=nil;
	while(islist(a2))
	  if(car(a2) != a1){a3=a2; a2=cdr(a2);}
	  else
	  {
	    if(isnil(a3)){A1=cdr(a2); *CAR(a2)=(Int)car(A1); *CDR(a2)=(Int)cdr(A1);
			  a1=a2; derec;}
	    if(isnil(cdr(a2))){*CDR(a3)=(Int)nil; a1=nil; derec;}
	    *CDR(a3)=(Int)nil; A1=cdr(a2); *CAR(a2)=(Int)car(A1); *CDR(a2)=(Int)cdr(A1);
	    a1=a2; derec;
	  }
	a1=nil;
	derec;
}
