 /* %I%, %R%, %B%, %D%,  %T%,  %M% */
 /*
  *	eval.c		interprete et fonctions associees
  *			Patrick Greussay   27 Oct 82
  *			Harald Wertz 1 Jan 83
  *                     Damien Ploix 20 Juin 95 (ajout de atcrewid en plus de pbind
  *                     pour recuperer les fonctions creatrices de callbacks X)
  */

#include <vlisp.h>

extern Int unbcls();

Int *savep;
Int TAIL;

 /* le vecteur de lancement pour eval */

Int *teval[]={
  (Int *)udfe, (Int *)popj, (Int *)evca1_no_careful, (Int *)eval2, (Int *)eval3, (Int *)evlis, /* 0 1 2 3 4  5  */
  (Int *)popj, (Int *)evexp, (Int *)evfexp, (Int *)evmac, (Int *)evesc}; /* 6 7 8 9 10    */

 /* le vecteur de lancement pour apply */

Int *tapply[]={
  (Int *)udfa, (Int *)popj, (Int *)carp , (Int *)apply2, (Int *)apply3, (Int *)popj, /* 0 1 2 3 4 5 */
  (Int *)popj, (Int *)apexp, (Int *)udfa, (Int *)udfa, (Int *)udfa}; /* 6 7 8 9 10  */


extern Int EVsui(),EVsui1(),EVsui2(),EVsui3(),EVsui4(),EVsui5(),EVsui10();
extern Int evcdr();

Int *getival10(x) Int *x;{
   Int *auxi,*auxi1;
   auxi=find(10, (Int *)((struct atome *)x)->ival);
   if(auxi!=0){auxi1=car(auxi);auxi1=cdr(auxi1);return(car(auxi1));}
   else return(nil);}

Int metival10(x,y) Int *x,*y;{
   Int *auxi,*auxi1;
   auxi=find(10,(Int *)((struct atome *)x)->ival);
   if(auxi!=0){auxi1=car(auxi);*CDR(auxi1)=(Int)ncons(y);}
   else {auxi=((struct atome *)x)->ival;auxi1=cons((Int *)10,ncons(y));((struct atome *)x)->ival=cons(auxi1,auxi);}
 }

Int egett(x) Int *x; {if(isnil(x)) a4=nil; else
		     if(a2 == car(x)) a4=cdr(x);
		     else {x=cdr(x);x=cdr(x);egett(x);}
		   }

Int evca12(){			/* retour de l'evaluation des pre-condition */
  pop_cast(a1,(Int *));a2=atsortie;egett(a1);
  a5=car(a4);pop_cast(a4,(Int *));pop_cast(a3,(Int *));pop_cast(a2,(Int *));pop_cast(form, (Int *));pop_cast(a1,(Int *));
  if(isnil(a5)) pc=eval;
  else {push(a5); rec(eval,evcdr);}
}

Int evca11(){
  a1=a4;pushf(evca12);pc=progn;
}

Int evcdr(){
  pop_cast(a5,(Int *));push(a1);	/* sauver le resultat et depiler l'assertion */
  *atsortie=(Int)a1;
  a1=a5;pushf(popa1);pc=progn;return;
}
 /*	pour les subr 1		*/

Int evca1_careful()
{
  if(*atcareful == (Int)t)
    {
#ifndef NOANNOT
      if(!isnum(*(a1)) && ((UInt)(*a1) & (UInt)ANNOT)) {
	a1=(Int *)(((UInt)(*a1)) & (UInt)~ANNOT);
	push((car(a1))); /* le car a evaluer */
	push(form);
	a1=cdr(a1);	/* l'annotation     */
	push(a2);push(a3);push(a4); /* sauve les registres */
	push(a1);	/* les annotations */
	a2=atentree;	/* cherchons les specs d'entree  */
	egett(a1);
	a4=car(a4);
	if(isnil(a4)) {
	  a2=atsortie;egett(a1); /* cherchons la sortie */
	  a5=car(a4);pop_cast(a4,(Int *)); /* les annotations */
	  pop_cast(a4,(Int *));pop_cast(a3,(Int *));pop_cast(a2,(Int *));pop_cast(form, (Int *));pop_cast(a1,(Int *));
	  if(isnil(a5)) pc=eval;
	  else {push(a5);	pushf(evcdr);pc=eval;}}
	else {pc=evca11;return;}}
      else 
#endif
	{a1=car(a1); pc=eval;}} /* else pour isnum.. */
  else {a1=car(a1); pc=eval;}	/* else pour careful */
}

Int evca1_no_careful(){ a1=car(a1); pc=eval; }

Int EVstep4(){pop_cast(a1,(Int *));form=a1;a2=car(a1);a1=cdr(a1);pc=evalfu;}
Int EVstep3(){a2=car(a1);
	   if(ATOME_FTYP(a2) < EXPR || *atstep != (Int)t) 
	     {a1=ncons(a2);rec(Break,EVstep4);}
	   else {pc=EVstep4;}}
Int EVste3(){a1= (Int *)*p;pushf(EVstep3);rec(prini,terpri);}
Int EVstep2(){			/* voila, on fait un step */
   push(a1);			/* sauvegardons la forme a evaluer */
   /* regardons s'il ya un compteur */
   a2=car(a1);
   if(((struct atome *)a2)->ofval > (Int *)0)
     {if(ATOME_FTYP(a2) < EXPR || *atstep!=(Int)t) 
	{((struct atome *)a2)->ofval=(Int *)((Int)((struct atome *)a2)->ofval)-1;}
	pop_cast(a1,(Int *));a2=car(a1);a1=cdr(a1); pc=evalfu;}
   else {a1=atSTEP;rec(prini,EVste3);}
 }

Int EVst6(){pop_cast(form, (Int *));derec;}

Int EVst3(){			/* le step apres liaison */
   if(((struct atome *)a1)->ofval > (Int *)0) /* y'a un compteur */
     {((struct atome *)a1)->ofval=(Int *)((Int)((struct atome *)a1)->ofval)-1;derec}
   a1=ncons(a1);push(form);rec(Break,EVst6);
 }

