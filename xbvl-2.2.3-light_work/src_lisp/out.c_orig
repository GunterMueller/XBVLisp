
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	out.c		pour traiter les sorties
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 15 Fevrier 83
 *                      Ernesto Sendoya, interface X, janvier 1991
 */

#include <vlisp.h>
#include <xwin.h>

static FILE *filout;
static Int *AA,*BB;

/* static locales a out.c : pour que explode ne foute pas la merde */
/* en effet, l'appel a tryat tue les variables que ces statiques preservent */

static Int xna1,xna2,COmment;
static CHAR *xpchar;
static Int isfilout = 0;

Int fwst(str) 
CHAR *str;     
{
  if(isfilout==0) wst(str); 
  else {fflush(filout);fputs(str,filout);}
}

Int fwint(num)
Int num;
{
  if(isfilout==0) wint(num); 
  else {fflush(filout);fprintf(filout,"%d",num);}
}

Int terpri(){/* sort le buffer */
  bufou[oubpt++]='\n'; pc=terpr0;
}

Int prini(){/* prInt interne */
  COmment=0;
  push(a1); rec(probj,popa1);
}

Int probj(){/* edite l'objet lisp en a1 */
  /* si bit 1 de opti, pas d'espace avant impression */
  prcln=0;
  prnxt=(opti & 02); pc=prob1;
}

Int prob1(){
  pc=(prnxt) ? probc : (pushf(probc), prspc);
}

Int pannot(){/* impression annotations */
  a1=(Int*)prnxt=((Int)'{');rec(pvcch,pann1);
}

Int pann1(){/* suite impression annotations */
  pop_cast(a1,(Int *));
  if(++prcln > valnb((Int *)((struct atome *)atprlen)->cval))
    {pchar="...";pushf(prob4A);rec(prspc,prstr_1);return;}
  push(cdr(a1));
  if(!isnum(*a1) && !isstr(*a1) && ((UInt)(*a1) & (UInt)ANNOT))
    {a3=(Int*)(((UInt)(*a1)) & (UInt)~ANNOT);pushf(prob3_1A);
     push(cdr(a3));a1=car(a3);
     if(p>(Int *)hpil){pc=erpil;return;}
     pushf(pannot);
     rec(prob1,prspc);return;}
  a1=car(a1);
  if(p>(Int *)hpil){pc=erpil;return;}
  rec(prob1,prob3_1A);
}

Int prob3_1A(){
  prnxt=0;pop_cast(a1,(Int *));
  if(islist(a1)){push(a1);pc=pann1;return;}
  if(isnil(a1)){pc=prob4A;return;}
  pushf(prob4A);pushf(prato);pushf(a1);pushf(popa1);pushf(prspc);
  pushf(pvcch);push('.');pushf(popa1);pc=prspc;
}

Int prob4A(){
  a1=(Int*)((Int)'}');pc=pvcch;
}
     
Int probc(){
  Int ANot=0;
  if(isnotlist(a1)){pc=prato; return;}
  if(*atall==(Int)t && !isnum((*a1)) && !isstr((*a1)) &&
     ((UInt)(*a1) & (UInt)ANNOT))
    {a3=(Int*)(((UInt)(*a1)) & (UInt)~ANNOT);push(cdr(a3));
     a3=car(a3);
     ANot=1;}
  else if(!isnum(*(a1)) && !isstr(*a1) &&
	  ((UInt)(*a1) & (UInt)ANNOT))
    {a3=(Int*)(((UInt)(*a1)) & (UInt)~ANNOT);a3=car(a3);}
  else  a3=car(a1);
  if(isatom(a3) && a3==quote && ANot==0)
    {pushf(probc33);push(a1);pushf(popa1);
     a1=(Int*)prnxt=((Int)'\'');pc=pvcch;return;}
  if(!TRACE && isatom(a3) && (ATOME_FTYP(a3) == MACOUT))
    {
      if(ANot!=0)pushf(pannot);
      push(((struct atome *)a3)->fval); a1=cdr(a1); pc=apexp; return;
    }
  if(ANot!=0) {pop_cast(a3,(Int *));
	       pushf(prob3);push(cdr(a1));pushf(popa1);push(a3);
	       pushf(pannot);pushf(prspc);pushf(prob1);
	       push(car(a1)); pushf(popa1);a1=(Int*)prnxt=((Int)'('); 
	       pc=pvcch;}
	else {pushf(prob3); push(a1); pushf(popa1);
	      a1=(Int*)prnxt=((Int)'('); pc=pvcch;}
}
     
     
Int *cherchcomm(x) 
     Int *x;
{
  BB=AA=nil;cherchComm(x);
}

