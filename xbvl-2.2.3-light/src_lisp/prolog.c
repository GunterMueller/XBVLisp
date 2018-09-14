/*
 *	prolog.c	Patrick Greussay
 *			Mardi 14 Decembre 1982, 5:13:20
 */
/*   PROVISOIRE:  les externes dans gc.c */
#include <vlisp.h>

#ifdef STDC_HEADERS
static Int *make_list_value(Int, Int *);
extern Int recrii(Int*), iult(void);
extern Int voir(Int*);
#else
static Int *make_list_value();
extern Int recrii(), iult();
extern Int voir();
#endif

/* les fonctions a declarer */

Int initiate(), forwd(), forwd2(), backtrack(), success(),
    evalprol(), terminate(), failure();
Int evlprl_1(),forw2_1(),forw_1();
Int recreal(), recre_1(),recre_2();
Int lisp_1(),affpr_1();
Int recri(),recri_1(),preaq_1(),preaq_2();

	/* LES MACROS */

#define mk_node(x1,x2,x3,x4,x5) \
			(cons(x1,cons(x2,cons(x3,cons(x4,cons(x5,\
				(isnil(atprolog)?nil:(Int *)(++nnode))))))))
#define parent(x) (car(x))
#define last_choice_point(x) (car(((ATOMEPTR)x)->pval))
#define alternatives(x) (*CAR(cdr(((ATOMEPTR)x)->pval)))

/*
#define lit_list(x) (int_p1=cdr(cdr(((ATOMEPTR)x)->pval)),car(((ATOMEPTR)int_p1)->pval))
*/

#define isvar(x)	(isatom(x) && (((ATOMEPTR)x)->tags & VARIABLE))
#define isdol(x)	(x == atdol)
#define isassert(x)	(isatom(x) && (((ATOMEPTR)x)->tags & CLAUSE))
#define evalp(x)	(isatom(x) && (((ATOMEPTR)x)->tags & EVALUABLE))

	/* VARIABLES PROLOG */
Int *ce,*cn,*clcp,*ilitlist,*iliteral;
static Int ci,nnode;
static Int *predname;
static Int *nn,*mm,*xx;
static Int *svp;

	/* FONCTIONS AUXILIAIRES */

/* #ifdef PROTRACE */
/* trace de la representation interne des objets */
printlist(x) Int *x; {
  if(isatom(x)) { fprintf(stderr, "%s", ((ATOMEPTR)x)->pn+1); return; }
  else if(isflo(x)) { fprintf(stderr, "%g", valflo(x)); return; }
  else if(isnum(x)) { fprintf(stderr, "%d", valnb(x)); return; }
  else if(isstr(x)) { fprintf(stderr, "%s", x+(2/2)); return; }
  else if(islist(x)) {
    if(islist(car(x))) { fprintf(stderr, "("); printlist(car(x)); fprintf(stderr, ")"); }
    else printlist(car(x));
    if(cdr(x) != nil) { fprintf(stderr, " "); printlist(cdr(x)); }
  }
}
#define prlist(x) { fprintf(stderr, "("); printlist(x); fprintf(stderr, ")"); }
/* #endif */


printrace(str,x)CHAR *str; Int *x;{
	Int **n,y;
	n=(Int**)cdr(cdr(((ATOMEPTR)cn)->pval)); n=(Int**)cdr(cdr((Int*)n));
#ifdef PROTRACE
	fprintf(stderr, "\nce = "); prlist(ce);
/*	fprintf(stderr, "\ncn = "); prlist(cn);
	fprintf(stderr, "\nclcp = ") ; prlist(clcp);
*/	fprintf(stderr, "\nilitlist = "); prlist(ilitlist);
	fprintf(stderr, "\niliteral = "); prlist(iliteral);
	fprintf(stderr, "\n%s %d ", str, n); prlist(x);
	fprintf(stderr, "\n");
#endif	
	for(y=(Int)n; y>0; y--)fwst(" ");
	fwst(str); fwst("("); fwint((Int)n); fwst(") ="); voir(x);
}

Int *
lit_list(x) Int *x;{
	x=cdr(cdr(((ATOMEPTR)x)->pval)); return(car(((ATOMEPTR)x)->pval));
}

		/* aa == (n . var) , ee == (((n . var) . (n . var)) ...) */