extern Int EVstepp();

Int EVstep()			/* le test s'il faut vraiment faire un step */
{
  if(isnotatom(car(a1))) {a2=car(a1);a1=cdr(a1);pc=evalfu;return;}
  a2=car(a1);			/* la fonction */
  push(a2);push(a1);a2= (Int*) *atSTEP;a1=car(a1);
  rec(memq,EVstepp);return;
}

Int EVstepp()
{
  a4=a1;pop_cast(a1,(Int *));pop_cast(a2,(Int *));
  if(ATOME_FTYP(a2) >= EXPR && *atSTEP==(Int)t)
    {pc=EVstep2;return;}
  if(ATOME_FTYP(a2) < EXPR && isnil(a4)) {a1=cdr(a1);pc=evalfu;return;}
  if(isnotnil(a4)) {pc=EVstep2;return;}
  if(((struct atome *)atSTEP)->pval == t) {pc=EVstep2;return;}
  else {a1=cdr(a1);pc=evalfu;return;}
}

Int EVst2(){ a4=a1;pop_cast(a1,(Int *));	 if(isnil(a4)) {derec;} else {pc=EVst3;return;} }

Int EVst1(){			/* ceci devrait faire les steps apres liaison */
  if(*atSTEP == (Int)t && *atstep == (Int)t) {push(a1);pc=EVst2;return;}
  push(a1);			/* le nom de fonction */ a2= (Int *)*atSTEP;rec(memq,EVst2);
  return;
}

Int EVstend(){pop_cast(a1,(Int *));popf;return;}

Int METCARF(){*atcareful=(Int)t;pop_cast(a1,(Int *));pop_cast(form, (Int *));a1= (Int *)*a1; derec;}

Int careful()/* (careful t/nil/?) subr 1 */
{
  if(a1!=nil){ 
    *atcareful=(Int)a1; 
    /* fonctions d'évaluation */
    eval     = eval_careful; 
    evca1    = evca1_careful; 
    evalfu   = evalfu_careful;
    teval[2] = (Int *)evca1_careful;
    /* et pour apply... */
    apply    = apply_careful;
  }
  else {
    *atcareful=(Int)nil; 
    /* fonctions d'évaluation */
    eval     = eval_no_careful; 
    evca1    = evca1_no_careful; 
    evalfu   = evalfu_no_careful;
    teval[2] = (Int *)evca1_no_careful;
    /* et pour apply... */
    apply    = apply_no_careful;
    if(a1!=nil) { clearall(); }
  }
  ((struct atome *)ateval)->fval = (Int *)eval;
  derec;
}

Int eval_careful()
{				/* (eval e) subr 1 */
  Int *auxa1;
  form=a1;
  if(isnotlist(a1))
    {
      if(isnum(a1) || isstr(a1)){popf;} 
      else if(*atcareful == (Int)t)
	/* pour le monitoring des variables */
	if(*a1 == (Int)undef)
	  {a1=atindvar; a2=ncons(form); pc=apply;return;}
	else
	  {
	    auxa1=find((Int )atentree, (Int *)((struct atome *)a1)->ival);
	    if(auxa1==0) {a1= (Int *)*a1;popf;return;}
	    else
	      {
		push(form);push(a1);*atcareful=(Int)nil;
		a1=car(auxa1);a1=cdr(a1);a1=car(a1);
		rec(eval,METCARF);
	      }
	  }
      else if((a1 = (Int *)*a1) != undef)
	{
	  popf;
	}
      else
	{
	  a1=atindvar; a2=ncons(form); pc=apply;
	}
      return;
    }
  /* a1 est une liste */
  if(p>(Int *)hpil)pc=erpil;
  else{
    if(isnotnil((Int *)*atSTEP)) pc=EVstep;
    else {
      a2=car(a1); a1=cdr(a1); pc=evalfu;
    }
  }
}

Int eval_no_careful()
{				/* (eval e) subr 1 */
  Int *auxa1;
  form=a1;
  if(isnotlist(a1))
    {
      if(isnum(a1) || isstr(a1)){popf;} 
      else if((a1 = (Int *) *a1) != undef) {popf;}
      else
	{
	  a1=atindvar; a2=ncons(form); pc=apply;
	}
      return;
    }
  /* a1 est une liste */
  if(p>(Int *)hpil)pc=erpil;
  else{
      a2=car(a1); a1=cdr(a1); pc=evalfu;
  }
}

Int EVsui(){			/* on y vient apres d'avoir tourne un exemple 
	     etat de la pile:
		 |   appel  exemple   |
		 |  resultat_desiree  |
		 | reste_des_exemples |
		 |       form	     |
		 | 	 a2	     |
		 | 	 a1	     |
		 | 	....	     |		*/

	  pop_cast(a3,(Int *));pop_cast(a2,(Int *));push(a2);push(a1);push(a3);rec(equal,EVsui2);}

Int EVsui2()
{/* etat de la pile:
    |   appel  exemple   |
    |  resultat effectif |
    |  resultat desiree  |
    | reste des exemples |
    |       form	 |
    |        a2	         |
    |        a1	         |
    |       ....         |
*/

  if(isnil(a1)){
    pop_cast(a1,(Int *)); /* l'appel */
    wst("exemple ne marchant pas :");
    rec(prini,EVsui3);
  }
  else {
    pop_cast(a1,(Int *));pop_cast(a1,(Int *));pop_cast(a1,(Int *));	/* pour enlever la merde */
    pop_cast(a2,(Int *)); /* reste des exemples -> a2 */
    if(isnil(a2)){pop_cast(form, (Int *));pop_cast(a2,(Int *));pop_cast(a1,(Int *));
    *atcareful=(Int)t;
    *atexample=(Int)nil;pc=evalfu;}
    else
      {
	a1=car(a2);a5=cdr(a2);push(a5);a5=cdr(a1);a5=car(a5);
	push(a5);a1=car(a1);push(a1);rec(eval,EVsui);}}
}

