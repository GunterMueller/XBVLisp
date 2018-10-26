
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	spec.c		fonctions speciales
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 7. Feb. 83
 */

#include <vlisp.h>
#include <sys/times.h>

Int getpackage(){/* (getpackage atome) subr1 */
  if(isatom(a1)) a1=((struct atome *)a1)->pakage; else a1=nil;
  derec;
}

Int step(){/* (step ftn n avant/apres) NSUBR */
  *atstep=(Int)nil;
  if(isnotnil(car(a1))) {*atSTEP=(Int)cons(car(a1), (Int *) *atSTEP);
			 a2=car(a1);((struct atome *)a2)->ofval=0;}
  else {*atSTEP=(Int)(a1=t);derec}
  a2=cdr(a1);
  a1=car(a1); /* la fonction */
  if(isnil(a2)) {derec;}
  /* le numero */
  a3=car(a2);
  if(isnotnil(a3))  {a3=(Int *)valnb(a3);((struct atome *)a1)->ofval=a3;}
  a2=cdr(a2); 
  /* si c'est des fonctions utilisateurs avant/apres */
  if(isnil(a2)) derec;
  *atstep=(Int)car(a2);derec;
}

Int unstep1(){
  a2=a1;pop_cast(a1,(Int *));
  if(isnil(a1)){*atSTEP=(Int)(a1=a2);derec;}
  else {push(cdr(a1));a1=car(a1);rec(delq,unstep1);}
}

Int unstep(){/* (unstep ftn1 .. ftn-n) FSUBR */
  if(isnil(a1))
    {*atSTEP=(Int)(((struct atome *)atSTEP)->pval=((struct atome *)atSTEP)->ival=nil);
     ((struct atome *)atSTEP)->ofval=0;derec;}
  else {push(cdr(a1));a1=car(a1);a2= (Int *)*atSTEP;rec(delq,unstep1);}
}
	
Int pour1(){
  if(isnil(a1)) {pop_cast(a2,(Int *));derec;}
  else {pop_cast(a1,(Int *));pc=progn;return;}
}

Int pour(){/* (pour nom act1 act2 .. act-n) FSUBR */
  if(isatom(car(a1)) && ateval==car(a1)){a1=cdr(a1);pc=progn;return;}
  else if(islist(car(a1)))
    {push(cdr(a1));a2=car(a1);a1=ateval;rec(memq,pour1);}
  else {a1=nil;derec;}
}

/* 
 *  E.S: On fait un atome autocote de l'atome contenant le
 *  package si undef,  on evite que l'atome 
 *  ne soit pas enlever dans le prochain garvage colector 
 */
Int package(){/* (package nom/nil) FSUBR */
  if(isnil(a1)) *atpackage=(Int)nil;
  else {
    a1 = car(a1);
    if(((struct atome *)a1)->cval == undef)
      ((struct atome *)a1)->cval = a1;
    *atpackage=(Int)a1;
  }
  derec;
}

Int ibase(){/* (ibase n) SUBR1 */
  if(a1==nil) 
    a1= ((struct atome *)atibase)->cval;
  else if((na1=valnb(a1))<63 && na1>1)
    ((struct atome *)atibase)->cval=a1;
  else a1=nil;
  derec;
}

Int obase(){/* (obase n) SUBR1 */
  if(a1==nil) 
    a1= ((struct atome *)atobase)->cval;
  else if((na1=valnb(a1))<63 && na1>1)
    ((struct atome *)atobase)->cval=a1;
  else a1=nil;
  derec;
}

Int macout(){
  TRACE=0;a1=0;derec;
}

Int nomacout(){
  TRACE=1;a1=(Int *)1;derec;
}

Int boundp(){/* (boundp x) SUBR1 */
	if(((struct atome *)a1)->cval==undef) a1=nil; else a1=t;
	derec;
}

Int isuser(){/* (isuser x) SUBR1 */
  if(a1>atuser) a1=t; else a1=nil;derec;
}

Int getcar(){/* (getcar x) SUBR1 */
#ifndef NOANNOT
  if(!isnum(*(a1)) && ((UInt)*a1 & (UInt)ANNOT))
    a1=(Int *)((UInt)*a1 & (UInt)~ANNOT);
  else 
#endif
    a1= nil;
  derec;
}