Int *
ult(aa)Int *aa;{
  re:
	if(isvar(cdr(aa)))
	{
	  for(xx=ce; islist(xx) ; xx=cdr(xx))
	    if((car(aa) == car((Int*)*(((ATOMEPTR)xx)->cval))) && (cdr(aa) == cdr((Int*)*(((ATOMEPTR)xx)->cval))))
	    {
	      aa=cdr(((ATOMEPTR)xx)->cval); if(isvar(cdr(aa)))goto re;
	    }  
	}
	return(aa);
}

Int *
ultt(aa)Int *aa;{/* used in recri */
	push(ce); ce=a3; aa=ult(aa); pop_cast(ce,(Int*)); return(aa);
}

Int *
icar(x) Int *x;{
	return(cons(car(x),car(((ATOMEPTR)x)->pval)));
}

Int *
icdr(x) Int *x;{
	return(cons(car(x),cdr(((ATOMEPTR)x)->pval)));
}
	/* INTERPRETE PROLOG */

Int assert(){/*  (assert cl1 .. clN)  fsubr */
	a3=a1;
	a1=car(car(car(a3))); aux1=((ATOMEPTR)a1)->tags; aux1|=CLAUSE; ((ATOMEPTR)a1)->tags=aux1;
	if(*atcareful==(Int)t) { clause(); }
	a2=atassert; a1=cons(a1,cons(a2,ncons(a3))); pc=put;
}

Int prolog(){/*  (prolog e1 ... eN)  fsubr */
	a1=ncons(cons(ncons(atprolog),a1)); rec(assert,initiate);
}

Int initiate(){
	nnode= -1;
 	cn=mk_node(nil,nil,nil,nil,cons(0,ncons(ncons(atprolog))));
	ce=nil;
	ci=0;
	clcp=nil;
	pc=forwd;
}

Int forwd(){/*  interprete  */
#ifdef PROTRACE
printf("forwd\n");
#endif
	ilitlist=lit_list(cn);
	if(isnil(cdr(ilitlist))){pc=success; return;}
	if(isnotlist(cdr(ilitlist))){fwst("wrong_body\n"); pc=reent; return;}
	iliteral=ult(icar(ilitlist));
	predname=car(((ATOMEPTR)iliteral)->pval);

	if(isnotnil(car(atprolog)))
	{
	  a3=ce; printrace("call",recrii(iliteral));
	}
	if(isvar(predname))
	{
	  a4=car(iliteral); a5=predname;
	  a3=ce; iult(); predname=a5;
	  if(isvar(predname)) {fwst("unbound_predname\n"); pc=reent; return;}
	}
	if(islist(predname)){fwst("wrong_predname\n"); pc=reent; return;}

	if(evalp(predname)){pc=evalprol; return;}
	if(isassert(predname))
	{
	  a1=predname; a2=atassert; gett(); alternatives(cn)=(Int)car(a4);
	  pc=forwd2;
	}
	else
	{
	  a1=iliteral; pushf(forw_1); a3=ce; rec(recreal,eval); return;
	}
}

Int forw_1(){/* used in evlprl-1 */
	*CDR(ilitlist)=(Int)cdr(((ATOMEPTR)ilitlist)->pval); pc=forwd;
}

Int forwd2(){/*  interprete */
#ifdef PROTRACE
printf("forwd2\n");
#endif
	a2=(Int*)alternatives(cn);
#ifdef PROTRACE
 if(isnotnil(car(atprolog)))
   {printf("\nilitlist = "); prlist(ilitlist);
    printf("\nAlternative(%d) = ", ci+1); prlist(a2); printf("\n");}
#endif
	if(isnil(a2))
	{
	  if(isnotnil(car(atprolog)))
	  {
	    a3=ce; printrace("fail",recrii(icar(ilitlist)));
	  }	/* a2 ruine mais on s'en moque ! */
	  pc=backtrack; return;
	}
	a1=icar(ilitlist); a2=cons((Int*)(ci+1),car(((ATOMEPTR)a2)->cval)); a3=ce;
	rec(unify,forw2_1);
}