Int EVsui3a(){
	 wst("le resultat obtenu est :");pop_cast(a1,(Int *));
	 rec(prini,EVsui4);}

Int EVsui3(){rec(terpri,EVsui3a);}

Int EVsui4a(){
	 wst("le resultat desire etait :");
	 pop_cast(a1,(Int *));
	 rec(prini,EVsui5);}

Int EVsui4(){rec(terpri,EVsui4a);}

Int EVsui5a(){push(a1);push(a1);push(a1);a1=t;pc=EVsui2;}

Int EVsui5(){rec(terpri,EVsui5a);}

Int evalfu_careful()
{				/* a2 = la fonction, a1 = args non evalues */
#ifndef NO_VFLO
  if(isnum(a2)  && isnotflo(a2)){push(a2); rec(evca1,evaln_1);}
#else
  if(isnum(a2)){push(a2); rec(evca1,evaln_1);}
#endif
  else if(isatom(a2)) {
    if(a2 > atuser)
      {
	push(a1);push(a2);a1=a2;
	a3=find(100,(Int *)((struct atome *)a2)->ival);
	if(a3!=0){a3=car(a3);a3=cdr(a3);}
	a2=atexample;gett();a4=car(a4);
	if(isnil(a4) || a3==0 || car(a3)==nil) {pop_cast(a2,(Int *));
	push(a2);
	if(islist(((struct atome *)a2)->fval) && 
	   (pbind>0 || *atcrewid != (Int)nil))
	  {
	    if(!pbind) a5 = (Int *) *atcrewid;
	    else { a5= (Int *) *(pbind-2);a5= (Int *) *(a5+2); }
	    if(isatom(a5)) adapteival((Int *)4, a5, a2);
	    adapteival((Int *)3, a2, a5);
	  }
	pop_cast(a2,(Int *));
	pop_cast(a1,(Int *));
	pushf(((struct atome *)a2)->fval); pc=(Int(*)())teval[ATOME_FTYP(a2)];}
	else
	  {
	    *CAR(a3)=(Int)nil;
	    *atcareful=(Int)nil;
	    push(form);a1=car(a4);a5=cdr(a4);push(a5);
	    a5=cdr(a1);a5=car(a5);push(a5);a1=car(a1);push(a1);
	    rec(eval,EVsui);
	  }
      }
    else			/* le else de a2 > atuser */
      {
	pushf(((struct atome *)a2)->fval); 
#ifndef NO_ERROR_TYPE_CHECKING
	if(*atertyp != (Int)undef && ATOME_ERR(a2)) { 
	  push(a2); 
	  pushf(check_error); 
	} /* traitement des erreurs de type */
#endif
	pc=(Int (*)())teval[ATOME_FTYP(a2)];
      }
  }
  else				/* le else de isatom */
    if(isnotlist(a2))
      {
	a1=form; popf; return;
      }
    else
      if(car(a2)==lambda){push(cdr(a2)); pc=evexp;}
      else
	if(car(a2)==atcls)pc=evcls;
	else
	  {
	    push(a1); a1=a2;
	    if(p>(Int *)hpil)pc=erpil; else rec(eval,evals_1);
	  }
}

Int evalfu_no_careful()
{				/* a2 = la fonction, a1 = args non evalues */
#ifndef NO_VFLO
  if(isnum(a2)  && isnotflo(a2)){push(a2); rec(evca1,evaln_1);}
#else
  if(isnum(a2)){push(a2); rec(evca1,evaln_1);}
#endif
  else if(isatom(a2)) {
	pushf(((struct atome *)a2)->fval); 
#ifndef NO_ERROR_TYPE_CHECKING
	if(*atertyp != (Int)undef && ATOME_ERR(a2)) { 
	  push(a2); 
	  pushf(check_error); 
	} /* traitement des erreurs de type */
#endif
	pc=(Int (*)())teval[ATOME_FTYP(a2)];
      }
  else				/* le else de isatom */
    if(isnotlist(a2))
      {
	a1=form; popf;
	return;
      }
    else if(car(a2)==lambda){push(cdr(a2)); pc=evexp;}
    else if(car(a2)==atcls)pc=evcls;
    else {
      push(a1); a1=a2;
      if(p>(Int *)hpil)pc=erpil; else rec(eval,evals_1);
    }
}

Int evals_1()		/* used by udfe */
{
  a2=a1;
  pop_cast(a1,(Int *)); 
  pc=evalfu;
}


Int evaln_1(){a2=a1; pop_cast(a1,(Int *)); pc=evaln_2;}

/* évaluation de (n liste) avec vérification du type de a2... */
Int evaln_2()
{
  if(*atertyp != (Int)undef && ATOME_ERR(atnth)) {
    pushf(carp);
    pushf(nth);
    push(atnth);
    pc=check_error;
  }
  else rec(nth,carp);
}

Int udfe()/* fonction indefinie dans eval */
{
  --p;
  if(*a2 != (Int)undef  && a2 != (Int *) *a2){a2 = (Int *) *a2; pc=evalfu;}
  else
    {push(a1); if(pbind>0){a5=pbind;a5= (Int *) *(a5-2);a5= (Int *) *(a5+2);}
    else a5=attop;
    a2=cons(a2,ncons(a1)); 
    a4=form;  /*   pour moi (= Harald) afin
		   d'avoir acces au corps de fonction */
    /*  a5       le nom de la fonction	*/
    a1=atindfun; rec(apply,evals_1);}
}

				 /*	pour les subr 0		*/
Int popj(){derec;}
				 /*	pour les subr 2		*/
Int eval2(){push(cdr(a1)); rec(evca1,eval2_1);}

Int eval2_1(){xtopst(a1,via,a5); rec(evca1,eval2_2);}

Int eval2_2(){a2=a1; pop_cast(a1,(Int *)); popf;}
				 /*	pour les subr 3		*/
Int eval3(){push(cdr(a1)); rec(evca1,eval3_1);}

