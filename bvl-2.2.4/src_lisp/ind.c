/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*                        pour les indexations .. etc...	*/
/*               		Harald Wertz		*/
/*		                Decembre 82		*/


/* pour les VARIABLES:
	globalvar-> var est variable globale
	localvar -> var est variable locale
	incr	 -> var est variable globale modifiee
   pour les FONCTIONS:
	0 -> liste de toutes les variables locales
	1 -> liste de toutes les variables globales
	2 -> liste des variables globales modifiees
	3 -> liste des fonctions appellantes
	4 -> liste des fonctions appellees
	5 -> liste des clauses appellantes
	6 -> liste des clauses appelles (pour les clauses)
	7 -> liste des fonctions appellees dans une clause
	20 -> fichier definissant la fonction
	100 -> temporaire de construction pour les fonctions
	201 -> temporaire de construction pour les fonctions
   Variables Globales accessibles en LISP:
	globalvar -> liste des variables globales
	localvar -> liste des variables locales
	all_functions -> liste des fonctions definie sous careful=t
	all_clauses -> liste des clauses definie sous careful=t
*/

#include <vlisp.h>

Int *la_fonction;
Int indprog=0; /* indicateur de PROG */

/* clr2 enleve toute les occurences de a1 (la_fonction) de l'interieur 	*/
/* de tous les listes d'un ival deja construit...			*/

Int clr2(la_liste) Int *la_liste;
{
  Int *auxi2,*auxi3;
  while(islist(la_liste)){
    auxi2=car(la_liste); 
    /* correction de D.P. : on avait *la_liste=(Int)cdr(la_liste);, ce qui boucle... */
    while(islist(la_liste) && isnotlist(auxi2)) { la_liste=cdr(la_liste); auxi2=car(la_liste); }
    if(!islist(la_liste)) break;
    if(car(auxi2)==atlocalvar || car(auxi2)==atglobalvar ||
       car(auxi2)==atincr || car(auxi2)==(Int*)3)
      {
	auxi3=cdr(auxi2);
	while(islist(auxi2)){
	  if(a1==car(auxi3)){
	    *CDR(auxi2)=(Int)cdr(auxi3);
	    break;
	  }
	  auxi2=auxi3;
	  auxi3=cdr(auxi2);
	}
      }
    la_liste=cdr(la_liste);
  }
}

/* clr3 attend une liste de variables. Il enleve dans le ival de chacune */
/* des variables toutes les occurences de a1 (la_fonction)		 */

clr3(auxi) Int *auxi;
{
  Int *auxi1;
  while(islist(auxi)){
    a4=car(auxi);
    auxi1=((struct atome *)a4)->ival;
    clr2(auxi1);
    auxi=cdr(auxi);
  }
}

Int clrocc()
{/* enleve toute les refs a la fonction */
  Int *auxi;
  auxi=(Int*) *atlocalvar;
  clr3(auxi);
  auxi=(Int*) *atglobalvar;
  clr3(auxi);
  auxi=(Int*) *atall_functions;
  clr3(auxi);
}

Int clear1a(la_liste) Int *la_liste;
{/* petite fonction auxiliaire enlever les ivals (pour '(careful 1)') */
  Int *le_atom;
  while(islist(la_liste)){
    le_atom= car(la_liste);
    ((struct atome *)le_atom)->ival=nil;
    la_liste= cdr(la_liste);
  }
}

Int clearall(){/* remettre a 0 tous les variables etc... */
  Int *la_liste;
  la_liste=(Int*) *atlocalvar;
  clear1a(la_liste);
  *atlocalvar=(Int)nil;
  la_liste=(Int*) *atglobalvar;
  clear1a(la_liste);
  *atglobalvar=(Int)nil;
  la_liste=(Int*) *atall_functions;
  clear1a(la_liste);
  *atall_functions=(Int)nil;
}


Int *
funion(le_atom,la_liste,indic) /* test et fait l'union */
Int *le_atom;
Int *la_liste;
Int indic;
{
  Int *auxi1;
  Int *auxi;
  if(indic==1) {
    auxi1= la_liste;
    la_liste=(Int*) *la_liste;
  }
  auxi=la_liste;
  while(islist(la_liste)){
    if(le_atom == car(la_liste)) 
      if(indic==0) return(auxi);
      else return(auxi1);
    la_liste = cdr(la_liste);
  }
  if(indic==1) {
    *auxi1= (Int)cons(le_atom,(Int*)*auxi1);
    return(auxi1);
  }
  else return(cons(le_atom,auxi));
}