Int forw2_1()
{
  /* resultat de l'unification dans a1 */
  a4=(Int*)alternatives(cn); a3=car(a4); alternatives(cn)=(Int)cdr(a4);
  if(isnil(car(a1))){pc=forwd2; return;}
  if(isnotnil(a4) || isnotnil(car(atprolog)))clcp=cn;
  ce=cdr(a1);
  ci++;
  if(isnil(cdr(a3))){pc=success; return;}
  cn=mk_node(cn,clcp,nil,ce,cons((Int*)ci,cdr(a3)));
  pc=forwd;
}

Int backtrack(){/*  interprete */
#ifdef PROTRACE
printf("backtrack\n");
#endif
	if(isnil(clcp)){pc=failure; return;}
	a3=ce;			/* pour le recrii de la trace */
	cn=clcp;
	a2=cdr(cdr(((ATOMEPTR)cn)->pval)); ce=car(a2);
	clcp=last_choice_point(cn);
	ilitlist=lit_list(cn);
#if 0
 	ci=(Int)(car(ilitlist)); 
#else
        if(car(ce) == nil) ci = 1;
#if 0
        else ci = car(car(car(ce)));
#endif
        else {
	  Int *tce, nmax = (Int)car(car(car(ce))), tmp;
	  for(tce = cdr(ce) ; tce != nil; tce = cdr(tce)) {
	    if((tmp = (Int)car(car(car(tce)))) >= nmax) nmax = tmp;
	    else break;
	  }
	  ci = nmax;
	}

#endif
	if(isnotnil(car(atprolog)))printrace("redo",recrii(icar(ilitlist)));
	pc=forwd2;
}

Int failure(){/* retour a vlisp */
	ce=cn=clcp=ilitlist=iliteral=nil;
	a1=nil;
	derec;
}

		/* en principe integrable a forwd() */

Int success(){/*  interprete */
#ifdef PROTRACE
printf("success\n");
#endif
  re:
	a1=cn;
	if(isnil(a1)){pc=backtrack; return;}
	a4=lit_list(a1);

	if(isnotnil(car(atprolog)))
	{
	  push(a4);
	  a3=ce; printrace("exit",recrii(icar(a4)));
	  pop_cast(a4,(Int *));
	}

        if(car(a4) == 0) { pc=terminate; return; }
	if(isnil(cdr(((ATOMEPTR)a4)->pval))){cn=parent(cn); goto re;}

	ilitlist=icdr(a4);
	cn=mk_node(parent(cn),clcp,nil,ce,ilitlist);
	pc=forwd;
  }

/* remplacement des variables dans une liste "resultat" par leur valeur */


Int 
has_var_in_list(l)
Int *l;
{
  while(islist(l)) { 
    if(isvar(car(l))) return 1;
    if(islist(car(l)) && has_var_in_list(car(l))) return 1;
    l = cdr(l);
  }
  return isvar(l);
}



Int *
make_result(l, v) Int l, *v; 
{
  Int nlev, *x, *xce, *ret = nil, *ptr, *tmp;
  for( xce = ce; car(xce) != nil; xce = cdr(xce)) 
    if(car(car(car(xce))) == (Int*)l) {
      if(v != nil && cdr(car(car(xce))) != v) continue;
      if(v == nil) ret = cons((ptr = cons(cdr(car(car(xce))), nil)), ret);
      x = cdr(car(xce));
      nlev =(Int) car(x);
      x = cdr(x);

      if(isvar(x)) tmp = make_result(nlev, x);
      else if(islist(x)) tmp = make_list_value(nlev, x); 
      else tmp = x;

      if(v != nil) return tmp; 
      *((Int **)CDR(ptr)) = tmp;
    }
  return ret;
}

static Int *
make_list_value(nlevel, lst)
Int nlevel, *lst;
{	
  Int *ret = nil, *ptr, *tmp;
  if(has_var_in_list(lst))
    for(;;) {
    
      if(islist(car(lst))) tmp = make_list_value(nlevel, car(lst));
      else if(isvar(car(lst))) tmp = make_result(nlevel, car(lst));
      else tmp = car(lst);
    
      if(ret == nil) ret = ptr = cons(tmp, nil);
      else *((Int **)CDR(ptr)) = cons(tmp, nil);

      /* check cdr */
      if(!islist(cdr(lst))) {
	if(isvar(cdr(lst))) *((Int **)CDR(ptr)) = make_result(nlevel, cdr(lst));
	break;
      }
      else lst = cdr(lst); 
    }
  else ret = lst;
  return ret;
}

