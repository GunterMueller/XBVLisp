
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	in.c		pour traiter les entrees
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 23 Fevrier 83
 *			Harald Wertz Janvier 84
 *			Ernesto Sendoya janvier 94 ; interface X-SYSTEME et Hash-Coding
 */

#include <vlisp.h>

Int rsharp(),rshar_1();Int beurk;
Int objlis();

Int COMMENT; /* indicateur de commentaire ou autre annotation */ 
Int pack,DEJA; /* indicateur pour package */
static CHAR pak[300]; /* pour la barre | */

Int inchb()
{
  /* car suivant dans a1 */
  if(dski)
    while((a1 = (Int*)((Int)*inbpt++)) == 0)
      {
	aux1=(Int)fgets(bufin,128,fdes);
	if(aux1==(Int)NULL){a2=ateof; a1=nil; pc=evalfu; return;}
	
	/* horrible hack pour faire imprimer la ligne entree */
	inbpt=bufin;
	if((optop&01) != 0) wst(buftop);
      }
  else
    while((a1 = (Int*)((Int)*inbpt++)) == 0)
      {
	if(DML!=1 && MARKED==0){
	  CHAR *prompt, nprompt[64];
	
	  aux1=(Int)((struct atome *)atlmarg)->cval;
	  while(aux1--) wst(" "); 
	  int_p1= (Int*)*atprompt;
	  if(isnum(int_p1)){ 
	    /* pour pouvoir donner un nombre au prompt , Ernesto*/
	    (void)numToString(nprompt, int_p1);
	    prompt = nprompt;
	  }
	  else {
	    if(isatom(int_p1))
	      int_p1=((struct atome *)int_p1)->pn;
	    prompt = (CHAR *)(int_p1+(2/2));
	  }
      	  wst(prompt);
	  if(*atprompt != (Int)atlst) wst(" ");
	  if(!external_ptr){ 
	    aux1=(rdprd>7) ? 7 : rdprd;
	    /* si evaluation normal (non selection)
	       indentation des parentheses          */
	    while(aux1--)wst("   ");
	  }
	}
	rst();	/* lire une ligne de clavier dans bufin */
	inbpt=bufin;
	if(DML==2){derec;}
      }
  derec;
}

Int getch1()
{/* car suiv dans a1, type dans tlu */
  pushf(getch_1);
  if((a1=(Int*)ringur) != 0){ringur=0; popf;}
  /* ici fut longtemps un horrible bug! */
  else
    if(isimpl)
      {
	a1=impl;
	if(isnotlist(a1)){
	  isimpl=0; a1=(Int*)((Int)' '); derec;
	} /* fake a separator */
	impl=cdr(a1); a1=car(a1);
	if(isnum(a1)){a1=(Int*)(((Int)a1)+'0'); derec;}
	if(isatom(a1))a1=((struct atome *)a1)->pn;
	/* cas atome ET string */
	pchar=(CHAR*)STR_FROM_STRLSP(a1);
	a1= (Int*)((Int)*pchar); popf;
      }
    else pc=inchb;
}

Int getch_1()
{
  if(DML==2) {pc=rdmac10;}
  else if ((tlu=tabch[(Int)a1]) == 0)pc=getch1; 
  else {if(tlu==MACL) tlu=NOR;popf;}
}

Int getcv()
{/* traitement des commentaires */
  pc=getch1;
}	

Int getcv_1(){
  if(tlu != BCOM)derec;
  rec(getch1,getcv_2);
}

Int getcv_2(){
  if(a1==(Int*)10 || tlu==BCOM)pc=getcv;
  else rec(getch1,getcv_2);
}

Int comm_ig(){pushf(rd1_1);rec(getch1,getcv_2);}

/* pour la lecture des commentaires */

Int comm_out() 
{
  static Int *A2;
  A2=a1;COMMENT=1;pop_cast(a5,(Int *));
  if(!isnum(*(a5)) && ((UInt)*a5 & (UInt)ANNOT))
    {a4=(Int*)((UInt)*a5 & (UInt)~ANNOT);
     *CDR(a4)=(Int)cons(atcomm,cons(A2,cdr(a4)));
     push(a5);pc=read3;}
  else { 
    a4=cons(car(a5),cons(atcomm,ncons(A2)));
    *CAR(a5)=(((UInt)a4) | (UInt)ANNOT);
    push(a5);
    a1=a5;pc=read3;}
}

Int comm_in() 
{
  COMMENT=2;rec(rdst,comm_out);
}