Int *cherchComm(x) 
     Int *x;
{
  if(isnil(x)) return(nil);
  else if (*x == (Int)atpcomm)
    {a4= (Int*)*x;a5= (Int*)*(x+1);A1= (Int*)*a5; a5= (Int*)*(a5+1);
     AA=cons(A1,AA);
     cherchComm(a5);return;}
  else if (*x == (Int)atcomm)
    {a4= (Int*)*x;a5= (Int*)*(x+1);A1= (Int*)*a5; a5=(Int*) *(a5+1);
     BB=cons(A1,BB);
     cherchComm(a5);return;}
  else {x= (Int*)*(x+1);x= (Int*)*(x+1);cherchComm(x);}
}

Int pro32(){
  pop_cast(a1,(Int *));
  if(isnil(a1)) {COmment=0;derec;}
  else{push((*(a1+1)));COmment=1;
       a1=car(a1);pushf(pro32);rec(prspc,prob1);}
}

Int pro31A(){pop_cast(a1,(Int *));pushf(pro32);rec(prspc,prob1);}

Int pro31(){
  cherchcomm((Int *)*(a1+1));
  if(isnotnil(AA)){push(BB);push(car(a1));a1=car(AA);AA=cdr(AA);
		   pushf(pro31A);push(AA);
		   COmment=1;rec(prob1,pro32);
		   return;}
  push(BB);
  a1=car(a1);rec(prob1,pro32); 
}	

Int probc33(){/* on a imprime les comentaires lies au quote */
  if(!isnum(*(a1)) && !isstr(*a1)
     && (((UInt)*(a1)) & (UInt)ANNOT))
    {a5=(Int*)(((UInt)*(a1)) & (UInt)~ANNOT);
     cherchcomm( (Int * ) *(a5+1));push(BB);pushf(pro32);
     a1=car(cdr(a1));pc=probc;return;}
  a1=cdr(a1);
  if(!isnum(*(a1)) && !isstr(*a1) 
		&& (((UInt)*(a1)) & (UInt)ANNOT))
    {a5=(Int*)(((UInt)*(a1)) & (UInt)~ANNOT);
     cherchcomm( (Int *) *(a5+1));push(BB);pushf(pro32);}
  a1=car(a1);pc=probc;return;
}
     
Int prob3(){
  if(++prcln > valnb((Int *)((struct atome *)atprlen)->cval))
    {
      pchar="..."; pushf(prob4); rec(prspc,prstr_1); return;
    }
  push(cdr(a1)); 
  if(*atall==(Int)t && !isstr(*a1) &&
     (!isnum(*(a1)) && (((UInt)*a1) & (UInt)ANNOT)))
    {a1=(Int*)(((UInt)*a1) & (UInt)~ANNOT);
     pushf(prob3_1);
     push(cdr(a1));a1=car(a1);pushf(pannot);
     rec(prob1,prspc);return;}
  else if(!isnum(*(a1)) && !isstr(*a1) 
	  && (((UInt)*a1) & (UInt)ANNOT))
    {a1=(Int*)(((UInt)*a1) & (UInt)~ANNOT);rec(pro31,prob3_1);}
  else  {a1=car(a1);
	 if(p > (Int *)hpil){pc=erpil; return;}
	 rec(prob1,prob3_1);}
}

Int prob3_1(){
  prnxt=0; pop_cast(a1,(Int *));
  if(islist(a1)){pc=prob3; return;}
  if(isnil(a1)){pc=prob4; return;}
  pushf(prob4); pushf(prato); push(a1); pushf(popa1); pushf(prspc);
  pushf(pvcch); push('.'); pushf(popa1); pc=prspc;
}

Int prob4(){
  a1=(Int*)((Int)')'); pc=pvcch;
}