Int eval3_1(){xtopst(a1,via,a5); push(cdr(a1)); rec(evca1,eval3_2);}

Int eval3_2(){xtopst(a1,via,a5); rec(evca1,eval3_3);}

Int eval3_3(){a3=a1; pop_cast(a2,(Int *)); pop_cast(a1,(Int *)); popf;}

Int evexp(){/* evaluation des exprs */
	 /* test de tail-rec ou construction de bloc lambda */
	 pop_cast(a4,(Int *)); pc=evexp_0;}

Int evexp_0(){/* a4 = la fval (vars . corps), used from evexp, self */
	 push(a4);
	 push(car(form)); /* pour le nom de fonction, s'il existe */
	 push(MRKEV); a2=car(a4); /* a2 = vars */
	 pc=evexp_1;}

Int evexp_1(){/* a2 = vars, a1 = vals non encore evaluees */
	 if(islist(a2))
	 {
	   if(isnotlist(a1)){push(nil); pc=evexp_4;}
	   else{push(cdr(a1)); push(a2); rec(evca1,evexp_3);}
	   return;
	 }
	 if(isnil(a2))pc=evexp_5;
	 else{/* var pointee */ push(a2); rec(evlis,evexp_2);}
 }

Int evexp_2(){/* fin de var pointee */
	 a5 = (Int *) *p; *p=(Int)a1; push(a5); pc=evexp_5;}

Int evexp_3(){/* ya encore des vals non encore evaluees */
	 pop_cast(a2,(Int *)); xtopst(a1,via,a5); pc=evexp_4;}

Int evexp_4(){/* on continue la boucle */
	 push(car(a2)); a2=cdr(a2); pc=evexp_1;}

Int evexp_5(){/* ici, pile p = var1 : val1 : ... : MRKEV : fval : 
			 et pour moi: nom_de_fonctio : reste */
	 savp=p;
	 for(;;)
	 {
	   pop_cast(a2,(Int *)); if(a2 == (Int *)MRKEV)break;
 /*	  	echange du sommet de pile avec le car(a2) */
	   a5=(Int*)(*p);*p=(Int)car(a2);*CAR(a2)=(Int)a5;--p;
	 }
	 --p; /* pour depasser le nom_de_fonction */
	 a1= (Int *) *p; *p-- = (Int)pbind;a3=p;
	 pc=evexp_6;
   }

Int ENtree1(){
	 pop_cast(a5,(Int *));
	 if(isnil(a5)){ pop_cast(a1,(Int *)); 
			 if(isnotnil((Int *)*atSTEP) && *atstep==(Int)t) 
				 {rec(EVst1,EVstend);return;}
			 else {pop_cast(a1,(Int *));
				 popf;return;}}
	 a1=car(a5);push(cdr(a5));rec(eval,ENtree1);
 }

Int ENtree(){
			 a2= (Int *) *(pbind-2);	/* pointe vers fin de bloc */
			 a2= (Int *) *(a2+2);	/* pointe vers le nom de ftn */
			 if(isnotatom(a2)){popf; return;}
			 push(a1);	/* save corps de lambda */
			 a1=a2;
			 push(a1);  	/* le nom de la fonction */
			 if(TAIL!=0) metival10(a2,t);  
			 a2=atentree;	/* cherchons des choses a faire */
					 /* a l'entree de la fonction	*/
			 gett();
			 a4=car(a4);	/* la liste de choses 		*/
			 if(isnil(a4))
			   {if(isnotnil((Int *)*atSTEP) && *atstep==(Int)t)
			     {pop_cast(a1,(Int *));rec(EVst1,EVstend);return;}
			    else {pop_cast(a1,(Int *));pop_cast(a1,(Int *));popf;return;}}
			 a1=car(a4);	/* premiere chose a faire	*/
			 a5=cdr(a4);	/* reste des choses a faire	*/
			 push(a5);	/* qu'on garde pour la suite	*/
			 rec(eval,ENtree1); /* et on y va		*/
			 }


Int evexp_6()
{
  /* ici  savp = var1 : oval1 : ... : MRKEV : nom : oldpbind : reste
     p a3 =					             : reste
     a1   = la fval */


  aux1=(Int)unbind;
  TAIL=0;
  for(;;)
    {
      if(*p-- != aux1)
	{/* cas non tail-rec */
	  p=savp; pc=evexpn; return;
	}
      /* ici p = un bloc-pbind */
      switch(*p)
	{
	case LAMBDA :
	  /* ceci en vue des DICs */
	  a4= (Int *) *(p-1);
	  if(cdr(a1) == cdr(a4))	/* cas tail-rec, hack Saint-James */
	    {
	      *(p-1) = *(pbind-1); *(pbind-1)=(Int)a1;
	      a5= (Int *) *(pbind-2);savep= (Int *) *(a5+2); *(a5+2)= *(a3+2);
	      a5= (Int *) *(p-2);*(a5+2)=(Int) savep;
	      p=a3; a1=cdr(a1); 
	      if(*atcareful==(Int)t) {
		pushf(progn); TAIL=1; ENtree();return;
	      }
	      else  {
		a2= (Int *) *(p-3);if(isatom(a2)) metival10(a2,t); pc=progn; return;
	      }
	    }
	  p = (Int *) *(p-2); break;	/* et on continue a chercher */
	case PROTECT :
	case WHESC :
	  p=savp; pc=evexpn; return;
	}
    }
}


Int evexpn()
{				/* cas non tail-rec, used from evexp_5, fexpr
				   on termine le bloc lambda */
  a2 = (Int *)(*atcrewid = *(a3+2));	/* le nom de fonction, s'il existe */
  if(isatom(a2)){a5=getival10(a2);metival10(a2,nil);}
  else a5=nil;
  push(a5);      /* empiler l'ancien ival10 (pour tail-recursif-trace) */
  push(a3); push(a1); push(LAMBDA); pbind=p;
  a1=cdr(a1); 
  if(*atcareful==(Int)t){pushf(unbind);pushf(progn);ENtree();}
  else rec(progn,unbind);
}