Int rd1()
{/* a1 <- adr lisp obj lu, tlu <- son type */
  rec(getcv,rd1_1);
}


Int rd1_1()
{
  a5= (Int*)((Int)*inbpt);a5=(Int*)((Int)tabch[(Int)a5]);
  switch(tlu)
    {
    case BCOM:	if(COMMENT==0){pc=comm_ig;return;} else derec;
    case LANNOT:  
    case LCRO:
    case LPAR:	++rdprd; derec;
    case RANNOT:
    case RCRO:
    case RPAR:	if(--rdprd < 0)rdprd=0; 
    case DOT: 	derec;
    case SEP:	pc=rd1; return;		/* separateur: on saute */
    case MAC:	if(COMMENT==0)++COMMENT;pc=rdmac; return;
    case STR:	pc=rdst; return;
    case SHAR:	rec(getch1,rsharp); return;
    case MACL:
    case HINIB: tlu=NOR; a1=(Int *)((Int)*(inbpt++)); a5=(Int *)NOR;
    case NOR:	pc=rd2;
    }
}

Int rsharp()
{/* traitement des diezes */
  if(a1==(Int*)((Int)'/'))rec(getch1,objlis)
  else
    if(a1==(Int*)((Int)'^'))rec(getch1,rshar_1)
    else
      {inbpt--; a1=(Int*)((Int)'#'); pc=(Int(*)())rd2;}	/* caractere dieze normal */
}

Int rshar_1(){/* cas de #^X */
  Int ch;
  ch=(Int)a1;
  if(ch>='a' && ch<='z')ch=(ch-'a')-'A';
  ch-= 0100; a1=(Int*)ch; pc=objlis;
}


Int rdmac(){/* traitement des macro-caracteres */
#if 0
/* pour rouler crato . --- crado et provisoire */
  aux1=ppn; aligner(aux1); ppn=aux1;	/* on cale */
  oppn=ppn;
  aux1=a1; *(oppn+sizeof(Int))=aux1; *(oppn+sizeof(Int)+1)=0;
  int_p1=oppn; *int_p1=2;

#endif
/* E.S. : introduction de la fonction generale 
     de creation de chaines */
  CHAR mac_in[2];
  mac_in[0] = (CHAR) ((Int)a1);
  mac_in[1] = (CHAR) 0;
  newstr(mac_in);

  ((struct atome *)atmc)->pn=(Int*)oppn;
  ((struct atome *)atmc)->fval=tabfval[(Int)a1].fval;
  ((struct atome *)atmc)->ftyp=tabfval[(Int)a1].ftyp;
  
  a1=atmc;
  a2=nil;
  pushf(objlis); pc=apply;
}


Int rdmac11(){
  DML=1;pc=getcv;
}
     
     
Int rdmac10(){/* traitement des macro-caracteres DML */
#if 0
  /* pour rouler crato . --- crado et provisoire */
  aux1=ppn; aligner(aux1); ppn=aux1;	
  oppn=ppn;
  aux1=a1; *(oppn+sizeof(Int))=aux1; *(oppn+sizeof(Int)+1)=0;
  int_p1=oppn; *int_p1=2;
#endif

  /* E.S. : introduction de la fonction generale 
     de creation de chaines */
  CHAR mac_in[2];
  mac_in[0] = (CHAR) ((Int)a1);
  mac_in[1] = (CHAR) 0;
  newstr(mac_in);

  ((struct atome *)atmc)->pn=(Int*)oppn;
  ((struct atome *)atmc)->fval=tabfval[(Int)a1].fval;
  ((struct atome *)atmc)->ftyp=tabfval[(Int)a1].ftyp;
  
  a1=atmc;
  a2=nil; 
  DML=3;
  pushf(rdmac11); pc=apply; 
}

Int rdst(){/* traitement de strings */
  newppn();
  rec(getch1,rdst_1);
}