Int prch(){/* edite le caractere dans a1 */
  if(isvlexpl)
    {
      xna1=na1; xna2=na2; xpchar=pchar;
      /* "creer" 1 atome mono-caractere */
      /* on roule tryat, meme hack crado que dans rdmac --- provisoire */
      /* foireux: cree 1 atome-8-mots/caractere, trouver mieux!!! */
      newppn();
      int_p1=(Int *)oppn; *int_p1=lpn=2;
      aux1=(Int)a1; *ppn++ =aux1; *ppn++=0;  /* que c'est merdique! */
      rec(tryat,prch_1); return;
    }
  --lcou; aux1=(Int)a1;  
  /* c'est mieux controler un debordement */
  oubpt = (oubpt >= (MAX_COLS - 2)) ? (MAX_COLS - 2) : oubpt; bufou[oubpt++]=aux1; 
  derec;
}

Int prch_1(){/* suite de prch dans le cas explode */
  a1=ncons(a1); 
  *CDR(vlexpl)=(Int)a1;
  vlexpl=a1;
  na1=xna1; na2=xna2; pchar=xpchar;
  derec;
}

Int prspc(){/* sort un espace ou un newline */
  if(lcou <= 0){pc=terpri; return;}
  push(a1); a1=(Int*)((Int)' '); rec(pvcch,popa1);
}

Int prato_12(){
  pop_cast(a1,(Int *));	na2=(*(((struct atome *)a1)->pn)); na2--;
  pchar=(CHAR *)((((struct atome *)a1)->pn)+(2/2));
  pc=prato_2;
  return;
}

Int prato_11(){pushf(prato_12);a1=(Int*)((Int)'|');pc=pvcch;return;}

Int prato(){/* edite l'atome en a1 */
  soubpt=oubpt; savp=p; sava1=(Int)a1;
  
  /* E.S : il faut dans tous les cas controler 
     la longeur de l'objet a imprimer */
#ifndef NO_VFLO
  if(isflo(a1)){pc=prflo; return;}
#endif
  if(isnum(a1)){pc=prnum; return;}
  if(isstr(a1)){pc=prstr; return;}

  if(*atall==(Int)t && ((struct atome *)a1)->pakage!=nil){
    a5=((struct atome *)a1)->pakage;
    na2=(*(((struct atome *)a5)->pn));na2--;
    pchar=STR_FROM_STRLSP(((struct atome *)a5)->pn);
    push(a1);pushf(prato_11);pushf(prato_2);
    if((na2+(*(((struct atome *)a1)->pn))) < lcou) popf; else pc=terpri;
    return;}
  else {
    na2=(*(((struct atome *)a1)->pn)); na2--;
    pchar=(CHAR *)((((struct atome *)a1)->pn)+(2/2));
    pushf(prato_2);
    if(na2 < lcou)popf; else pc=terpri;}
}

Int prato_2(){
  if(na2--){a1=(Int*)((Int)(*pchar++)); rec(pvch,prato_2); return;}
  derec;
}

Int pvch(){/* edite le caractere dans a1: teste et trappe
	  le depassement longueur de ligne */
  if(lcou > 0)pc=prch;
  else
    {
      oubpt=soubpt; p=savp; a1=(Int*)sava1;
      rec(terpri,prato);
    }
}

Int pvcch(){/* edite un caractere separateur en a1 */
  pc=(lcou>1) ? prch : (pushf(prch),terpri);
}

Int prnum(){/* edite le nb dans a1 */
  Int obs; obs= valnb((Int *)((struct atome *)atobase)->cval);
  sign=((na1=valnb(a1)) < 0) ? (na1=(-na1),1) : 0;
  do{na2=na1%obs; na1/=obs; pushf(pvch); 
     if(na2>9) 
       {if(na2>35) push(na2+61);else push(na2+55);}
     else push(na2+'0'); pushf(popa1);}
  while(na1);
  if(sign){a1=(Int*)((Int)'-'); pc=pvch;} else popf;
}

Int prstr(){/* impression de chaine et de commentaires */
  pchar=(CHAR *) (a1+(2/2));
  /* si bit 0 de opti : impression des '"' des chaines */
  if(COmment==1)
    {a1=(Int*)((Int)';'),pushf(prch),push(a1),pushf(popa1),pushf(prstr_1),pc=prch;}
  else
    pc=((opti & 01))
      ? (a1=(Int*)((Int)'"'),pushf(prch),push(a1),pushf(popa1),pushf(prstr_1),prch)
	: prstr_1;
}


Int prstr_1(){
  if(a1=(Int*)((Int)(*pchar++)))
    {
      pushf(prstr_1);
      if(isvlexpl && a1==(Int *) '\n') derec;
      pc=((a1==(Int *)'\n') ? terpri : ((lcou==0) ? --pchar, terpri : prch));
      return;
    }
  derec;
}