Int evfexp(){/* traitements des fexprs */
	 pop_cast(a4,(Int *)); pc=evfexb;}

Int evmac(){/* traitement des macros */
	 pop_cast(a4,(Int *)); a1=form; rec(evfexb,eval);}

Int evfexb()/* suite des fexprs et macros */
{
  a2=car(a4); /* les arguments */ a3=p; /* pour evexpn */
  /* on construit une base de bloc lambda */
  push(pbind); 
  push(car(form)); /* le nom */
  push(MRKEV);
  while(islist(a2)){a5=car(a2); push(car(a5)); 
  *CAR(a5)=(Int)a1;
  push(a5); a1=nil; a2=cdr(a2);}
  if(isnotnil(a2)){
    push(nil); 
    *CAR(a2)=(Int)nil;
    push(a2);}	/* liste pointee */
  a1=a4; pc=evexpn;
}	/* fexprs et macros ne SONT PAS tail-recs */

Int apply_careful()
{			/* (apply f l)  subr 2 */
  if(isnotatom(a1)){		/* apply de non-subr */
#ifndef NO_VFLO
    if(isnum(a1) && isnotflo(a1)){pc=evaln_2;}
#else
    if(isnum(a1)){pc=evaln_2;}
#endif
    else
      if(car(a1) == lambda){
	push(cdr(a1)); form=a1; a1=a2; pc=apexp;}
      else
	{push(a2); rec(eval,apply_1);}
    return;
  }
  /* introduire la mise-a-jour.. pour la doc on-line */
  if(*atcareful==(Int)t && (pbind>0||*atcrewid!=(Int)nil) && islist(((struct atome *)a1)->fval))
    {
      if(!pbind) a5 = (Int *) *atcrewid;
      else{a5= (Int *) *(pbind-2);a5= (Int *) *(a5+2);}
      if(isatom(a5)) adapteival((Int *)4, a5,a1);
      adapteival((Int *)3, a1, a5);
    }
  form=a1; push(((struct atome *)a1)->fval); 
  a3=(Int*)(Int)(((struct atome *)a1)->ftyp); a1=a2;
  /* a1 = args, a3 = ftyp */
  pc=(Int(*)())tapply[(Int)a3];
}

Int apply_no_careful()
{			/* (apply f l)  subr 2 */
  if(isnotatom(a1)){		/* apply de non-subr */
#ifndef NO_VFLO
    if(isnum(a1) && isnotflo(a1)){pc=evaln_2;}
#else
    if(isnum(a1)){pc=evaln_2;}
#endif
    else
      if(car(a1) == lambda){
	push(cdr(a1)); form=a1; a1=a2; pc=apexp;}
      else
	{push(a2); rec(eval,apply_1);}
    return;}
  form=a1; push(((struct atome *)a1)->fval); 
  a3=(Int*)((Int)((struct atome *)a1)->ftyp); a1=a2;
  /* a1 = args, a3 = ftyp */
  pc=(Int(*)())tapply[(Int)a3];
}

Int apply_1()		/* used by udfa */
{
  pop_cast(a2,(Int *)); pc=apply;
}


Int udfa(){/* fonction indefinie dans apply */
  --p;
  push(a2); a2=cons(form,ncons(a1));
  a1=atindfun; rec(apply,apply_1);
}



Int apply2(){/* apply de subr2 : reviendra sur fval empilee */
	 a2=car((Int *)((struct atome *)a1)->pval); pc=carp;
}

Int apply3(){/* apply de subr3 */
	 a3=cdr(a1); a3=car((Int *)((struct atome *)a3)->pval); pc=apply2;
}

Int apexp()
{/* apply des exprs */
  pop_cast(a2,(Int *)); a3=p;	/* pour evexpn */
  /* on construit la base d'un bloc lambda, evexpn completera */
  push(pbind); 
  push(form);			/* la fonction */
  push(MRKEV); a4=car(a2);	/* les vars */
  while(islist(a4)){
    a5=car(a4); push(((struct atome *)a5)->cval); push(a5);
    ((struct atome *)a5)->cval=car(a1);
    a1=cdr(a1); a4=cdr(a4);}
  if(isnotnil(a4)){push(((struct atome *)a4)->cval); push(a4); 
  ((struct atome *)a4)->cval=a1;
  } /* var poInt */
  if(p>(Int*)hpil){pc=erpil; return;}
  a1=a2; pc=evexpn;
  /* consequence : une expr lancee par apply 
     n'EST PAS tail-recursive */
}

Int self()/* (self . l)  fsubr */
{
  for(;;)
    {
      if(pbind <= 0){a4=form;
      push(a1);
      a2=cons(a2,ncons(a1));
      rec(erself,evals_1);return;
      /*
	pc=erself;
	return;}  */}
      else {
	savp=pbind;
	switch(*savp)
	  {
	  case LAMBDA :
	    a5 = (Int *) *(savp-2); /* fin du bloc */
	    a5 = (Int *) *(a5+2);   /* le nom de la ftn */
	    if(isatom(a5)){a4=((struct atome *)a5)->fval;form=cons(a5,cdr(form));}
	    a4 = (Int *) *(savp-1); /* la fval */
	    pc=evexp_0; return;
	  case WHESC :
	    savp = (Int *) *(savp-2); break;
	  case PROTECT :
	    savp = (Int *) *(savp-1); break;
	    /* le cas CLOSURE ne peut pas arriver
	     * on tombera d'abord sur une lambda
	     * bloquante.
	     */
	  }}
    }
}

Int exitp(){/* (exit . l)  subr */
  for(;;){
    p=pbind; if(p <= 0){pc=erexit; return;}
    switch(*p){
    case LAMBDA :
      ++p; pc=progn; return;
    case WHESC :
      --p; pop_cast(pbind,(Int*)); pop_cast(a4,(Int *));
      pop_cast(((struct atome *)a4)->fval, (Int*)); 
      pop(((struct atome *)a4)->ftyp);}
  }
}


