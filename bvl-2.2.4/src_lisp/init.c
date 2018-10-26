
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	init.c		initialisations generales et atomes
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz    Decembre 82
 *			Harald Wertz	Fevrier  84
 *                      Ernesto Sendoya Fevrier  91
 *                      Damien Ploix    Fevrier  93
 */


/* EN CAS DE MACH 32 BITS: 4 espaces au lieu de 2 pour chaque pname */




#include <vlisp_files.h>
#include <vlisp.h>
#define LISFR	15

Int auxx1;


#define LINES 1
#define COLS 1



/* pour fabriquer un atome standard */
static void mkat(m_pn,m_ft,m_fv,m_err,m_pk)
     Int (*m_fv)(),m_ft; CHAR *m_pn;Int *m_pk;
{
  ((struct atome *)freeat)->cval=undef; 
  ((struct atome *)freeat)->pval=nil;
  ((struct atome *)freeat)->fval=(Int*)m_fv;  
  ((struct atome *)freeat)->ofval=(Int*)m_fv;
  ((struct atome *)freeat)->ftyp=m_ft;  
  ((struct atome *)freeat)->oftyp=m_ft;
  ((struct atome *)freeat)->err=m_err;
  ((struct atome *)freeat)->ival=nil;   
  ((struct atome *)freeat)->tags=0; 
  ((struct atome *)freeat)->pakage=m_pk;
  ((struct atome *)freeat)->obj=nil;
  ((struct atome *)freeat)->nexa=(ATOMEPTR)derat;
  
  /* On place la longueur de la chaine */
  /* E.S. ici on a cree des chaines completement bidones pour logntemps */
  if( m_pn ){
    newstr( m_pn );
    ((struct atome *)freeat)->pn=(Int*)oppn;
  }
  derat=freeat; freeat+=(SIZAT/sizeof(Int)); /* et on termine l'atome */
  /* et on sauve son adresse dans la hash-table, si c'est un atome standard */

 if (atuser == 0)(void)insert_atome((struct atome *) derat, get_hashind((struct atome *) derat)); 
}