Int rdst_1(){
  aux1=(Int)a1; 
  *ppn++ = aux1;
  if(aux1=='\\' && *atANTISL==1) *ppn++ =aux1;
  if(isnotnil((Int*)*atANTISL)){
    if(aux1=='\\') beurk++;
    else if(tlu!=STR) beurk=0;}
  if(ppn>hstr)gcstrsv();
  /* est-ce l'anti-slash ? */
  if(*(ppn-1) == '\\' && isnil((Int*)*atANTISL))
    {--ppn; rec(getch1,rdst_2); return;}
  if(COMMENT==2 && (a1 != (Int*)10 && tlu != BCOM)){rec(getch1,rdst_1);return;}
  if(tlu != STR && COMMENT!=2) {rec(getch1,rdst_1);return;}
  if(tlu == STR && isnotnil((Int*)*atANTISL) && COMMENT!=2 && (beurk%2)==1)
    {beurk=0; rec(getch1,rdst_1); return;}
  /* ici, *(ppn-1) == '"' */
  beurk=0;
  *(ppn-1)=0; a1=(Int*)oppn; lpn=ppn-oppn-sizeof(Int);
  *a1=lpn; pc=objlis;
}
Int rdst_2(){
  switch((Int)a1)
    {
      /* d'abord: cas '\' en fin de ligne */
    case '\n':  rec(getch1,rdst_1); return;
    case 'n' :  a1=(Int*)((Int)'\n');  break;
    case 't' :  a1=(Int*)((Int)'\t');  break;
    case 'r' :  a1=(Int*)((Int)'\r');  break;
    case 'b' :  a1=(Int*)((Int)'\b');  break;
    case '0' :  na1=0; rec(getch1,rdst_3); return;
      default  :  break;
    }
  aux1=(Int)a1; *ppn++ = (Int)aux1; rec(getch1,rdst_1);
}

Int rdst_3(){
  na2=(Int)a1;
  if('0' <= na2 && na2 <= '7'){na1*=8; na1+=na2-'0'; pushf(rdst_3);}
  else{*ppn++ = na1; pc=rdst_1; return;}
  pc=getch1;
}

Int objlis(){
  tlu=AT; pack=DEJA=0;derec;
}
     
Int rd2(){/* traitement des pnames */
  pack=1;
  newppn();
  pc=rd4;
}

Int rd4(){
  aux1=(Int)a1; *ppn++ = aux1;
  if(ppn>hstr)gcstrsv();
  rec(getcv,rd4_1);
}

Int restaure(){
  pop_cast(a5,(Int *));*atpackage=(Int)a5;derec;
}

Int restaure1(){
  Int i; *atpackage=(Int)a1;newppn();i=0;
  ppn=oppn+sizeof(Int);
  while((*ppn++ = pak[i++]) != 0);
  pushf(restaure);
  lpn=ppn-oppn-sizeof(Int);
  int_p1=(Int*)oppn; *int_p1=lpn;
  rec(tryat,objlis);
}

Int rd4_1(){
  if(tlu == HINIB) {tlu=NOR; a1=(Int *) ((Int)*(inbpt++)); pc=rd4; }
  else if(tlu == NOR  || tlu == DOT || tlu == MACL) pc=rd4;
  else {CHAR *pc2,*pc3;CHAR pak1[300];
	ringur=(Int)a1; *ppn++=0;
	pc2=oppn+sizeof(Int);pc3=pak1;
	while (*pc2) if((*pc3++ = *pc2++) == '|') break;
	if(*pc2) {
	  pc3--;*pc3=0; /* pak1 contient le nom du package */
	  /* pc2 pointe vers le debut du pname */
	  if(pak1[0]==0) 
	    {push(*atpackage);*atpackage=(Int)nil;pushf(restaure);
	     ppn=oppn+sizeof(Int);
	     while((*ppn++ = *pc2++) != 0);}
	  else {Int i;i=0;pc3=pak;while((*pc3++ = *pc2++) != 0);
		/* pak = le pname */
		ppn=oppn+sizeof(Int);
		while((*ppn++ = pak1[i++]) != 0);
		/* oppn+Int = le nom du package */
		push(*atpackage);*atpackage=(Int)nil;pushf(restaure1);}
	  
	}
	lpn=ppn-oppn-sizeof(Int);
	int_p1=(Int*)oppn; *int_p1=lpn;
	rec(tryat,objlis);
      }
}