Int evesc(){/* traitement des escapes */
	 pop_cast(a4,(Int *)); /* la fval */ push(a2); /* le nom */
	 rec(progn,evesc_1);}

Int evesc_1(){
	 pop_cast(a2,(Int *)); /* le nom */
	 pc=unwind;
   }

extern Int undo_2();
Int undo_1(){
	 na1=valnb(a1);pop_cast(a1,(Int *));pc=undo_2;}

Int undo(){/* on delie un bloc quelconque et on entre dans un break */
	 if(!isnil(car(a1))) {push((cdr(a1)));a1=car(a1);rec(eval,undo_1);}
	 else {na1=1;a1=cdr(a1);pc=undo_2;}
}

Int undo_2(){
hop:	
  p=pbind;
  if(p <= 0){pc=erexit; return;}
  switch(*p)
    {
    case LAMBDA :
      savep=p-3;  /* le ival10 */
      for(p-=4;;){/* on retablit les valeurs */
	pop_cast(a2,(Int *)); if(a2 == (Int*)MRKEV)break; 
	pop(*CAR(a2));}
      pop_cast(a5,(Int *)); /* le nom de fonction */
      if(isatom(a5)) metival10(a5,*savep);
      pop_cast(pbind,(Int*));
      if(na1 <= 1) break;else {na1--;goto hop;}
      break;
    case WHESC :
      --p; pop_cast(pbind,(Int*)); pop_cast(a2,(Int *)); pop_cast(((struct atome *)a2)->fval,(Int*)); pop(((struct atome *)a2)->ftyp); 
      if(na1 <= 1) break;else {goto hop;}
      break;
    case CLOSURE :
      unbcls(); 
      if(na1 <= 1) break;else {goto hop;}
      break;
    case PROTECT :
      --p; pop_cast(pbind,(Int*)); pop_cast(a3,(Int *)); pushf(Break);
      push(a1); a1=a3; rec(progn,popa1); return;
    case LOCK :
      --p; pop_cast(pbind,(Int*)); --p; 
      if(na1 <= 1) break;else {goto hop;}
      break;
    }
  if(a1!=nil) pc=progn; else
    {a1=ncons(atundo);pushf(Break);derec;}
}

extern Int done_2();
Int done_1(){na1=valnb(a1);pop_cast(a1,(Int *));pc=done_2;}

Int done(){/* on delie un bloc quelconque et on entre dans un break */
	 if(!isnil(car(a1))) {push((cdr(a1)));a1=car(a1);rec(eval,done_1);}
	 else {na1=1;a1=cdr(a1);pc=done_2;}
}

Int done_2(){
hop:
  p=pbind;
  if(p <= 0){pc=erexit; return;}
  switch(*p)
    {
    case LAMBDA :
      savep=p-3;  /* le ival10 */
      for(p-=4;;){/* on retablit les valeurs */
	pop_cast(a2,(Int *)); if(a2 == (Int*)MRKEV)break; 
	pop(*CAR(a2));}
      pop_cast(a5,(Int *)); /* le nom de fonction */
      if(isatom(a5)) metival10(a5,*savep);
      pop_cast(pbind,(Int*));
      *atcrewid = (Int)((pbind) ? (Int*)*(((Int *)*(pbind-2))+2) : nil);
      if(na1 <= 1) break;else {na1--;goto hop;}
      break;
    case WHESC :
      --p; pop_cast(pbind,(Int*)); pop_cast(a2,(Int *)); pop_cast(((struct atome *)a2)->fval, (Int*)); pop(((struct atome *)a2)->ftyp); 
      if(pbind) {*atcrewid= *(pbind-2); *atcrewid= *((Int *)*atcrewid+2);}
      else *atcrewid=(Int)nil;
      if(na1 <= 1) break;else {goto hop;}
      break;
    case CLOSURE :
      unbcls(); 
      if(na1 <= 1) break;else {goto hop;}
      break;
    case PROTECT :
      --p; pop_cast(pbind,(Int*)); pop_cast(a3,(Int *)); pushf(Break);
      *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
      push(a1); a1=a3; rec(progn,popa1); return;
    case LOCK :
      --p; pop_cast(pbind,(Int*)); --p; 
      *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
      if(na1 <= 1) break;else {goto hop;}
      break;
    }
  if(a1!=nil) pc=progn; else
    {a1=ncons(atdone);pushf(Break);derec;}
}

Int unwind(){/*  (unwind)  subr 0 */

  if(external_buffer){ /* netoyer une entree externe */
    XtFree(external_buffer);
    external_ptr = NULL;
  }
  for(;;)
    {
      if(pbind <= 0){pc=reent; return;}
      p=pbind;
      switch(*p)
	{
	case LAMBDA :
	  p-=4; pop_cast(a3,(Int *));
	  while(a3 != (Int*)MRKEV){
	    pop(*CAR(a3));
	    pop_cast(a3,(Int *));}
	  pop_cast(a5,(Int *));	/* depiler le nom de fonction */
	  if(isatom(a5)){ /* re-adapter le ival */
	    metival10(a5,nil);}
	  *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
	  pop_cast(pbind,(Int*)); continue;
	case WHESC :
	  if(*(p-2) != (Int)a2){
	    --p; pop_cast(pbind,(Int*)); pop_cast(a3,(Int *));
	    *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
	    pop_cast(((struct atome *)a3)->fval,(Int*)); pop(((struct atome *)a3)->ftyp); continue;
	  }
	  pc=unbind; return;
	case CLOSURE :
	  unbcls(); continue;
	case PROTECT :
	  --p; a3=a1; pop_cast(pbind,(Int*)); pop_cast(a1,(Int *)); push(a2);  /* le-nom */
	  *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
	  pushf(evesc_1); push(a3); rec(progn,popa1); return;
	case LOCK :
	  --p; pop_cast(pbind,(Int*)); a3=a1; pop_cast(a1,(Int *));
	  *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
	  /* a1= fonc-2-args, a2=nom, a3=val-a-ramener */
	  a2=cons(a2,ncons(a3)); pc=apply; return;
	}
    }
}