/* occurloc est une sorte de memq */

occurloc(ax1,ax2) Int *ax1; 
Int *ax2;
{
  while(islist(ax2))
    {	
      if(ax1== car(ax2)) return(1);
      ax2=cdr(ax2);
    }
  return(0);
}

adapteival(indic,variabl1,variabl2) 
     Int *indic; 
     Int *variabl1; 
     Int *variabl2;
{				/* doit adapter le ival des atomes ... */
  Int *auxi,*auxi1;
  signal(SIGINT,SIG_IGN);
  auxi=find((Int)indic,((struct atome *)variabl1)->ival);
  if(auxi==0)
    {
      auxi1=ncons(variabl2);
      auxi=cons(indic,auxi1);
      ((struct atome *)variabl1)->ival=cons(auxi,((struct atome *)variabl1)->ival);
    }
  else
    {
      auxi1=car(auxi);
      auxi1=cdr(auxi1);
      auxi1=funion(variabl2,auxi1,0);
      auxi1=cons(indic,auxi1);
      *CAR(auxi)=(Int)auxi1;
    }
  signal(SIGINT,(void(*)(int))onintr);
}

Int testpro()
{ /* fonction de test ... */
  /* la_fonctio = le nom de la fonctio */
  /* a2 = le corps de la fonction */
  /* a3 = la liste de variables locales */
  Int IND;
  Int *expression,*TYPE,*vraitype;
  Int *auxi,*auxi1,*auxi2;
  while(islist(a2)){	/* prendre une expression apres l'autre */
    IND=0;
    expression= car(a2);
    a2=cdr(a2);
    if(isstr(expression) || isnum(expression) ||
       (isatom(expression) && expression <= atuser))
      continue;
    if(isatom(expression))
      {
	if(indprog==0)
	  if ((na1=occurloc(expression,a3))==1)
	    /* occurloc ramene 1 si expression ds localvars */
	    continue;
	  else {
	    funion(expression,atglobalvar,1);
	    adapteival(atglobalvar,expression,la_fonction);
	    adapteival((Int*)1,la_fonction,expression);
	  }
	continue;
      }
    if(isatom(car(expression))){/* peut-etre y'a un type */
      vraitype=car(expression);
      auxi=find(200,((struct atome *)vraitype)->ival);
      if(auxi!=0) {auxi=car(auxi);
      auxi=cdr(auxi);TYPE=car(auxi);}
      else TYPE=nil;
    }
    if(car(expression)==atprog || TYPE==atprog) {/* cas d'un PROG */
      expression=cdr(expression);
      auxi=car(expression);
      push(a3);
      push(a2);
      expression=cdr(expression);
      indprog=1;
      while(islist(auxi)){
	if(isatom(car(auxi))){
	  funion(car(auxi),atlocalvar,1);
	  a3=funion(car(auxi),a3,0);
	  adapteival(atlocalvar,car(auxi),la_fonction);
	  adapteival(0,la_fonction,car(auxi));}
	auxi=cdr(auxi);
      }
      if(auxi!=nil){
	funion(auxi,atlocalvar,1);
	a3=funion(auxi,a3,0);
	adapteival(atlocalvar,auxi,la_fonction);
	adapteival(0,la_fonction,auxi);
      }
      a2=expression;
      testpro();
      pop_cast(a2,(Int *));
      pop_cast(a3,(Int *));
      indprog=0;
      continue;
    }
    if(car(expression)==quote || car(expression)==atqua
       || TYPE==quote) 
      /* ne rien faire */
      {
	auxi=cdr(expression);
	auxi1=car(auxi);
	if(car(auxi1)==lambda){
	  push(a2);
	  a2=auxi;
	  testpro();
	  pop_cast(a2,(Int *));
	}
	continue;
      }
    if(car(expression)==atescape || TYPE==atescape) {/* cas escape */
      expression=cdr(expression);
      push(a2);
      a2=cdr(expression);
      testpro();
      pop_cast(a2,(Int *));
      continue;
    }
    if(car(expression)==atcond || car(expression)==atsele
       || TYPE==atcond || TYPE==atsele){
      push(a2); /* prepare l'appel recursif */
      if(car(expression)==atsele || TYPE==atsele) IND=1;
      expression=cdr(expression);
      if(IND==1){
	push(IND);
	push(expression);
	a2=car(expression);
	a2=ncons(a2);
	testpro();
	pop_cast(expression,(Int*));
	pop(IND);
	expression=cdr(expression);
      }
      while(islist(expression)){
	a2=car(expression);
	if(IND==1) a2=cdr(a2);
	expression=cdr(expression);
	push(IND);
	push(expression);
	testpro();
	pop_cast(expression,(Int*));
	pop(IND);
      }
      pop_cast(a2,(Int *));
      continue;
    }
    if(car(expression)==atlet || TYPE==atlet) /* c'est un let */
      {
	Int mflag = False;
	expression=cdr(expression);
	auxi=car(expression);
	expression=cdr(expression);
	push(a3);
	push(a2);
	push(expression);
	while(islist(auxi)){
	  /* pour traiter (let (x y )... 
	     le probleme subsiste si 'y' est une liste donc y 
	     est vu comme une liste */
	  if(islist(car(auxi)))
	    auxi1 = car(auxi);
	  else {
	    mflag = True;
	    auxi1 = auxi;
	  }
	  if(isatom(car(auxi1))){
	    funion(car(auxi1),atlocalvar,1);
	    a3=funion(car(auxi1),a3,0);
	    adapteival(0,la_fonction,car(auxi1));
	    adapteival(atlocalvar,car(auxi1),la_fonction);}
	  push(auxi);
	  a2=cdr(auxi1);
	  testpro();
	  pop_cast(auxi,(Int*));
	  if(mflag == True) auxi = nil;
	  else auxi=cdr(auxi);
	}
	pop_cast(a2,(Int *)); /* cdr(expression)->a2 */
	testpro();
	pop_cast(a2,(Int *));
	pop_cast(a3,(Int *));
	continue;
      }
    if(car(expression)==lambda || TYPE==lambda || TYPE==atde) 
      /* c'est une lambda-expr */
      {
	auxi1=car(expression);
	expression=cdr(expression);
	auxi=car(expression);
	if(TYPE==atde){adapteival((Int*)201,auxi,auxi1);
	expression=cdr(expression);}
	auxi=car(expression);
	expression=cdr(expression);
	push(a3);
	push(a2);
	while(islist(auxi)){
	  if(isatom(car(auxi))){
	    funion(car(auxi),atlocalvar,1);
	    a3=funion(car(auxi),a3,0);
	    adapteival(atlocalvar,car(auxi),la_fonction);
	    adapteival(0,la_fonction,car(auxi));}
	  auxi=cdr(auxi);
	}
	if(auxi!=nil){
	  funion(auxi,atlocalvar,1);
	  a3=funion(auxi,a3,0);
	  adapteival(atlocalvar,auxi,la_fonction);
	  adapteival(0,la_fonction,auxi);
	}
	a2=expression;
	testpro();
	pop_cast(a2,(Int *));
	pop_cast(a3,(Int *));
	continue;
      }
    if((auxi=car(expression))==atattach || auxi==atnreverse ||
       auxi== atnconc || auxi==atplaca || auxi== atplacd ||
       auxi== atplacb || auxi==atset   || auxi== atsetq  ||
       auxi== atincr  || auxi==atdecr  || auxi== atnewl  ||
       auxi== atnextl || TYPE==atsetq || TYPE==atnconc) 
      /* les effets de bord */
      {
	if(auxi==atsetq || auxi == atset || TYPE==atsetq) IND=1;
	expression=cdr(expression);
	auxi=car(expression); /*auxi = la variable ? */
	push(a2);
      resetq:		
	push(expression);
	push(IND);
	a2=ncons(auxi);     /* preparer pour la recursion */
	testpro();
	pop(IND);
	pop_cast(expression,(Int*));
	auxi=car(expression);
	if(isatom(auxi)){ 
	  auxi1=find(1,((struct atome *)la_fonction)->ival);
	  if(auxi1!=0){
	    adapteival((Int*)2,la_fonction,auxi);
	    adapteival(atincr,auxi,la_fonction);
	  }}
	if(IND==1){
	  expression=cdr(expression);
	  a2=car(expression);
	  a2=ncons(a2);
	  push(expression);
	  testpro();
	  pop_cast(expression,(Int*));
	  expression=cdr(expression);
	  if(expression!=nil){
	    IND=1;
	    auxi=car(expression);
	    goto resetq;
	  }
	}
	else {
	  a2=cdr(expression);
	  testpro();
	}
	pop_cast(a2,(Int *));
	continue;
      }
    if(auxi==atapply || TYPE==atapply)
      {
	expression=cdr(expression);
	push(a2);
	push(expression);
	auxi1=car(expression);
	auxi=car(auxi1);
	a2=ncons(auxi1);
	testpro();
	pop_cast(expression,(Int*));
	a2=cdr(expression);
	testpro();
	pop_cast(a2,(Int *));
	continue;
      }
    if(isatom(auxi) && auxi<=atuser) /* fonction std quelconque */
      {
	if(indprog!=0 && auxi==atgo) continue;
	push(a2);
	a2=cdr(expression);
	testpro();
	pop_cast(a2,(Int *));
	continue;
      }
    if(islist(auxi)){
      expression=cdr(expression);
      push(a2);
      push(expression);
      a2=ncons(auxi);
      testpro();
      pop_cast(a2,(Int *));
      testpro();
      pop_cast(a2,(Int *));
      continue;
    }
    if(!isnum(auxi) && !isstr(auxi)) {
      funion(auxi,atall_functions,1);
      adapteival((Int*)3,auxi,la_fonction);
      adapteival((Int*)4,la_fonction,auxi);
    }
    push(a2);
    a2=cdr(expression);
    testpro();
    pop_cast(a2,(Int *));
  }
}