/* les initialisations totales */
#ifndef NO_VFLO
init(tnum,tflo,tstr,tat,tlst ,tpil) 
Int tnum,tflo,tstr,tat,tlst ,tpil;
{
#else
init(tnum,tstr,tat,tlst,twin,tswin,tfrlst,tpil) 
Int tnum,tstr,tat,tlst,twin,tswin,tfrlst,tpil;
 {
#endif
   /* init bords */
   /* | entiers {|floats |}| chaines | atomes | listes | pile */
   /* 40: tolerance strings */
   /* 40: tolerance pile */

   if(!bnum){printf(" * Configuration impossible *\n"); exit(0);}
   aux1=(Int)bnum; aligner(aux1); bnum=(CHAR*)aux1;	/* on cale */
   hnum=bnum+tnum-SIZNUM;
#ifndef NO_VFLO
   bflo=hnum+SIZNUM;
   hflo=bflo+tflo-SIZFLO;
   busrflo = bflo+16;
   bstr=hflo+SIZFLO;
#else
   bstr=hnum+SIZNUM;
#endif      
   hstr=bstr+tstr-1;
   aux1=(Int)bstr;aux1+=LISFR;aux1 &= ~LISFR;bstr=(CHAR*)aux1;
   bat=hstr+41; hat=bat+tat-SIZAT;
   aux1=(Int)bat;aux1+=LISFR;aux1 &= ~LISFR;bat=(CHAR*)aux1;
   blst=hat+SIZAT; hlst=blst+tlst-SIZLST;
   aux1=(Int)blst;
   aux1+=LISFR;aux1 &= ~LISFR;blst=(CHAR*)aux1;


   aux1=(Int)bpil=hlst+SIZLST;

   aligner(aux1);bpil=(CHAR*)aux1;
   hpil=bpil+tpil-SIZPIL;
   /* ets-ce que ca aide? HARALD 6.6.90 */

   p=(Int*)bpil; *p=(Int)0; 
   /* init nbs */
   freenb=0;

#ifndef NO_VFLO
   freeflo=0;
#endif
      
   /* init listes */
   freel=0;
      
   /* init strings */
   ppn=bstr;
      
   /* init gc */
   pgc=0; ngc=0;		/* impression et nb de gc de listes */

   /* init gensym */
   ngsym=0;
      
   /* init atomes */
   freeat=nil=(Int*)bat; derat=(Int*)0; undef=nil+(SIZAT/sizeof(Int*));
      
   initat();	/* on construit l'oblist initiale */
      
   /* on initialise atuserx: adr 1er atome non-systeme */
   atuser=derat+(SIZAT/sizeof(Int));
      
   /* on remplit le reste d'atomes bidon */
      
   auxx1=((hat-(CHAR *)derat)/SIZAT);
      
   while(auxx1--)mkat(NULL,0,0,0,nil);	/* voir si faut garder ou pas */
   /* si on ne le garde pas : plus d'atomes, catastrophe ! */

   freeat=0;
      
   /* super init sorties */
   ((struct atome *)atprlen)->cval=cranb(800);
   LIGNE=2;DEBWIN=2;
 }
      
/* pour tester le hash coding */
extern Int test_hash();

#define erreur_type(val)   ((struct atome *)derat)->err=val

Int initat()
{
  bsysstr = " " ;
  Indvar="    * variable indéfinie : ";
  Indfun="    * fonctions indéfinie : ";
  Errtyp="    * type érroné ou division par 0 : ";
  Errlec="    * erreur lecture : ";
  Errpil="    * pile débordée :";
  Errself="    * self hors fonction";
  Errexit="    * exit hors fonction";

  mkat("nil",0,0,0,nil); nil=derat; *nil=(Int)nil;	/* nil DOIT etre ici */
  mkat("undef",0,0,0,nil); undef=derat;	/* undef DOIT etre ici */
  mkat("? ",0,0,0,nil); atlst=derat;atANNOT=derat;	/* pour readi */
  /* pour le macrocaractere de creation de fenetre */
  mkat("!",0,0,0,nil); 
  mkat("|",0,0,0,nil); 
  mkat("t",0,0,0,nil); t=derat; *t=(Int)t;
  mkat("it",0,0,0,nil); it=derat; *it=(Int)it;
  /* sauvegarde du repertoire dans lequel se trouve bvlisp.sys */
  mkat("VLISPDIR",0,0,0,nil);  newstr(VLISPDIR); ((struct atome *)derat)->cval = (Int*)oppn; 

  mkat("LINES",0,0,0,nil);atLINES=derat;  /* hauteur de l'ecran en caracteres */
  mkat("COLS",0,0,0,nil);atCOLS=derat;    /* largeur de l'ecran en caracteres */
  mkat("WIDTH",0,0,0,nil);atWIDTH=derat;  /* largeur de l'ecran en pixels */
  mkat("HEIGHT",0,0,0,nil);atHEIGHT=derat;/* hauteur de l'ecran en pixels */
  mkat("lmargin",0,0,0,nil); atlmarg=derat; *atlmarg=(Int)0;
  mkat("rmargin",0,0,0,nil); atrmarg=derat; *atrmarg=(Int)78;
  mkat("printlength",0,0,0,nil); atprlen=derat;
  mkat("ALL",0,0,0,nil);atall=derat;*atall=(Int)nil;
  mkat("SELF",0,0,0,nil); atSELF=derat;
  mkat("STEP",0,0,0,nil); atSTEP=derat;*atSTEP=(Int)nil;
  mkat("ANTISL",0,0,0,nil);atANTISL=derat;*atANTISL=(Int)nil;
  mkat("COM",0,0,0,nil);atcomm=derat;*atcomm=(Int)atcomm;/* pour les comment */
  mkat("PCOM",0,0,0,nil);atpcomm=derat;*atpcomm=(Int)atpcomm;/*  les comment */
  mkat("&",0,0,0,nil); atperl=derat;
  mkat("$",0,0,0,nil); atdol=derat; ((struct atome *)derat)->tags= VARIABLE;
  mkat("*cls*",0,0,0,nil); atcls=derat;
  mkat("prog",0,0,0,nil); atprog=derat;
  mkat("go",0,0,0,nil); atgo=derat;
  mkat("example",0,0,0,nil);atexample=derat;*atexample=(Int)atexample;
  mkat("entree",0,0,0,nil);atentree=derat;*atentree=(Int)atentree;
  mkat("sortie",0,0,0,nil);atsortie=derat;*atsortie=(Int)atsortie;
  mkat("crewid",0,0,0,nil);atcrewid=derat;*atcrewid=(Int)nil;
  
/*
  mkat("LISPSTD",0,0,0,nil);atLISPSTD=derat; 
  */
  mkat("*splice*",0,0,0,nil); atsplice=derat;
  mkat("*unquo*",0,0,0,nil); atunquo=derat;
  mkat("bufstr",SUBR0,bufstr,0,nil);
  mkat("eol",SUBR0,eol,0,nil); ateol=derat;
  mkat("prompt",0,0,0,nil); atprompt=derat; *atprompt=(Int)atlst;
  /* mkat("save-screen",SUBR0,cprintw,0,nil); */
  mkat("localvar",0,0,0,nil); atlocalvar=derat;*atlocalvar=(Int)nil;
  mkat("globalvar",0,0,0,nil); atglobalvar=derat;*atglobalvar=(Int)nil;
  mkat("all_functions",0,0,0,nil); atall_functions=derat;*atall_functions=(Int)nil;
  mkat("all_clauses",0,0,0,nil); atall_clauses=derat;*atall_clauses=(Int)nil;
  mkat("onintr",0,0,0,nil);atONINTR=derat;
  mkat("EOF",0,0,0,nil);atEOF=derat;
  mkat("bvlisp",0,0,0,nil);atbvlisp=derat;
  
  /* */
  mkat("mapc",0,0,0,nil);
  mkat("mapcar",0,0,0,nil);
  mkat("mapct",0,0,0,nil);
  mkat("do",0,0,0,nil);
  mkat("chemin1",0,0,0,nil);
  mkat("%%remprop",0,0,0,nil);
  mkat("chemin",0,0,0,nil);
  mkat("chemin-sortie",0,0,0,nil);
  mkat("chemin-entree",0,0,0,nil);
  mkat("lc1",0,0,0,nil);
  mkat("first-call",0,0,0,nil);
  mkat("tyi-or-print",0,0,0,nil);
  mkat("existe-qqc",0,0,0,nil);
  mkat("read-objets",0,0,0,nil);
  mkat("read-objet",0,0,0,nil);
  mkat("alltrans",0,0,0,nil);
  mkat("trans",0,0,0,nil);
  mkat("not-all",0,0,0,nil);
  mkat("all",0,0,0,nil);
  mkat("assert-sortie",0,0,0,nil);
  mkat("assert-sor",0,0,0,nil);
  mkat("%%aux",0,0,0,nil);
  mkat("%POS-sortie",0,0,0,nil);
  mkat("%POS-entree",0,0,0,nil);
  mkat("assert-entree",0,0,0,nil);
  mkat("assert-ent",0,0,0,nil);
  mkat("test-sortie",0,0,0,nil);
  mkat("test-entree",0,0,0,nil);
  mkat("pop",0,0,0,nil);
  mkat("push",0,0,0,nil);
  mkat("%pile",0,0,0,nil);
  mkat("reassert-s",0,0,0,nil);
  mkat("reassert-e",0,0,0,nil);
  mkat("as-so-dummy",0,0,0,nil);
  mkat("unassert-s",0,0,0,nil);
  mkat("unassert-e",0,0,0,nil);
  mkat("%enleve",0,0,0,nil);
  mkat("enleve-sortie",0,0,0,nil);
  mkat("enleve-entree",0,0,0,nil);
  mkat("ind",0,0,0,nil);
  mkat("%ubf1",0,0,0,nil);
  mkat("%self",0,0,0,nil);
  mkat("%ubf",0,0,0,nil);atubf=derat;
  mkat("%ubv1",0,0,0,nil);
  mkat("%ubv",0,0,0,nil);atubv=derat;
  mkat("%ubt",0,0,0,nil);attyp=derat;
  mkat("%union",0,0,0,nil);
  mkat("%desca",0,0,0,nil);
  mkat("%descf",0,0,0,nil);
  mkat("%desc",0,0,0,nil);
  mkat("%descr",0,0,0,nil);
  mkat("%suprev",0,0,0,nil);
  mkat("flat",0,0,0,nil);
  mkat("%indexx",0,0,0,nil);
  mkat("%a",0,0,0,nil);
  mkat("%i",0,0,0,nil);
  mkat("%index",0,0,0,nil);
  mkat("%find",0,0,0,nil);
  mkat("%vard1",0,0,0,nil);
  mkat("%varde",0,0,0,nil);
  mkat("getthem",0,0,0,nil);
  mkat("%outpot",0,0,0,nil);
  mkat("%prina",0,0,0,nil);
  mkat("%enl",0,0,0,nil);
  mkat("%mpc1",0,0,0,nil);
  mkat("%mpc",0,0,0,nil);
  mkat("%prinar",0,0,0,nil);
  mkat("%trac",0,0,0,nil);
  mkat("f-entree", 0,0,0,nil); /* fonction d'entree non-prise en compte */
  mkat("f-sortie", 0,0,0,nil); /* fonction d'sortie non-prise en compte */
  /* et les fonctions standard */
  mkat("getpackage",SUBR1,getpackage,0,nil);
  mkat("changeval",SUBR1,changeval,0,nil);

/* les macro caracteres sont traites par la table tabfval (.ftyp, .fval)
  mkat("'",SUBR0,rdmaq,0,nil);
  mkat("`",SUBR0,rdmabq,0,nil);
  mkat(",",SUBR0,rdmau,0,nil);  */
  mkat(" ",0,0,0,nil); atmc=derat; *atmc=(Int)nil;   /* atome d'execution... */
  tabfval['\''].ftyp = SUBR0; tabfval['\''].fval = (Int *) rdmaq;
  tabfval['`'].ftyp = SUBR0;  tabfval['`'].fval = (Int *) rdmabq;
  tabfval[','].ftyp = SUBR0;  tabfval[','].fval = (Int *) rdmau;
  
  mkat("cut",SUBR0,cut,0,nil); ((struct atome *)derat)->tags= EVALUABLE;
  mkat("fail",SUBR0,fail,0,nil); ((struct atome *)derat)->tags= EVALUABLE;
  mkat("oblist",SUBR0,oblist,0,nil);
  mkat("myoblist",SUBR0,myoblist,0,nil); /* oblist avec du hash */
  mkat("test_hash",SUBR0,test_hash,0,nil); /* oblist avec du hash */
  mkat("check_hash",SUBR0,check_hash,0,nil);
  mkat("macout",SUBR0,macout,0,nil);
  mkat("nomacout",SUBR0,nomacout,0,nil);
  mkat("fork",SUBR0,forkp,0,nil);
  mkat("user",SUBR0,userp,0,nil);
  mkat("tty",SUBR0,ttyp,0,nil);
  mkat("argc",SUBR0,argcp,0,nil);
  mkat("read",SUBR0,readu,0,nil); atread=derat;
  mkat("readline",SUBR0,readline,0,nil);
  mkat("readlin",SUBR0,readlin,0,nil);
  mkat("readch",SUBR0,readch,0,nil);
  mkat("peekch",SUBR0,peekch,0,nil);
  mkat("tyi",SUBR0,tyi,0,nil);
  mkat("eof",SUBR0,eof,0,nil); ateof=derat;
/*
  mkat("wterpri",SUBR1,wterpri,0,nil);
*/
  mkat("terpri",SUBR0,terpri,0,nil);
  mkat("stop",SUBR0,stop,0,nil);
  mkat("toplevel",SUBR0,toplv,0,nil); attop=derat;*attop=(Int)derat;


  mkat("date",SUBR0,date,0,nil);
  mkat("timing",SUBR0,timep,0,nil);

/*
  mkat("restart",SUBR0,restart,0,nil);
*/
  mkat("wait",SUBR1,waitp,0,nil);
  mkat("time",SUBR1,timepp,0,nil);
  mkat("careful",SUBR1,careful,0,nil); atcareful=derat;*atcareful=(Int)nil;
  mkat("obase",SUBR1,obase,0,nil);atobase=derat;*atobase=(Int)10;
  mkat("ibase",SUBR1,ibase,0,nil);atibase=derat;*atibase=(Int)10;
  mkat("isuser",SUBR1,isuser,0,nil);
  /* ?? mkat("window",SUBR1,window,0,nil); */
  mkat("output",SUBR1,output,0,nil);
  mkat("unwind",SUBR1,unwind,0,nil);
  mkat("lognot",SUBR1,lognot,0,nil);
  mkat("smash",SUBR1,smash,0,nil);
  /* mkat("ctrlz",SUBR1,ctrlz,0,nil); */
  mkat("remob",SUBR1,remob,0,nil);
  mkat("itoa",SUBR1,itoap,0,nil);
  mkat("atoi",SUBR1,atoip,0,nil);
  mkat("argv",SUBR1,argvp,0,nil);
#ifndef NOANNOT
  mkat("clearcar",SUBR1,clearcar,0,nil);
  mkat("clearcdr",SUBR1,clearcdr,0,nil);
#endif
  mkat("getenv",SUBR1,getenp,0,nil);
  mkat("fread",SUBR1,freadp,0,nil);
  mkat("gc",SUBR1,gc,0,nil); atgc = derat;
  mkat("eval",SUBR1,eval_no_careful,0,nil);  
  /* positionnement des pointeurs en careful = nil */
    ateval=derat; 
    eval = eval_no_careful; 
    evca1 = evca1_no_careful;
    evalfu = evalfu_no_careful;
  /* ... */
  mkat("1+",SUBR1,add1,0,nil);
  mkat("1-",SUBR1,sub1,0,nil);
#ifndef NO_VFLO
  mkat("PI", 0,0,0,nil); atPI = derat;
  mkat("sin",SUBR1,sinp,0,nil);
  mkat("cos",SUBR1,cosp,0,nil);
  mkat("asin",SUBR1,asinp,0,nil);
  mkat("acos",SUBR1,acosp,0,nil);
  mkat("atan",SUBR1,atanp,0,nil);
  mkat("atan2",SUBR2,atan2p,0,nil);
  mkat("sqrt",SUBR1,sqrtp,0,nil);
  mkat("fix",SUBR1,pfix,0,nil);
  mkat("float",SUBR1,pfloat,0,nil);
  mkat("fixp",SUBR1,pfixp,0,nil);
  mkat("floatp",SUBR1,pfloatp,0,nil);
  mkat("log",SUBR1,logp,0,nil);
  mkat("exp",SUBR1,expp,0,nil);
#endif
  mkat("lisp?",SUBR1,lisp,0,nil); ((struct atome *)derat)->tags= EVALUABLE;
  mkat(":=",SUBR1,affpro,0,nil); ((struct atome *)derat)->tags= EVALUABLE;
  mkat("var",SUBR1,var,0,nil); ((struct atome *)derat)->tags= EVALUABLE;
  mkat("univ",SUBR1,univ,0,nil); ((struct atome *)derat)->tags= EVALUABLE;
#ifdef SFI_TRACE
  mkat("sfi-trace",SUBR0,sfi_trace,0,nil);
  mkat("sfi-mark",SUBR0,sfi_mark,0,nil);
#endif

  mkat("macroexpand",SUBR1,macexp,0,nil);
  mkat("abs",SUBR1,Abs,0,nil);
  mkat("unboundp",SUBR1,unbou,0,nil);
  mkat("rtext",SUBR1,rtext,0,nil);
  mkat("car",SUBR1,carp,ERR_NONS,nil); 
  mkat("cdr",SUBR1,cdrp,ERR_NONS,nil); 
#ifndef NOANNOT
  mkat("getcar",SUBR1,getcar,ERR_NONS,nil); 
  mkat("getcdr",SUBR1,getcdr,ERR_NONS,nil); 
#endif
  mkat("caar",SUBR1,caar,ERR_NONS,nil); 
  mkat("cadr",SUBR1,cadr,ERR_NONS,nil); 
  mkat("cdar",SUBR1,cdar,ERR_NONS,nil); 
  mkat("cddr",SUBR1,cddr,ERR_NONS,nil); 
  mkat("caaar",SUBR1,caaar,ERR_NONS,nil); 
  mkat("caadr",SUBR1,caadr,ERR_NONS,nil); 
  mkat("cadar",SUBR1,cadar,ERR_NONS,nil); 
  mkat("caddr",SUBR1,caddr,ERR_NONS,nil); 
  mkat("cdaar",SUBR1,cdaar,ERR_NONS,nil); 
  mkat("cdadr",SUBR1,cdadr,ERR_NONS,nil); 
  mkat("cddar",SUBR1,cddar,ERR_NONS,nil); 
  mkat("cdddr",SUBR1,cdddr,ERR_NONS,nil); 
  mkat("clrival",SUBR1,clrival,ERR_ATOME,nil); 
  mkat("eprogn",SUBR1,progn,0,nil);
  mkat("evlis",SUBR1,evlis,ERR_NONS,nil); 
  mkat("null",SUBR1,nullp,0,nil);
  mkat("not",SUBR1,nullp,0,nil);
  mkat("tyo",SUBR1,tyo,0,nil);
  mkat("tyoesc",SUBR1,tyoesc,0,nil);
  mkat("length",SUBR1,length,ERR_NONS,nil); 
  mkat("atom",SUBR1,atom,0,nil);
  mkat("atomp", SUBR1, atomp,0, nil); 
  mkat("error-ubv",SUBR1,erindvar,0,nil);atindvar=derat;atubv=derat;
  mkat("error-pile",SUBR1,erpil,0,nil);aterpil=derat;atpil=derat;
  /* par défaut on ne vérifie pas les erreurs de types ou de division par 0 ... */
  mkat("error-type",SUBR1,ertyparg,0, nil);atertyp=derat;attyp=derat; *derat=(Int)undef; 
  mkat("litatom",SUBR1,litatom,0,nil);
  mkat("listp",SUBR1,listp,0,nil);
  mkat("numbp",SUBR1,numbp,0,nil);
  mkat("plength",SUBR1,plength,ERR_NONS,nil); 
  mkat("stringp",SUBR1,stringp,0,nil);
  mkat("session",SUBR1,sessp,0,nil); 
  mkat("sh",SUBR1,sh,0,nil);
  mkat("ssh",SUBR1,ssh,0,nil);
  mkat("ls",SUBR1,ls,0,nil);
  mkat("cwd",SUBR1,change_repertoire,0,nil);
  mkat("implode",SUBR1,implode,0,nil);
  mkat("explode",SUBR1,explode,0,nil);
  mkat("outpos",SUBR1,outpos,0,nil);
  mkat("random",SUBR1,randomp,0,nil); atrandom=derat; *atrandom=1;
  mkat("zerop",SUBR1,zerop,0,nil);
  mkat("copy",SUBR1,copy,ERR_NONS,nil);       
  mkat("copy-all",SUBR1,copyall,ERR_NONS,nil);
  mkat("probef",SUBR1,probef,0,nil);
  mkat("input",SUBR1,linput,0,nil);
  mkat("boundp",SUBR1,boundp,0,nil);

  mkat("frame",SUBR2,frame,0,nil);atframe=derat;
  mkat("stol",SUBR2,stol,0,nil);
  mkat("ltos",SUBR2,ltos,0,nil);
  mkat("chartyp",SUBR2,chartyp,0,nil);
  mkat("**",SUBR2,power,0,nil);
  mkat("cassq",SUBR2,cassq,0,nil);
  mkat("nmemq",SUBR2,nmemq,ERR_NONS|ERR_ARG2,nil); 
  mkat("vmemq",SUBR2,vmemq,ERR_NONS|ERR_ARG2,nil); 
  mkat("logand",SUBR2,logand,0,nil);
  mkat("logor",SUBR2,logor,0,nil);
  mkat("logxor",SUBR2,logxor,0,nil);
  mkat("logshift",SUBR2,logshift,0,nil);
  mkat("dupl",SUBR2,dupl,0,nil);
  mkat("sublis",SUBR2,sublis,0,nil);
#ifndef NOANNOT
  mkat("putcar",SUBR2,putcar,ERR_NONS,nil); 
  mkat("putcdr",SUBR2,putcdr,ERR_NONS,nil); 
#endif
  mkat("strincp",SUBR2,stringi,0,nil);

  mkat("strcmp",SUBR2,strcmpp,0,nil);
  mkat("attach",SUBR2,attach,0,nil); atattach=derat;
  mkat("nreverse",SUBR2,nreverse,ERR_NONS,nil); atnreverse=derat; 
  mkat("memq",SUBR2,memq,ERR_NONS|ERR_ARG2,nil); 
  mkat("assq",SUBR2,assq,ERR_NONS|ERR_ARG2,nil); 
  mkat("delq",SUBR2,delq,ERR_NONS|ERR_ARG2,nil); 
  mkat("typch",SUBR2,typch,0,nil);
  mkat("error-ubf",SUBR2,erindfun,0,nil); atindfun=derat;atubf=derat;
  mkat("strcar",SUBR2,strcar,ERR_NONL,nil);
  mkat("strcdr",SUBR2,strcdrp,ERR_NONL,nil);
  mkat("apply",SUBR2,apply_no_careful,0,nil);atapply=derat; apply=apply_no_careful;
  mkat("cons",SUBR2,consp,0,nil); atcons=derat;
  mkat("nth",SUBR2,nth,ERR_NONS|ERR_ARG2,nil); atnth=derat; 
  mkat("+",SUBR2,plus,0,nil);
  mkat("-",SUBR2,differ,0,nil);atminus=derat;
  mkat("*",SUBR2,mul,0,nil);
  mkat("=",SUBR2,eq,0,nil);
  mkat("eq",SUBR2,eq,0,nil);
  mkat("le",SUBR2,le,0,nil);
  mkat("fval",SUBR2,fval,ERR_ATOME,nil); 
  mkat("ftyp",SUBR2,ftyp,ERR_ATOME,nil); 
  mkat("ival",SUBR2,ival,ERR_ATOME,nil);
  mkat("append",SUBR2,append,0,nil);
  mkat("reverse",SUBR2,reverse,ERR_NONS,nil); 
  mkat("nconc",SUBR2,nconc,0,nil); atnconc=derat;
  mkat("set",SUBR2,set,ERR_NONS,nil); atset=derat; 
  mkat("rplaca",SUBR2,rplaca,ERR_NONS,nil); atplaca=derat; 
  mkat("rplacd",SUBR2,rplacd,ERR_NONS,nil); atplacd=derat; 
  mkat("rplacb",SUBR2,rplacb,ERR_NONS,nil); atplacb=derat; 
  mkat("ge",SUBR2,ge,0,nil);
  mkat("<",SUBR2,lt,0,nil);
  mkat(">",SUBR2,gt,0,nil);
  mkat("neq",SUBR2,neq,0,nil);
  mkat("/",SUBR2,quo,ERR_NOZERO|ERR_ARG2,nil);   
  mkat("rem",SUBR2,rem,ERR_NOZERO|ERR_ARG2,nil); 
  mkat("equal",SUBR2,equal,0,nil);
  mkat("remprop",SUBR2,remprop,ERR_NONS,nil); 
  mkat("get",SUBR2,get,ERR_NONS,nil);      
  mkat("member",SUBR2,member,ERR_NONS|ERR_ARG2,nil);
  mkat("princh",SUBR2,princh,0,nil);
  mkat("assoc",SUBR2,assoc,ERR_NONS|ERR_ARG2,nil); 
  mkat("delete",SUBR2,delete,ERR_NONS|ERR_ARG2,nil); 
  mkat("deset",SUBR2,deset,0,nil);
  mkat("closure",SUBR2,closure,0,nil);atclosure=derat;
  mkat("last",SUBR2,last,ERR_NONS,nil); 
  
  mkat("setcassq",SUBR3,setcassq,0,nil);
  mkat("rplac",SUBR3,rplac,ERR_NONS,nil);     
  mkat("addprop",SUBR3,addprop,ERR_NONS,nil); 
  mkat("subst",SUBR3,subst,ERR_NONS|ERR_ARG3,nil); 
  mkat("pairlis",SUBR3,pairlis,0,nil);
  
  mkat("oval",NSUBR,oval,0,nil);
  mkat("error",NSUBR,error,0,nil);aterror=derat;
  mkat("print",NSUBR,print,0,nil); atprint=derat;
/*
  mkat("wprint",NSUBR,wprint,0,nil); 
  mkat("wprin1",NSUBR,wprin1,0,nil); 
  */
  mkat("allprint",NSUBR,allprint,0,nil);
  mkat("allprin1",NSUBR,allprin1,0,nil);
  mkat("extprint",NSUBR,extprint,0,nil);
  mkat("extprin1",NSUBR,extprin1,0,nil);
  mkat("princ",NSUBR,princ,0,nil);
  mkat("prin1",NSUBR,prin1,0,nil);
  mkat("mcons",NSUBR,mcons,0,nil); atmcons=derat;
  mkat("strlen",SUBR1,strlenp,0,nil);
  mkat("strcat",NSUBR,strcatp,0,nil);
  mkat("internal",NSUBR,internal,0,nil);
  mkat("internalprint",NSUBR,intern_1,0,nil);
  mkat("put",NSUBR,put,ERR_NONS,nil); 
  mkat("gensym",NSUBR,gensym,0,nil);
  mkat("step",NSUBR,step,0,nil);atstep=derat;
  
  mkat("prolog",FSUBR,prolog,0,nil); atprolog=derat; *atprolog=(Int)nil;
  mkat("assert",FSUBR,assert,0,nil); atassert=derat;
  
  mkat("background",FSUBR,background,0,nil);
  mkat("pour",FSUBR,pour,0,nil);
  mkat("unstep",FSUBR,unstep,0,nil);
  mkat("break",FSUBR,Break,0,nil);
  mkat("package",FSUBR,package,0,nil);atpackage=derat;*atpackage=(Int)nil;
  mkat("enable",FSUBR,enable,0,nil);
  mkat("disable",FSUBR,disable,0,nil);
  mkat("quote",FSUBR,carp,0,nil); quote=derat; *quote=(Int)quote;
  mkat("let",FSUBR,llet,0,nil);atlet=derat;
  mkat("lock",FSUBR,lockp,0,nil);atlock=derat;
  mkat("letdic",FSUBR,letdic,0,nil);
  mkat("letdicq",FSUBR,letdicq,0,nil);
  mkat("unwind-protect",FSUBR,unwinp,0,nil);atprotect=derat;
  mkat("quasiquote",FSUBR, quasiquote,0,nil); atqua=derat;
  mkat("de",FSUBR,de,0,nil);atde=derat;
  mkat("df",FSUBR,df,0,nil);
  mkat("dm",FSUBR,dm,0,nil);
  mkat("dmo",FSUBR,dmo,0,nil);
  mkat("dmc",FSUBR,dmc,0,nil);
  mkat("undmc",FSUBR,undmc,0,nil);
  mkat("typech",FSUBR,typech,0,nil);
  mkat("redmc",FSUBR,redmc,0,nil);
  mkat("dml",FSUBR,dml,0,nil);
  mkat("open",FSUBR,lopen,0,nil);
  mkat("close",SUBR1,lclose,0,nil);
  mkat("lambda",FSUBR,lambdap,0,nil); lambda=derat; *lambda=(Int)lambda;
  mkat("progn",FSUBR,progn,0,nil); atprogn=derat;
  mkat("prog1",FSUBR,prog1,0,nil);
  mkat("list",FSUBR,evlis,0,nil); atlist=derat;
  mkat("if",FSUBR,ifp,0,nil);
  mkat("ifn",FSUBR,ifnp,0,nil);
  mkat("when",FSUBR,whenp,0,nil);
  mkat("unless",FSUBR,unlessp,0,nil);
  mkat("self",FSUBR,self,0,nil);atself=derat;
  mkat("done",FSUBR,done,0,nil);atdone=derat;
  mkat("undo",FSUBR,undo,0,nil);atundo=derat;
  mkat("exit",FSUBR,exitp,0,nil);at_exit=derat;
  mkat("escape",FSUBR,escape,0,nil);atescape=derat;
  mkat("letf",FSUBR,letf,0,nil);
  mkat("setq",FSUBR,setq,ERR_NONS,nil); atsetq=derat;    
  mkat("nextl",FSUBR,nextl,ERR_NONS,nil); atnextl=derat; 
  mkat("newl",FSUBR,newl,ERR_NONS,nil); atnewl=derat;    
  mkat("while",FSUBR,whilep,0,nil);
  mkat("until",FSUBR,until,0,nil);
  mkat("incr",FSUBR,incr,ERR_NONS,nil); atincr=derat;    
  mkat("decr",FSUBR,decr,ERR_NONS,nil); atdecr=derat;    
  mkat("repeat",FSUBR,repeat,0,nil);
  mkat("lib",FSUBR,lib,0,nil);
  mkat("include",FSUBR,include,0,nil);
  mkat("cond",FSUBR,cond,0,nil);atcond=derat;
  mkat("and",FSUBR,and,0,nil);
  mkat("or",FSUBR,or,0,nil);
  mkat("config",FSUBR,config,0,nil);
  mkat("selectq",FSUBR,selectq,0,nil);atsele=derat;
  mkat("status",FSUBR,status,0,nil);


  mkat("atomuses",SUBR0,atomuses,0,nil);
  mkat("struses",SUBR0,struses,0,nil);
  mkat("listuses",SUBR0,listuses,0,nil);
  mkat("flouses",SUBR0,flouses,0,nil);
  mkat("numuses",SUBR0,numuses,0,nil);
  mkat("piluses",SUBR0,piluses,0,nil);
 
  mkat("SOcreate",SUBR3,soCreate,0,nil) ;
  mkat("SOclose", SUBR1,soClose,0, nil) ;
  mkat("SOconnect", SUBR3, soConnect,0, nil) ;
  mkat("SOlisten", SUBR2, soListen,0, nil) ;
  mkat("SOwrite", NSUBR, soWrite,0, nil) ;
  mkat("SOread", SUBR3, soRead,0, nil) ;
  mkat("SOhostname", SUBR0, soHostName,0, nil) ;

  initlibname(); /* nom des fichiers de librairie standart (fil.c) */
  hsysstr = " " ;
  newppn() ; 
  hdsysstr = oppn ;
}