Int clearcar(){/* (clearcar x) SUBR1 */
#ifndef NOANNOT
  if(!isnum(*(a1)) && ((UInt)*a1 & (UInt)ANNOT))
    {a2=(Int *)((UInt)*a1 & (UInt)~ANNOT);
     a2=car(a2);*a1=(Int)a2;}
#endif
  derec;
}
 
Int getcdr(){/* (getcar x) SUBR1 */
#ifndef NOANNOT
  if(!isnum(*(a1+1)) && ((UInt)*(a1+1) & (UInt)ANNOT))
    a1=(Int*)((UInt)*(a1+1) & (UInt)~ANNOT);
  else 
#endif
    a1= nil;
  derec;
}

Int clearcdr(){/* (clearcdr x) SUBR1 */
#ifndef NOANNOT
  if(!isnum(*(a1+1)) && ((UInt)*(a1+1) & (UInt)ANNOT))
    {a2=(Int*)((UInt)*(a1+1) & (UInt)~ANNOT);
     a2=car(a2);*(a1+1)=(Int)a2;}
#endif
  derec;
}

Int putcdr(){/* (getcdr x y) SUBR2 */
#ifndef NONANNOT
  if(!isnum(*(a1+1)) && ((UInt)*(a1+1) & (UInt)ANNOT))
    {a1=(Int*)((UInt)*(a1+1) & (UInt)~ANNOT);
     a2=cons(a2,cdr(a1));
     *CDR(a1)=(Int)a2;}
  else{a2=cons((Int *)*(a1+1),ncons((Int *)a2));
       *CDR(a1)=((UInt)a2 | (UInt)ANNOT);
       a1=a2;}
#endif
  derec;
}
Int putcar(){/* (getcar x y) SUBR2 */
#ifndef NOANNOT
  if(!isnum(*(a1)) && ((UInt)*a1 & (UInt)ANNOT))
    {a1=(Int*)((UInt)*a1 & (UInt)~ANNOT);
     a2=cons(a2,cdr(a1));
     *CDR(a1)=(Int)a2;}
  else{a2=cons(car(a1),ncons(a2));
       *CAR(a1)=((UInt)a2 | (UInt)ANNOT);
       a1=a2;}
#endif
  derec;
}

Int gsynum(){/* vraie routine : appelee comme "gsynum();" */
  /* also used by strcat */
  register Int r;
  r=na1%10; na1/=10; if(na1)gsynum(); *ppn++ = (r + '0');
}


Int gensym(){/* (gensym . l)  subr n */
  if(isnotnil(a1)){rec(strcatp,crato); return;}
  newppn();
  *ppn++ = 'g'; *ppn++ = '0'; *ppn++ = '0';
  na1= ++ngsym; gsynum();
  *ppn++=0;
  if(ppn>hstr)gcstrsv();
  int_p1=(Int *)oppn; *int_p1=ppn-oppn-sizeof(Int); pc=crato;
}

Int status(){/* (status at n)  fsubr */
  /* (status prInt n) : colle les bits de n dans opti.
     n=1 : bit 0 : imprimer les '"' des chaines.
     n=2 : bit 1 : pas d'espace avant impression.
     n=3 : bit 1 ET bit 0.
     n=0 : tous les deux a zero.
     
     (status print) : ramene la valeur de opti.
     
     (status toplevel n) : colle les bits de n dans optop
     n=1 : bit 0 : imprimer la ligne lue.
     */
  
  a3=car((Int *)((struct atome *)a1)->pval);	/* le 2d argument */
  
  if(car(a1) == atprint){if(isnotnil(a3))opti=(Int)a3;	a1=(Int*)opti;}
  else
    if(car(a1) == attop){ if(isnotnil(a3))optop=(Int)a3;a1=(Int*)optop;}
  derec;
}

Int randomp(){/* (random n)  subr 1 */
  if(isnil(a1)){
    srand((unsigned int)time(NULL));
  }
  a1=cranb(rand() % valnb(a1));
  derec;
}

Int break1();static Int *AUX;

Int break3(){
  AUX=a1;derec;
}

Int break2(){
  a2= (Int*)*p;push(a1);rec(equal,break1);
}