Int terminate(){/* sortie de prolog avec success */
#ifdef PROTRACE 
  printlist(ce);
#endif 
  a1 = make_result(1, nil);
  ce=cn=clcp=ilitlist=iliteral=nil;
  if(a1 == nil) a1 = t;
  derec;
}

		/* en principe integrable a forwd() */

Int evalprol(){/*  interprete */
#ifdef PROTRACE
printf("evalprol\n");
#endif

	a2=predname; a1=icdr(iliteral);
	a1=cons(quote,ncons(a1));
	a1=cons(a2,ncons(a1));
	rec(eval,evlprl_1);
}

Int evlprl_1(){
	if(isnil(car(a1))){pc=backtrack; return;}
	ce=cdr(a1);
	a4=cdr(((ATOMEPTR)cn)->pval); *CAR(((ATOMEPTR)a4)->pval)=(Int)ce;
	pc=forw_1;
  }


	/* pour extraire une valeur vlisp d'une variable prolog */

Int recreal(){/*  (recreal e env)  */
	if(isnotlist(cdr(a1)))
	  if(isvar(cdr(a1))){rec(recri,recre_2); return;}
	  else {a1=cdr(a1); derec;}
	push(icdr(a1)); a1=icar(a1); rec(recreal,recre_1); return;
}

Int recre_1(){
	pop_cast(a2,(Int *)); push(a1); a1=a2; rec(recreal,copy_2);
  }

Int recre_2(){
	a1=cons(quote,ncons(a1)); derec;
  }

Int recri(){/* recreal-interne */
	a1=ultt(a1);
	if(isnotlist(cdr(a1))){a1=cdr(a1); derec;} /* vrai pour atomes
						    * et vars non insts
						    */
	push(icdr(a1)); a1=icar(a1); rec(recri,recri_1); return;
}

Int recri_1(){
	pop_cast(a2,(Int *)); push(a1); a1=a2; rec(recri,copy_2);
  }

Int recrii(x)Int *x;{
	x=ultt(x);
	if(isnotlist(cdr(x)))return((Int)cdr(x));
	else
	{
	  x=cons((Int*)recrii(icar(x)),(Int*)recrii(icdr(x))); return((Int)x);
	}
}

			/* PREDICATS EVALUABLES */

Int fail(){/* (fail)  subr 0 */
	a1=cons(nil,ce); derec;
}

Int cut(){/*  (cut)  subr 0  */
	a4=parent(cn); clcp=last_choice_point(a4); a1=cons(t,ce); derec;
}

Int var(){/* (var e)  subr 1 */
	a1=cons(isvar(car(((ATOMEPTR)a1)->pval)) ? t : nil,ce); derec;
}

Int univ(){/* (univ x y)  subr 1 (j'insiste!) */
	a2=cons(car(a1),car(cdr(((ATOMEPTR)a1)->pval))); a1=icar(a1); a3=ce; pc=unify;
}

Int lisp(){/*  (lisp? e)  subr 1 */
	a3=ce; pushf(lisp_1); rec(recreal,progn);
}
Int lisp_1(){
	a1=cons(a1,ce); derec;
}

Int affpro(){/* (:= $var e)  subr 1 */
	/* a1 = (n $var e) */
	push(a1); a1=cons(car(a1),cdr(((ATOMEPTR)a1)->pval));
	pushf(affpr_1); a3=ce;
	rec(recreal,progn);
}

Int affpr_1(){
	pop_cast(a2,(Int *)); a1=cons(car(a2),a1);
	a2=cons(car(a2),car(((ATOMEPTR)a2)->pval));
	a3=ult(a2);
	if(a2 == a3)ce=cons(cons(a2,a1),ce);
	else {*CAR(a3)=(Int)car(a1); *CDR(a3)=(Int)cdr(a1);}
	a1=cons(t,ce);
	derec;
}

	/* UNIFICATION */

		/* a1, a2, a3==environnement */