Int tryat(){/* apres reception de pname: voyons si nb */
  Int maxchar;
  Int base;
  
  base= valnb(((struct atome *)atibase)->cval);
  if(base<=10) maxchar=base+47; else maxchar=base+54;
  if(base>35) maxchar=maxchar+6;
  
  pchar=oppn+sizeof(Int);
#ifndef NO_VFLO
  if(veriflo()){a1=craflo(atof(pchar)); ppn=oppn; derec;}
#endif
  sign=1;na2= *pchar++;
  if(na2== '-')
    {
      if(lpn==2){pc=crato; return;}
      na2=(*pchar++);
      sign=(-1);
    }
  
  if(maxchar>57 && na2>'9') {pc=crato;return;}
  else na1=0;
  do
    {
      if(na2<'0') {pc=crato;return;}
      if(na2>maxchar){pc=crato; return;}
      if(na2>'9' && na2<'A') {pc=crato;return;}
      if(na2>'Z' && na2<'a'){pc=crato;return;}
      na1 *= base; 
      na1+=na2-'0';
      if(na2>'9') na1-=7;
      if(na2>'Z') na1-=6;
      na2=(*pchar++);
    }
  while(na2);
  na1*=sign;
  ppn=oppn;		/* ne pas avancer le pointeur de string! */
  a1=cranb(na1);
  derec;
}

#if 0
/* def dans vlisp.h par #define pour optimisation */
Int *cherchat(adstr) char *adstr; { return (Int *) lookfor_atome((char *)(adstr+sizeof(Int))); }
#endif


Int *creatom_hash(adstr, hash_ind) CHAR *adstr; Int hash_ind; {
  Int *x;
  
  if(freeat==0)gci();
  x=freeat;
  ((struct atome *)x)->cval=undef;
  ((struct atome *)x)->pval=nil;
  ((struct atome *)x)->ival=nil;
  ((struct atome *)x)->pakage= (Int*)*atpackage;
  ((struct atome *)x)->fval=((struct atome *)x)->ofval=0; 
  ((struct atome *)x)->ftyp=((struct atome *)x)->oftyp=0;	
  ((struct atome *)x)->obj=nil;
  ((struct atome *)x)->pn=(Int*)adstr;
  if(*(adstr+sizeof(Int)) == '$')((struct atome *)x)->tags= 01;	/* variable prolog */
  freeat=(Int*)((struct atome *)freeat)->nexa;
  ((struct atome *)x)->nexa=(ATOMEPTR)derat; derat=x;

  /* ici on sauve l'atome dans la hash-table */
  (void)insert_atome((struct atome *) x, (hash_ind < 0 ? get_hashind((struct atome *)x) : hash_ind));

  return(x);
}

#if 0
/* pour optimisation: def par #define dans vlisp.h */
Int *creatom(adstr) CHAR *adstr; { creatom_hash(adstr,-1); }
#endif

Int crato(){/* pname literal: existe ou pas ? */

  if(a1 = (Int *) lookfor_atome_hash((CHAR *)(oppn+sizeof(Int)), &DEJA)) {DEJA=1;ppn=oppn;}
  else {
    a1=creatom_hash(oppn, DEJA);
    DEJA = 0;
  }

/*  if(a1=cherchat(oppn)){DEJA=1;ppn=oppn;}
  else a1=creatom(oppn);
*/
  derec;
}

Int readind()
{
  Int *auxi,*TYPE;
  if(islist(a1))
    if(isatom(car(a1)) ){
      auxi=car(a1);auxi=find(200,((struct atome *)auxi)->ival);
      if(auxi!=0) {
	auxi=car(auxi);auxi=cdr(auxi);TYPE=car(auxi);
	if(TYPE==atde) {
	  push(a1);a2=ncons(a1);a1=cdr(a1);
	  a1=car(a1);EXTREAD=1;func();
	  EXTREAD=0;pop_cast(a1,(Int *));
	}
      }
    }
  derec;
}

Int readu()
{/* read externe */
   COMMENT=pack=EXTREAD=beurk=0;
  if(*atcareful==(Int)t) pushf(readind);
  rec(rd1,readu_1);
}

Int allrend(){
  pop(aux1);tabch[(Int)'}']=aux1;pop(aux1);tabch[(Int)'{']=aux1;
  derec;
}

Int allread(){/* read externe avec annotations */
  push(tabch[(Int)'{']);push(tabch[(Int)'}']);
  tabch[(Int)'{']=LANNOT;tabch[(Int)'}']=RANNOT;
  COMMENT=pack=EXTREAD=beurk=0;
  if(*atcareful==(Int)t) pushf(readind);
  pushf(allrend);rec(rd1,readu_1);
}

Int readu_1(){
  if(tlu==RPAR) pc=readu; else {COMMENT=1;pc=read0;}
}

Int readi(){/* read interne */
  pack=0;
  rec(rd1,read0);
}