Int tyo(){/* (tyo n)  subr 1 */
  /* inserre le caractere sur l'interface courrante */
  aux1=(Int)a1;
  xinsch(NULL, (CHAR)aux1, NULL);
  derec;
}

Int tyoesc(){/* (tyoesc n)  subr 1 */  /* inutil dans la nouvelle interface */ 
  a1 = nil;
  derec;
}

Int nilall(){pop(*atall);pop(opti);derec;}

Int allprint(){/* nsubr - pour imprimer les annotations */
  push(opti);opti |= 01;
  push((*atall));*atall=(Int)t;pushf(nilall);rec(prin1,terpri);
}

Int extprint(){/* nsubr - pour imprimer les annotations */
  push(opti);
  push((*atall));*atall=(Int)t;pushf(nilall);rec(prin1,terpri);
}

Int allprin1(){/* nsubr - pour imprimer les annotations */
  push(opti);opti |= 01;
  push((*atall));*atall=(Int)t;rec(prin1,nilall);
}

Int extprin1(){/* nsubr - pour imprimer les annotations */
  push(opti);
  push((*atall));*atall=(Int)t;rec(prin1,nilall);
}


Int print(){/* (prInt . l)  nsubr */
  rec(prin1,terpri);
}

Int explode(){/* (explode s)  subr 1 */
  lcou=MAX_COLS-1;
  ++isvlexpl; vlexpl=ncons(nil); push(vlexpl);  /* hook */
  prnxt=1;   /* pour ne pas avoir l'espace avant */
  rec(prob1,explo_1);
}

Int explo_1(){
  pop_cast(a1,(Int *)); a1=cdr(a1); isvlexpl=0; derec;
}

Int terpr00(){/* sort le buffer sans aller a la ligne */
  bufou[oubpt++]='\0'; 
  if(isfilout==0) wst(bufou);
  else {
    fflush(filout);
    fwrite(bufou,strlen(bufou),1,filout);
  }
  iniott(); derec;
}

Int princ(){/* (princ . l)  nsubr */
  rec(prin1,terpr00);
}

Int terpr0(){/* sort le buffer sans aller a la ligne */
  bufou[oubpt++]='\0'; 
  push(a1);a1=ateol; a2=nil; rec(apply,popa1);
}
Int eol(){/* (eol) subr 0 */
  if(isfilout==0) wst(bufou); 
  else
    {
      fflush(filout);
      fputs(bufou,filout);
    }
  iniott(); derec;
}

Int prin_1()
{
  if(isnotnil(cdr(a1)))
    {
      pushf(prin_1); push(cdr(a1)); pushf(popa1);
    }
  a1=car(a1); pc=prini;
}

Int prin1(){/* (prin1 . l)  nsubr */
  if(isfilout) lcou = *atrmarg;
  pc = prin_1;
}

Int princh(){/* (princh e n)  subr 2 */
  /* princh n'imprime PAS les '"' des chaines */
  push(opti); opti &= ~01;
  push((isnil(a2) ? 1 : valnb(a2)));
  pc=prich_1;
}

Int prich_1(){
  pop(na1); if(--na1 < 0){pop(opti); derec;}
  push(na1); pushf(prich_1); push(a1);
  rec(prato,popa1);
}

Int outpos(){/* (outpos n)  subr 1 */
  if(isnil(a1))a1=(Int*)oubpt;
  else{oubpt=(Int)a1; lcou=(*atrmarg)-oubpt;}
  derec;
}


Int output(){/* (output NOMFILE-ou-())  subr 1 */
  CHAR *pch2;
  if(isnil(a1))
    {
      *atrmarg = getXbvlColonnes(Xbvlisp_courr) - 2;
      lcou = *atrmarg + 2 - CARS_MARGE_DROITE;
      if(!isfilout)goto fini;
      fflush(filout);
      fclose(filout); isfilout=0; goto fini;
    }
  *atrmarg = MAX_COLS - 1;
  a4=a1; if(isatom(a4))a4=((struct atome *)a4)->pn; pch2=(CHAR *)(a4+(2/2));
  filout=fopen(pch2,"a");
  if(filout==NULL){a1=nil; goto fini;}
  isfilout=1;
 fini:
  derec;
}