Int *
find(indic,la_liste) /* trouve la liste qui commence par indic */
/* ramene soit 0, si inexistant, soit la  */
/* liste commenceant par cette sousliste  */
Int indic; 
Int *la_liste;
{
  Int *axi;
  signal(SIGINT,SIG_IGN);
  while(islist(la_liste)){
    axi=car(la_liste);
    if(car(axi) == (Int*)indic) {signal(SIGINT,(void(*)(int))onintr);return(la_liste);}
    la_liste=cdr(la_liste);
  }
  signal(SIGINT,(void(*)(int))onintr);
  return(0);
}


Int func(){/* prepare pour l'analyse */
  Int IND;
  Int *auxi3;
  Int *auxi2;
  Int *auxi1;
  Int *auxi;
  auxi1=((struct atome *)a1)->ival;auxi=find(201,auxi1);
  if(auxi!=0) return;
  la_fonction=a1;
  a3=atall_functions;	/* d'abord le nom de fonctions */
  auxi=(Int*)occurloc(a1,(Int*)*atall_functions);
  if(auxi!=0){
    clrocc(); /* si deja defini, faut enlever all occurences */
    auxi=((struct atome *)la_fonction)->ival;
    while(islist(auxi)){
      auxi1=car(auxi);
      if((a3=car(auxi1)) == (Int*)0 || a3==(Int*)4 || a3==(Int*)2 || a3==(Int*)1)
	{
	  *CDR(auxi1)=(Int)nil;
	}
      auxi=cdr(auxi);
    }
  }
  a3=atall_functions;
  funion(a1,a3,1);
  if(EXTREAD==0){
    a4=car(a2);		/* ensuite les variables locales */
    /* Modif de D.P. : les variables sont en premier lieu copiées dans auxi
       pour traiter le cas des (a . b) où b était traité de mainière étrange
       tant dans localvar "((b) a)" que dans ival (0 a . b)...
    */
    if(isatom(a4) && !isnil(a4)) a4=ncons(a4);
    auxi = nil; /* précédement auxi = a4; */

    while(islist(a4)){
      funion(car(a4),atlocalvar,1); /* le met ds localvar */
      if(auxi == nil) auxi = cons(0, auxi3 = cons(car(a4), nil));
      else { *CDR(auxi3) = (Int) cons(car(a4), nil); auxi3 = cdr(auxi3); }
      a4=cdr(a4);
    }
    if(isatom(a4) && a4!=nil) { 
      funion(a4,atlocalvar,1); /* précédemment funion(nconc(a4, ...) (D.P.) */
      if(auxi == nil) auxi = cons(0, cons(a4, nil));
      else *CDR(auxi3) = (Int) cons(a4, nil);
    }
    if(auxi != nil) { 
      auxi1=((struct atome *)a1)->ival;   /* les mettre sur ival de la ftn */
      auxi2=find(0,auxi1);
      if(auxi2!=(Int*)0) {*CAR(auxi2)=(Int)auxi;}
      else
	((struct atome *)a1)->ival=cons(auxi,auxi1);
    }
    a3=a4=cdr(auxi);		/* ensuite chaqu'une des variables */
    /*    if(isatom(a4) && !isnil(a4)) a4=ncons(a4); (DP) */
    while(islist(a4)){
      IND=0;
      auxi=car(a4);
      auxi=((struct atome *)auxi)->ival;
      if(isnil(auxi)){/* y'avait pas encore cette variable */
	auxi2=ncons(la_fonction);
	auxi1=cons(atlocalvar,auxi2);
	auxi=car(a4);
	((struct atome *)auxi)->ival=ncons(auxi1);
      }
      else{	/* elle existait deja */
	while(islist(auxi)){
	  auxi2=car(auxi); /* la variable */
	  if(car(auxi2) == atlocalvar){
	    auxi2=car(auxi);
	    auxi1= funion(la_fonction,cdr(auxi2),0);
	    auxi1= cons(atlocalvar,auxi1);
	    *CAR(auxi)=(Int)auxi1;
	    IND=1;
	    break;
	  }
	  else auxi=cdr(auxi);
	}
	if(isnil(auxi) && IND==0){
	  auxi2=ncons(la_fonction);
	  auxi1=cons(atlocalvar,auxi2);
	  auxi=car(a4);
	  ((struct atome *)auxi)->ival=cons(auxi1,((struct atome *)auxi)->ival);
	}
      }
      a4=cdr(a4);
    }
    /*    a3=car(a2); (déplacé plus haut en prenant la liste construite) (D.P) /* la liste des variables  */
    if(isatom(a3)) a3=ncons(a3);
    a2=cdr(a2); /* le progn de la fonction */
  } /* le EXTREAD */
  else a3=nil;
  indprog=0; /* pas d'indicateur de PROG a priori */
  testpro();  /* doit faire le boulot */
  a1=la_fonction;    /* et on retourne le nom de fonction */
}