Int read0(){
  switch(tlu)
    {
    case BCOM:	derec;
    case RANNOT: case RPAR: case RCRO: case DOT: pc=erlec; return;
    case LANNOT: 	rec(rd1,read3_1); return;
    case LPAR:	rec(rd1,read2_1); return;
    case LCRO:	rec(rd1,read1_1); return;
    case AT:	popf;
    }
}

Int read1_1(){/* continuation de [ lu */
  if(tlu==RCRO){a1=nil; derec;}
  else if (tlu==BCOM){a3=ncons(atlst);push(a3);push(a3);
		      pc=comm_in;}
  else{a3=ncons(atlst); push(a3); push(a3); pc=read4;}
}

Int read214(){
  COMMENT=1;pop_cast(A1,(Int*));push(cons(atpcomm,cons(a1,A1)));
  rec(readi,read215);
}

Int read215()
{
  if(tlu!=BCOM){
    pop_cast(A1,(Int*));a1=cons(a1,A1);
    a1=ncons((Int*)((UInt)a1 | (UInt)ANNOT));
    push(a1);push(a1);pc=read3;}
  else {
    COMMENT=2;rec(rdst,read214);
  }
}

Int read213(){
  pop_cast(A1,(Int*));
  /* provisoire */
  if(tlu!=BCOM)
    {a1=(cons(a1,(cons(atpcomm,ncons(A1)))));
     a1=ncons((Int*)((UInt)a1 | (UInt)ANNOT));
     push(a1);push(a1);pc=read3;}
  else {push(cons(atpcomm,ncons(A1)));COMMENT=2;rec(rdst,read214);}
}

Int read212(){COMMENT=1;push(a1);rec(readi,read213);}

Int read2_1(){/* continuation de ( lue */
  if(tlu==RPAR){a1=nil; derec;}
  else if (tlu==BCOM){COMMENT=2;rec(rdst,read212);}
  else if (tlu==LANNOT){pc=erlec;return;}
  else rec(read0,read2_2); 
}

Int read2_2(){
  a1=ncons(a1); push(a1); push(a1); pc=read3; 
}

Int read3_1(){/* continuation '{' lue */
  if(tlu==RANNOT) {rec(rd1,read4);}
  else if(tlu==LANNOT){pc=erlec;return;}
  else rec(read0,read3_2);
}

Int read3_2(){/* suite '{' lue */
  pop_cast(a4,(Int *));a1=ncons(a1);
  a5=cons(car(a4),a1);
  *CAR(a4)=((UInt)a5 | (UInt)ANNOT);
  push(a4);push(a1);
  pc=read3;
}

Int read3(){/* lecture d'un elt de liste */
  rec(rd1,read4);
}

Int read4(){/* traitement d'un elt de liste */
  switch(tlu)
    {
    case BCOM:	if(COMMENT!=1) 
      {
	wst("read4,BCOM\n");
	pc=erlec;
      } 
    else {
      pc=comm_in;
    } return; 
    case LPAR:	pushf(read52); rec(rd1,read2_1); return;
    case LANNOT:  rec(rd1,read3_1);return;
    case RANNOT:  pc=read43_A;return;
    case RPAR:	--p; pc=read43; return;
    case LCRO:	pushf(read52); rec(rd1,read1_1); return;
    case RCRO:	--p; pop_cast(a1,(Int *));
      if(car(a1) != atlst){wst("read4,RCRO\n");pc=erlec;}
      else{*CAR(a1)=(Int)atlist;derec;}
      return;
    case DOT:	rec(readi,red48_1); return;
    case AT:	pc=read52;
    }
}

Int read43_A(){
  pop_cast(a5,(Int *));pop_cast(a5,(Int *));push(a5);
  if(!isnum(*a5) && !isstr(*a5) && 
     ((UInt)(*a5) & (UInt)ANNOT)) {rec(rd1,read4);}
  else {pc=erlec;return;}
}

Int read43(){
  pop_cast(a1,(Int *)); if(car(a1)==atlst){wst("read43\n");pc=erlec;} 
  else popf;
}

Int red48_1(){/* liste pointee */
  push(a1); 
  rec(rd1,red48_2);
}

Int red48_2(){
  if(tlu == RPAR)
    {
      pop_cast(a2,(Int *)); pop_cast(a1,(Int *)); 
      *CDR(a1)=(Int)a2;
    pc=read43; return;
    }
  if(tlu != RCRO){
    pc=erlec; return;
  }
  pop_cast(a3,(Int *)); pop_cast(a2,(Int *)); 
  *CDR(a2)=(Int)ncons(a3);	
  pop_cast(a1,(Int *));
  if(car(a1) != atlst){pc=erlec; return;}
  *CAR(a1)=(Int)((a2 != cdr(a1)) ? atmcons : atcons);
  derec;
}