Int unwinp(){/* (unwind-protect e . l)  fsbur */
	 push(cdr(a1)); push(pbind); push(PROTECT); pbind=p;
	 rec(evca1,unbind);
}

Int escape(){/* (escape nom . l)  fsubr */
	 a4=(Int*)ESCAPE; a2=a3=car(a1); a1=cdr(a1); pc=wheresc;}

Int letf(){/* (letf (nom args . corps) . l)  fsubr */
	 a4=(Int*)EXPR; a2=car((Int *)((struct atome *)a1)->cval); a3=cdr((Int *)((struct atome *)a1)->cval);
	 a1=cdr(a1); pc=wheresc;
}

Int wheresc(){/* a1 : corps a executer, a2 : nomfonc, a4 : newftyp 
	   * a3 : new-definition en cas de letf
	   */
  push(((struct atome *)a2)->ftyp); push(((struct atome *)a2)->fval);		/* sauver les vieux */
  aux1=(Int)a4; ((struct atome *)a2)->ftyp=(CHAR)((Int)aux1); ((struct atome *)a2)->fval=a3;
  push(a2); push(pbind); push(WHESC); pbind=p;
  rec(progn,unbind);}

Int SOrtie1(){
  Int *auxi,*auxi1;
  p=pbind;
  savep=p-3; /* le ival10 */
  for(p-=4;;){/* on retablit les valeurs */
    pop_cast(a2,(Int *)); if(a2 == (Int*)MRKEV)break; 
    pop(*CAR(a2));}
  pop_cast(a5,(Int *));
  if(isatom(a5)) metival10(a5,*savep);
  pop_cast(pbind,(Int*)); 
  *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
  derec;}

Int changeval(){ /* subr 1 */
    *(atsortie+1)=(Int)t;
    *atsortie=(Int)a1;derec;
}

 /* XXXXX ATTENTION, harald */
Int SORree1()
{
  pop_cast(a5,(Int *));
  if(isnil(a5)){ pop_cast(a1,(Int *)); 
  if(isnotnil((Int*)*atSTEP) && *atstep==(Int)t) {
    pop_cast(a5,(Int *));
    if(*(atsortie+1)==(Int)t) {
      *(atsortie+1)=(Int)nil;
      if(a5!= (Int *) *atsortie) {
	a5= (Int *) *atsortie;
	push(a5);
      }
    }
    rec(EVst1,EVstend);return;
  }
  else {
    pop_cast(a1,(Int *));
    if(*(atsortie+1)==(Int)t) {*(atsortie+1)=(Int)nil;
    if(a1!= (Int *) *atsortie) a1= (Int *) *atsortie;
    }
    popf;return;
  }
  }
  a1=car(a5);push(cdr(a5));rec(eval,SORree1);
}

Int SOrtie()
{
  a2= (Int *) *(pbind-2);	/* pointe vers fin de bloc */
  a2= (Int *) *(a2+2);	/* pointe vers le nom de ftn */
  if(isnotatom(a2)){popf; return;}
  push(a1);	/* save valeur */
  a1=a2;
  push(a1);	/* le nom de la fonction */
  a2=atsortie;	/* cherchons des choses a faire */
  /* a la sortie de la fonction	*/
  gett();
  a4=car(a4);	/* la liste de choses 		*/
  if(isnil(a4)){pop_cast(a1,(Int *));pop_cast(a1,(Int *));popf;return;}
  *atsortie= *(p-1);
  a1=car(a4);	/* premiere chose a faire	*/
  a5=cdr(a4);	/* reste des choses a faire	*/
  push(a5);	/* qu'on garde pour la suite	*/
  rec(eval,SORree1); /* et on y va		*/
}


Int unbind()
{/* on delie un bloc quelconque */
  p=pbind;
  switch(*p)
    {
    case LAMBDA :
      if(*atcareful==(Int)t) {pushf(SOrtie1);SOrtie();return;}
      else {
	savep=p-3;  /* le ival10 */
	for(p-=4;;){/* on retablit les valeurs */
	  pop_cast(a2,(Int *)); if(a2 == (Int*)MRKEV)break; 
	  pop(*CAR(a2));}
	pop_cast(a5,(Int *));	/* depiler le nom de fonction */
	if(isatom(a5)) /* re-adapter le ival */
	  metival10(a5,*savep);
	pop_cast(pbind,(Int*));
	*atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
	derec;}
    case WHESC :
      --p; pop_cast(pbind,(Int*)); pop_cast(a2,(Int *)); pop_cast(((struct atome *)a2)->fval, (Int*)); 
      pop(((struct atome *)a2)->ftyp); 
      *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
      derec;
    case CLOSURE :
      unbcls(); derec;
    case PROTECT :
      --p; pop_cast(pbind,(Int*)); pop_cast(a3,(Int *)); push(a1); a1=a3; 
      *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
      rec(progn,popa1);
      return;
    case LOCK :
      --p; pop_cast(pbind,(Int*)); --p; 
      *atcrewid = (Int) ((pbind) ? (Int *)*(((Int *)*(pbind-2))+2) : nil);
      derec;
    }
}

Int evcls(){/* evaluation des closures. a2 = (*cls* aliste fonc), a1 = largs */
  /* constitution d'un bloc CLOSURE */
  /* il est CRUCIAL que toute la forme soit conservee pour SELF */
  /* pour cette raison "*cls* ne doit pas etre une subr */
  a5=a2;				/* on garde pour SELF */
  a2=cdr(a2); a3=car(a2);		/* la a-liste */
  push(pbind); push(MRKEV);
  while(isnotnil(a3)){
    a4=car(a3); a3=cdr(a3);		/* a4 = le couple */
    push(car((Int *)((struct atome *)a4)->cval)); push(car(a4));
    *CAR((Int *)((struct atome *)a4)->cval)=(Int)cdr(a4);}
  push(car(atSELF)); push(atSELF); ((struct atome *)atSELF)->cval=a5;
  push(car(a2));
  push(CLOSURE); pbind=p;
  a2=car(((struct atome *)a2)->pval);		/* la fonction */
  rec(evalfu,unbind);
}