Int unify(){
#ifdef PROTRACE
if(isnotnil(car(atprolog))) {printf("Unify "); prlist(a1); printf("\nWith "); prlist(a2); printf("\n");}
#endif
	nn=car(a1); a1=cdr(a1); mm=car(a2); a2=cdr(a2);
	svp=p;
	rec(preequate,unifsuc);
}

Int unifsuc(){
	a1=cons(t,a3); derec;
}

Int preequate(){
	if(isvar(a1))
	{
	  a4=nn; a5=a1; iult(); nn=a4; a1=a5;
	}
	if(isvar(a2))
	{
	  a4=mm; a5=a2; iult(); mm=a4; a2=a5;
	}
	if(islist(a1) && car(a1) == atperl)
	{
	  push(a1); push(a2); push(nn); push(mm); push(a3);
	  a1=cons(nn,a1);
	  pushf(preaq_1); pushf(progn); rec(recreal,cdrp); return;
	}
	if(islist(a2) && car(a2) == atperl)
	{
	  push(a1); push(a2); push(nn); push(mm); push(a3);
	  a1=cons(mm,a2);
	  pushf(preaq_2); pushf(progn); rec(recreal,cdrp); return;
	}
	pc=iequate;
}

Int preaq_1(){
	pop_cast(a3,(Int *)); pop_cast(mm,(Int*)); pop_cast(nn,(Int*)); pop_cast(a2,(Int *)); --p;
	pc=preequate;
}

Int preaq_2(){
	pop_cast(a3,(Int *)); pop_cast(mm,(Int*)); pop_cast(nn,(Int*)); a2=a1; --p; pop_cast(a1,(Int *));
	pc=preequate;
}

Int iult(){
  re:
	for(xx=a3; islist(xx) ; xx=cdr(xx))
	  if((a4 == car((Int*)*(((ATOMEPTR)xx)->cval))) && (a5 == cdr((Int*)*(((ATOMEPTR)xx)->cval))))
	  {
	    a5=cdr(cdr(((ATOMEPTR)xx)->cval)); a4=car(cdr(((ATOMEPTR)xx)->cval));
	    if(isvar(a5))goto re;
	    return;
	  }  
}

Int iequate(){
	if(nn == mm && a1 == a2){xx=t; derec;}   /* PROVISOIRE c'est EQUAL ? */
	if(isvar(a1))
	{
	  if(isdol(a1)){xx=t; derec;}
	  a1=cons(nn,a1); a2=cons(mm,a2);
	  a3=cons(cons(a1,a2),a3); xx=t; derec;
	}
	if(isvar(a2))
	{
	  if(isdol(a2)){xx=t; derec;}
	  a2=cons(mm,a2); a1=cons(nn,a1);
	  a3=cons(cons(a2,a1),a3); xx=t; derec;
	}
	if(islist(a1) && islist(a2))
	{
	  push(cdr(a1)); push(cdr(a2)); push(nn); push(mm);
	  a1=car(a1); a2=car(a2);
	  rec(preequate,iequa_1); return;
	}
	if(islist(a1) || islist(a2))pc=unifail;
	else rec(eq,iequa_0);
}

Int iequa_0(){
	if(isnil(a1))pc=unifail;
	else
	{
	  xx=t; derec;
	}
}

Int iequa_1(){
	pop_cast(mm,(Int*)); pop_cast(nn,(Int*)); pop_cast(a2,(Int *)); pop_cast(a1,(Int *));
	pc=(isnil(xx)) ? unifail : preequate;
}

Int unifail(){
	p=svp; a1=cons(nil,a3); derec;
}


Int voir(x)Int *x;{
	Int *mysvp,xx;
	push(a1); push(a2); push(a3); push(a4); push(a5);
	a1=x;
	mysvp=p;
	push(nil); pushf(terpri); pc=probj;
	for(;;){xx=(Int)pc; if(isnil((Int*)xx))break; xx&=~01; pc=(Int(*)())xx; (*pc)();}
	p=mysvp;
	pop_cast(a5,(Int *)); pop_cast(a4,(Int *)); pop_cast(a3,(Int *)); pop_cast(a2,(Int *)); pop_cast(a1,(Int *));
}