Int  read52(){/* hookage de l'element lu */
   a1=ncons(a1); 
   pop_cast(a2,(Int *)); 
   *CDR(a2)=(Int)a1;
   push(a1); pc=read3;
 }

Int rdmaq(){/* pour le macro-caractere quote  (') */
  rec(readi,rdmaq_1);
}

Int rdmaq_1(){
  if(tlu==BCOM){pushf(rdmaq);rec(readi,readi);}
  else {a1=cons(quote,ncons(a1)); derec;}
}

Int rdmabq(){/* pour le macro-caractere backquote (`) */
  /* noter que (,) n'est macro-caractere que dans un backquote */
  push(tabch[(Int)',']); tabch[(Int)',']=MAC;
  rec(readi,rdmabq_1);
}

Int rdmabq_1(){
  a1=cons(atqua,a1); pop(aux1); tabch[(Int)',']=aux1; derec;
}

Int rdmau(){/* pour le macro-caractere *unquote* (,) */
  rec(getch1,rdmau_1);
}
     
Int rdmau_1(){
  if(a1== (Int*)((Int)'@'))push(atsplice); else{ringur=(Int)a1; push(atunquo);}
  rec(readi,rdmau_2);
}

Int rdmau_2(){
  pop_cast(a2,(Int *)); a1=cons(a2,ncons(a1)); derec;
}

Int implode(){/* (implode l)  subr 1 */
  if(isnil(a1)) derec;
  push(a1);	/* pour gc */
  impl=a1; ++isimpl; rec(readi,implo_1);
}

Int implo_1(){
  isimpl=0; --p; derec;
}

Int tyi(){/* (tyi)  subr 0 */
  /* on lit le caractere de la fenetre courante */
  a1=(Int*)xgetch1(NULL);
  derec;
}


Int readline(){/* (readline) subr 0 */
  printf("readline");
  if(dski)
    {
      aux1=read(fdes,bufin,128); bufin[aux1]=0;
      if(aux1==0){a2=ateof; a1=nil; pc=evalfu; return;}
    }
  else
    rst();	/* lire une ligne de clavier dans bufin */
  pchar=index(bufin,'\n'); *pchar=0; inbpt=pchar; pchar=bufin;
  comstr1();	/* qui se termine par derec */
}

Int readlin(){/* (readlin) subr0 */
  if(dski)
    {
      aux1=read(fdes,bufin,128); bufin[aux1]=0;
      if(aux1==0){a2=ateof; a1=nil; pc=evalfu; return;}
    }
  else
    if(*inbpt == '\n') pc=readline;
    else {
      for(aux1=0;*inbpt!='\n';++aux1)
	bufin[aux1]= *inbpt++;bufin[aux1]='\n';
      pchar=index(bufin,'\n'); *pchar=0; inbpt=pchar; pchar=bufin;
      comstr1();
    }	/* qui se termine par derec */
}

Int typch(){/* (typch char n)   subr 2 */
  if(isatom(a1))a1=((struct atome *)a1)->pn;
  pchar=STR_FROM_STRLSP(a1);
  if(isnil(a2))a1=cranb((Int)tabch[(Int)*pchar]);
  else{tabch[(Int)*pchar]=valnb(a2); a1=a2;}
  derec;
}

Int readch(){/* (readch)  subr 0 */
  rec(getch1,readc_1);
}
Int readc_1(){/* used by itoa */
  newppn();
  aux1=(Int)a1; *ppn++ = aux1; *ppn++ = '\0';
  if(ppn>hstr)gcstrsv();
  int_p1=(Int*)oppn; *int_p1=ppn-oppn-sizeof(Int); pc=crato;
}

Int peekch(){/* (peekch)  subr 0 */
  rec(readch,peekc_1);
}
Int peekc_1(){
  ringur= *(oppn+sizeof(Int)); derec;
}

Int chartyp(){/*  (chartyp c n)  subr 2 */
  if(isatom(a1))a1=((struct atome *)a1)->pn;
  pchar=(CHAR*)a1; a3= (Int*)((Int)*(pchar+sizeof(Int)));
  if(isnil(a2))a1=cranb(tabch[(Int)a3]);
  else
    {
      tabch[(Int)a3]=(CHAR)valnb(a2); a1=a2;
    }
  derec;
}