Int unbcls(){/* deliaison des closures : vraie routine appellee par
	    evesc_1 et unbind, comme "unbcls();" */
  --p; pop_cast(a3,(Int *));			/* la a-liste */
  while(isnotnil(a3)){
    a4=car(a3); a3=cdr(a3);	/* a4 = le couple */
    /*	  cdr(a4)=car((Int *)a4->cval);}*/	/* on referme la valeur de la var */
    *CDR(a4)=(Int)car((Int *)((struct atome *)a4)->cval);}
  for(;;){pop_cast(a2,(Int *)); if(a2==(Int*)MRKEV)break; 
  pop(*CAR(a2));}
  pop_cast(pbind,(Int*));
}

Int internal(){/* (internal f . l)  nsubr */
	 a3=car(a1);
	 push(((struct atome *)a3)->ofval); a3=(Int*)((Int)(((struct atome *)a3)->oftyp)); a1=cdr(a1);
	 pc=(Int(*)())tapply[(Int)a3];
}

Int intern_1(){/* (internalprInt . l)  nsubr */
	 rec(prin1,terpri);
}

Int closure(){/* (closure lvars fonc)  subr 2 */
	 for(a3=nil; isnotnil(a1) ;
		     a3=cons(cons(car(a1),car((Int *)((struct atome *)a1)->cval)),a3), a1=cdr(a1));
	 a1=cons(atcls,cons(a3,ncons(a2)));
	 derec;
}

Int lockp(){/* (lock f . l)  fsubr */
	 push(car(a1)); push(pbind); push(LOCK);
	 pbind=p; a1=cdr(a1); rec(progn,unbind);
}

Int letdic(){/*  (letdic arbre vals . corps)  fsubr */
	 push(cdr(a1)); rec(evca1,letdi_1);
}

Int letdicq(){/*  (letdicq arbre vals . corps)  fsubr */
	 push(cdr(a1)); a1=car(a1); pc=letdi_1;
}

Int letdi_1(){
	 pop_cast(a2,(Int *)); push(a1); push(cdr(a2)); a1=a2; rec(evca1,letdi_2);
}

Int letdi_2(){
  a2=a1; pop_cast(a3,(Int *)); pop_cast(a1,(Int *));
  a4=p; push(pbind); 
  push(cons(a1,a3));
  push(MRKEV); desetp(a1,a2); savp=p;
  p=a4; a1=cons(a1,a3); a3=a4;
  pc=evexp_6;
}

desetp(x,y)/* used by letdi_2 */
Int *x, *y;
{
  while(islist(x))
    {
      if(isatom(car(x)))
	{
	  push(car(((struct atome *)x)->cval)); push(car(x)); 
	  /*	    car(((struct atome *)x)->cval)=car(y);		*/
	  *CAR(((struct atome *)x)->cval)=(Int)car(y);
	}
      else
	desetp(car(x),car(y));
      x=cdr(x); y=cdr(y);
    }
  if(isnotnil(x)){push(car(x)); push(x); 
  /*			car(x)=y;}	*/
  *CAR(x)=(Int)y;}
}

Int frame()/*  (frame n n-only)  subr 2 (n-only!=nil signifie uniquement le n-ième de la frame) */
{
  Int nna1, Onna1; Int *pp;

  nna1=(isnil(a1)) ? 1 : valnb(a1);
  Onna1 = (a2 == nil);
  pp=pbind; a1=nil;
  while(pp != 0 && nna1-- > 0)
    switch(*pp)
      {
      case LAMBDA:
	a3=lambda; a4= (Int *) *(pp-1); 
	pp-=4;
	a2=nil;
	while(*pp != MRKEV)
	  {
	    if(nna1 == 1 || Onna1) a2=cons(cons((Int*)*pp,(Int*)*(pp-1)),a2); 
	    pp-=2;
	  }
	a3= (Int *) *(--pp); 	/* le nom de la fonction */
	pp= (Int *) *(--pp);
	if(nna1 == 1 || Onna1) 
	  a4=(isatom(a3)) ? cons(a3,ncons(a2)) : cons(lambda,cons(a4,ncons(a2)));
	goto suite;
      case WHESC:
	a3=atescape;
	if(nna1 == 1 || Onna1)
	  a4=cons(a3,cons((Int*)*(pp-2),cons((Int*)*(pp-3),ncons((Int*)*(pp-4)))));
	pp= (Int *) *(pp-1);
	goto suite;
      case CLOSURE:
	a3=atclosure; a4= (Int *) *(pp-1); pp-=2;
	a2=nil;
	while(*pp != MRKEV)
	  {
	    if(nna1 == 1 || Onna1) a2=cons(cons((Int*)*pp,(Int*)*(pp-1)),a2); 
	    pp-=2;
	  }
	pp= (Int *) *(--pp);
	if(nna1 == 1 || Onna1) a4=cons(a3,cons(a4,ncons(a2)));
	goto suite;
      case PROTECT:
	a3=atprotect; goto lockpro;
      case LOCK:
	a3=atlock;
      lockpro:
	if(nna1 == 1 || Onna1) a4= cons(a3,ncons((Int*)*(pp-2))); 
	pp= (Int *) *(pp-1);
      suite:
	if(nna1 == 1 && !Onna1) a1 = a4;
	else if(Onna1) a1=cons(a4,a1);
      }
fini:
  if(!Onna1) { derec; }
  else { a2=nil; pc=nreverse; }
}

Int oval(){/*  (oval at [val])  nsubr */
	 a2=car(a1);
	 if(isnil(cdr(a1)))a1=((struct atome *)a2)->oval;
	 else{((struct atome *)a2)->oval=car(((struct atome *)a1)->pval); a1=a2;}
	 derec;
}

Int unbou(){/*  (unboundp at)  subr 1 */
	 a1=(*a1 == (Int)undef) ? t : nil;
	 derec;
}