#if 0     
Int restart(){
  /* lastwin(); */
  *atprompt=atlst;
  if(pbind<=0) pc=unwind;
  else {savp=pbind;switch(*savp)
	  {case LAMBDA :
	     a5= *(savp-2);a5= *(a5+2); /* le nom */
	     if(isatom(a5)){a4=((struct atome *)a5)->fval;form=cdr(a4);}
	     a4= *(savp-1);a1=cdr(a4);
	     pc=progn;return;
	   case WHESC : savp= *(savp-2);break;
	   case PROTECT : savp= *(savp-1);break;}}
}
#endif
     
Int break1(){
  if(a1== t){--p;--p;pop_cast(a2,(Int *));*atprompt=(Int)a2;a1=AUX;derec;}
  else pop_cast(a1,(Int *));
  if(a1==atminus){--p;pop_cast(a2,(Int *));*atprompt=(Int)a2;pc=unwind;}
  else {pushf(break2);pushf(readu);
	pushf(break3);pushf(terpri);rec(eval,prini);}
}

Int Break(){/* petite toplevel interne, (break nom) fsubr */
  a2= (Int*)*atprompt;
  a1=car(a1);
  push(a2);*atprompt=(Int)a1;push(a1);
  rec(readu,break2);
}

Int wint(n) Int n;{/* sortir l'entier n */
  CHAR buf[40];
  
  sprintf(buf,"%d",n);
  wst(buf);
  /*wprintw(WWW,"%d",n);*/
}


Int forkp(){/* (fork ) subr 0 */
  register Int temp;
  static Int frk_cnt=0;
  temp=fork(); a1=cranb(temp); 
  derec;
}

Int background(){/* (background . x) fsubr */
  register Int temp;
  temp=fork();
  if(temp==0) {
    signal(SIGINT,SIG_IGN);
    a1=cons(atprogn,a1);pushf(stop);pc=eval;}
  else {a1=cranb(temp);derec;}
}

Int waitp(){/* (wait n) subr 1 */
  register Int temp;
  temp=wait(0);
  if(temp==-1) {a1=nil;derec;}
  if(temp==(Int)a1) {a1=t; derec;}
  else waitp();
}

Int itoap(){/* (itoa n)  subr 1 */
  a1=(Int*)valnb(a1); pc=readc_1;
}

Int atoip(){/* (atoi c)  subr 1 */
  if(isatom(a1))a1=((struct atome *)a1)->pn;
  pchar=(CHAR *) (a1+(2/2));	a1=cranb((Int)*pchar); derec;
}

#if 0
	/* A RE-VOIR */
Int ctrlz(){/* (ctrlz e)  subr 1 */
  puts("ctrlz appele");
  derec;
}
#endif

Int remob(){/* (remob at)  subr 1 */
  if(a1 < atuser)a1=nil;
  else
    {
      if(a1 == derat)derat=(Int *)((struct atome *)a1)->nexa;
      else
	{
	  for(a5=derat; (Int *) ((struct atome *)a5)->nexa != a1; 
	      a5=(Int *) ((struct atome *)a5)->nexa);
	  ((struct atome *)a5)->nexa=((struct atome *)a1)->nexa;
	}	  
      ((struct atome *)a1)->nexa=(struct atome *)freeat; freeat=a1;
      enleve_atome((ATOMEPTR)a1); /* l'enlever de la hash-table */
      /* a1=t; retourne plutot l'atome enleve en valeur */
    }
  derec;
}

Int timep(){/*  (time)  subr 0 */
  struct tms temps; long t_u;
  
  times(&temps);
  t_u=temps.tms_utime * 20L;
  na1=(Int)(t_u / 60000L); a1=cranb(na1);		/* mins */
  t_u%=60000L;
  na1=(Int)(t_u / 1000L); a2=cranb(na1);		/* secs */
  na1=(Int)(t_u % 1000L); a3=cranb(na1);		/* msecs */
  a1=cons(a1,cons(a2,ncons(a3)));	derec;
}

Int stol(){/*  (stol at)  subr 1, Ernesto: defini subr 2 */
  Int *x;
  a2=a1;
  a1=nil;
  for(x=p; x >= (Int *)bpil; x--)a1=cons(cranb(*x),a1);
  if(isnotnil(a2))*a2=(Int)a1;
  derec;
}

Int ltos(){/*  (ltos l f)  subr 2 */
  for(p = (Int *)(bpil-1); isnotnil(a1);)
    {*++p=valnb(car(a1)); a1=cdr(a1);}
  a1=a2; a2=nil; pc=apply;
}