/* D.P. 1996, Ajout de l'analyse des clauses prolog *
 * Analyse d'une clause lors de sa definition par assert
 * a1 = le nom de la clause
 * a3 = le corps de la definition
 */
#define isvar(x)	(isatom(x) && (((struct atome *)x)->tags & VARIABLE))
#define isdol(x)	(x == atdol)
#define isassert(x)	(isatom(x) && (((struct atome *)x)->tags & CLAUSE))
#define evalp(x)	(isatom(x) && (((struct atome *)x)->tags & EVALUABLE))

Int clause() 
{
  Int *condition, *tmp, *auxi, *auxi1, *auxi2, *la_clause, *la_def, n_push = 0; 
  la_clause = a1;
  la_def = a3;
  if((auxi = ((struct atome *) la_clause)->ival) != 0) {
    /* netoyage de l'ancienne ival */
    auxi1 = 0;
    while(islist(auxi)) {
      /* on enleve les ival 5 et 6 */
      if((auxi2=car(car(auxi)))==(Int*)5 || auxi2==(Int*)6) { 
	*CAR(auxi) = (Int)car(cdr(auxi));
	*CDR(auxi) = (Int)cdr(cdr(auxi));
      }
      else if(auxi2 == (Int*)3) { auxi1 = car(auxi); auxi = cdr(auxi); }
      else auxi = cdr(auxi);
    }
    if(auxi1)
      /* remplacement des anciens appel dans des clauses par des references 
	 a des clauses prolog et non plus a des fonctions utilisateur */
      for(auxi1 = car(auxi1); auxi1 && islist(auxi1); auxi1 = cdr(auxi1)) {
	tmp = car(auxi1);
	if(isassert(tmp)) {
	  /* on enleve de ival 3 et 4 */
	  a1=la_clause; clr2(((struct atome *)tmp)->ival);
	  a1=tmp;       clr2(((struct atome *)la_clause)->ival); 
	  /* on ajoute dans ival 5 et 6 */
	  adapteival((Int*)5,tmp,la_clause);
	  adapteival((Int*)6,la_clause,tmp);
	}
      }
  }
  if(!occurloc(la_clause, atall_clauses)) atall_clauses = cons(la_clause, (atall_clauses ? atall_clauses : nil));

  /* on sauve l'ancienne ival */
  push(((struct atome *)la_clause)->ival); ((struct atome *)la_clause)->ival = nil; auxi = 0;
  /* preparation pour l'utilisation de testpro dans le cas de lisp? */
  la_fonction = a1; 
  /* parcour de la definition */
  a2 = la_def;
  while(islist(a2)) { /* boucle sur les clauses */
    /* definition de plusieurs nom de clauses dans 1 assert : mauvais */
    if(car(car(car(a2))) != la_clause) return; 

    /* construction des variables locales a la clause */
    a3 = nil;
    push(a2);
    a2 = cdr(car(car(a2)));
    n_push = 0;
    while(islist(a2) || n_push) {
      if(a2 == nil && n_push) { pop_cast(a2,(Int *)); a2 = cdr(a2); --n_push; continue; }
      if(isvar(car(a2)) && !occurloc(car(a2),a3)) a3=cons(car(a2),a3);
      if(islist(car(a2))) {
	push(a2); ++n_push; a2 = car(a2); continue;
      }
      a2 = cdr(a2);
    }
    pop_cast(a2,(Int *));

    /* parcour des conditions... */
    push(a2);
    a2 = cdr(car(a2));
    while(islist(a2)) {
      condition = car(a2);
      tmp = car(condition);
      if(isassert(tmp)) { /* appel a un assert */
	/* regarde si pas deja reference */
	if(!auxi || !(auxi1 = find(6, auxi)) || !occurloc(tmp,cdr(car(auxi1)))) { 
	  adapteival((Int*)5, tmp, la_clause);	
	  adapteival((Int*)6, la_clause, tmp);
	  auxi = ((struct atome *) la_clause)->ival;
	}
      }      
      else { /* appel a l'evaluation de lisp... */
	if((!((struct atome *)tmp)->ftyp || ATOME_FTYP(tmp) >= EXPR) &&  /* utilisateur */
	   (!auxi || !(auxi1 = find(4,auxi)) || !occurloc(tmp,cdr(car(auxi1))))) {           /* non reference */
	  adapteival((Int*)3,tmp,la_clause);
	  adapteival((Int*)4,la_clause,tmp);
	  auxi = ((struct atome *) la_clause)->ival;
	}
	push(a2);
	a2 = cdr(condition);
	testpro(); /* parcour des arguments pour y trouver les appels */
	pop_cast(a2,(Int *));
      }
      a2 = cdr(a2);
    }
    pop_cast(a2,(Int *));

    /* clause suivante.... */
    a2 = cdr(a2);
  }
  if(!auxi) { pop_cast(((struct atome *)la_clause)->ival,(Int*)); }
  else {
    /* on merge l'ancienne ival avec la nouvelle */
    pop_cast(auxi1,(Int*));
    if(auxi1 == nil) ((struct atome *)la_clause)->ival = auxi;
    else {
      while(islist(auxi)) {
	auxi2 = car(car(auxi));
	if((tmp = find((Int)auxi2, auxi1)) != 0) {
	  auxi2 = cdr(car(auxi1));
	  while(islist(auxi2)) {
	    if(!occurloc(car(auxi2), tmp)) {
	      *CDR(tmp) =  (Int)cons(car(auxi2),cdr(tmp));
	    }
	    auxi2 = cdr(auxi2);
	  }
	}
	else { /* champ de l'ival non present precedemment */
	  auxi1 = cons(car(auxi),auxi1);
	}
	auxi = cdr(auxi);
      }
      ((struct atome *)la_clause)->ival = auxi1;
    }
  }
  a1 = la_clause;
  a3 = la_def;
}
