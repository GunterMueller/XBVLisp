
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
#include <xwin.h>
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
/*   
  mkat("all_windows",0,0,0,nil); atall_windows=derat;*atall_windows=(Int)nil; 
  */
  mkat("onintr",0,0,0,nil);atONINTR=derat;
  mkat("EOF",0,0,0,nil);atEOF=derat;
  mkat("bvlisp",0,0,0,nil);atbvlisp=derat;
  
  /* pour la proprete de la doc, tous les atomes de bvlisp.sys */

  mkat("xMoveWidget", 0, 0,0,nil);
  mkat("xResizeWidget", 0, 0,0,nil); 
  mkat("xInitXbvlisp",0,0,0,nil);    
  mkat("popupDisplay",0,0,0,nil);
  mkat("xCreateXbvlisp",0,0,0,nil);
  mkat("xGetSupWidget",0,0,0,nil);
  mkat("xGetHierarchie",0,0,0,nil);
  mkat("xGetSousArbre",0,0,0,nil);

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
  mkat("stdmenu",SUBR1,stdmenu,0,nil);
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
  
  mkat("menu",SUBR2,menu,0,nil);atmenu=derat;*atmenu=(Int)nil;
  mkat("frame",SUBR2,frame,0,nil);atframe=derat;
  mkat("stol",SUBR2,stol,0,nil);
  mkat("ltos",SUBR2,ltos,0,nil);
  mkat("chartyp",SUBR2,chartyp,0,nil);
  mkat("**",SUBR2,power,0,nil);
  mkat("cassq",SUBR2,cassq,0,nil);
  mkat("nmemq",SUBR2,nmemq,ERR_NONS|ERR_ARG2,nil); 
  mkat("vmemq",SUBR2,vmemq,ERR_NONS|ERR_ARG2,nil); 
  mkat("logand",SUBR2,logand,0,nil);
  mkat("aide",NSUBR,aide,0,nil); ataide=derat; *ataide = (Int)nil;
  mkat("sections", SUBR1, section,0, nil);
  mkat("listesection", SUBR2, listesection,0, nil);
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

  mkat("print_strings", SUBR0, print_strings,0, nil);

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
  mkat("make-menu",FSUBR,makemenu,0,nil);
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

  /* ***************************** */
  /* primitives pour l'interface X */
  /* ***************************** */

  mkat("xMap",NSUBR,MapWid,0,nil);
  mkat("xUnmap",NSUBR,UnMapWid,0,nil);
  mkat("xRemoveWidget",NSUBR,xRemoveWidget,0,nil);
  mkat("xwst",NSUBR,xwst,0,nil);
  mkat("xGrab",SUBR1,grab,0,nil);
  mkat("xPopup",SUBR2,xPopup,0,nil);
  mkat("xPopdown",SUBR1,xPopdown,0,nil);
  mkat("xUngrab",SUBR0,ungrab,0,nil);
  mkat("xflush",SUBR0,xflush,0,nil);
  mkat("sleep",NSUBR,xsleep,0,nil);
  mkat("xwdim",NSUBR,ldim,0,nil);
  mkat("xBeep",SUBR1,xBeep,0,nil);

  mkat("xwinp",SUBR0,lwin,0,nil);
  mkat("xwindowp",NSUBR,lwindowp,0,nil);

  mkat("isobjet",SUBR1,isobjet,0,nil);
  mkat("isressource",SUBR1,isressource,0,nil);
  mkat("iswidget",SUBR2 ,iswidget,0,nil);

  mkat("xChangeWin",NSUBR,lchangewin,0,nil);
  
  mkat("xPosPointer",NSUBR,pospointer,0,nil);
  mkat("xGetPosPointer",SUBR1,xgetpospointer,0,nil);

  /* capacites video des fenetres  X */
  mkat("home",NSUBR,home,0,nil);
  mkat("cleol",NSUBR,cleol,0,nil);
  mkat("poscur",NSUBR,poscur,0,nil);
  mkat("cleos",NSUBR,cleos,0,nil);
  mkat("left",NSUBR,gauche,0,nil);
  mkat("shift_left",NSUBR,shift_gauche,0,nil);
  mkat("right",NSUBR,droite,0,nil);
  mkat("shift_right",NSUBR,shift_droite,0,nil);
  mkat("back",NSUBR,back,0,nil); /* backspace sur la fenetre nommee */
  mkat("up",NSUBR,haut,0,nil);
  mkat("down",NSUBR,bas,0,nil);
  mkat("cursorpos",NSUBR,cursorpos,0,nil);

  mkat("xscroll",NSUBR, xscroll,0, nil);
  mkat("xEnable",SUBR0, xEnable,0, nil);
  mkat("xDisable",SUBR0, xDisable,0, nil);
  mkat("xRealize", NSUBR, xRealize,0, nil);
  mkat("xUnrealize", NSUBR, xUnrealize,0, nil);
  mkat("xManage", NSUBR, xManage,0, nil);
  mkat("xUnmanage", NSUBR, xUnmanage,0, nil);

  mkat("xSetValues", NSUBR, xSetValues,0, nil);
  mkat("xGetValues", NSUBR, xGetValues,0, nil);

  mkat("xGetWindow", SUBR1, xGetWindow,0, nil);

  mkat("xGetResourceType", SUBR2, xGetResRepType,0, nil);
  mkat("xGetResourceList", SUBR1, xGetResourceList,0, nil);
  mkat("xGetNewResourceList", SUBR1, xGetNewResourceList,0, nil);
  mkat("xGetAllResourceLists", SUBR0, xGetAllResourceLists,0, nil);

  mkat("xCircSubWidgets", NSUBR, xCircSubWidgets,0, nil);


  /*************************************/
  /*        a modifier                 */
  
  /*
    mkat("xconfirm",NSUBR,xconfirm,0,nil);
    mkat("xdialog",NSUBR,xdialog,0,nil);
    
    mkat("xsetState", NSUBR, xsetState,0, nil);
    mkat("xsetTrace", NSUBR, xsetTrace,0, nil);
    */
  /* essais de configuration de GCs */

  /*************************************/
  /*      primitives graphiques        */
  mkat("xDrawLines", NSUBR, xDrawLine,0, nil);
  mkat("xDrawRays", NSUBR, xDrawRay,0, nil);
  mkat("xDrawPoints", NSUBR, xDrawPoints,0, nil);
  mkat("xDrawRectangles", NSUBR, xdrawrectangle,0, nil);
  mkat("xFillRectangles", NSUBR, xfillrectangle,0, nil);
  mkat("xDrawArcs", NSUBR, xdrawarc,0, nil);
  mkat("xFillArcs", NSUBR,xfillarc,0, nil);
  mkat("xDrawString", NSUBR, xdrawstring,0, nil);
  mkat("xGetStringDimension", SUBR2, xgetstringdim,0, nil);
  mkat("xClearArea", NSUBR, xcleararea,0, nil);
  mkat("xFillPolygon", NSUBR, xFillPolygon,0, nil);
  mkat("xSetForeground", SUBR2, xsetForeground,0, nil);
  mkat("xSetBackground", SUBR2, xsetBackground,0, nil);
  mkat("xDisplayBitmap", NSUBR, xDisplayBitmap,0, nil);
  mkat("xModifyGC", SUBR3, xmodifyGC,0, nil);
  mkat("xGetGCValue", SUBR2, xgetGCvalue,0, nil);

  /*************************************/
  /*        a eliminer                 */
  /* la tortue */
  /*
  mkat("xcretortue", NSUBR, xcretortue,0, nil);
  mkat("deplacetortue", NSUBR, deplace_tortue,0, nil);
  mkat("postortue", NSUBR, pos_tortue,0, nil);
  mkat("getpostortue", NSUBR, getpos_tortue,0, nil);
  mkat("pencilOn", NSUBR, pencilOn,0, nil);
  mkat("pencilOff", NSUBR, pencilOff,0, nil);
  mkat("tortueOn", NSUBR, tortueOn,0, nil);
  mkat("tortueOff", NSUBR, tortueOff,0, nil);
  */
  /*************************************/


  /* la Tortue */
  mkat("AVANCE", SUBR3, AvanceTortue,0, nil);

  /* manipulation du curseur */
  mkat("xGetCursorpos", NSUBR, xGetCursorpos,0, nil);
  /* pour travailler avec les sources */
  mkat("xchgsrc", NSUBR, xchgsrc,0, nil);
  mkat("xwsave", NSUBR, xwsave,0, nil);
  /* pour creer les premiers objets graphiques */
  mkat("xCreateResource", NSUBR, xCreateResource,0, nil);
  mkat("xsetRessource", SUBR2, xsetRessource,0, nil);
  mkat("xCreateWidget", NSUBR, xcreeWidget,0, nil);
  mkat("xWidgetParent", SUBR1, xWidgetParent,0, nil);
  /* ajouter des callbacks-lisp au widgets */
  mkat("xAddCallback",SUBR3,xaddCallback,0,nil);
  /* modification des trables de translation */
  mkat("xAugment", SUBR3,xAugmentTranslation,0,nil);
  mkat("xOverride", SUBR3,xOverrideTranslation,0,nil);
  /* gestion des evenements */
  mkat("xCheckEvent", SUBR2,xCheckEvent,0,nil);
  mkat("xIsRealized", SUBR1,xisrealized,0,nil);

#if (OPEN_GL | MESA_GL)

  /* ********************** */
  /* Interface avec Open GL */
  /* ********************** */

#ifdef MODIFIEDMESA
  mkat("MESAcopylist", SUBR3, mesaCopy, 0, nil);
  mkat("MESAwritelist", SUBR3, mesaWriteList, 0, nil);
  mkat("MESAreadlist", SUBR2, mesaReadList, 0, nil);
  mkat("MESAgetlist", SUBR1, mesaGetList, 0, nil);
#endif


  mkat("OPENGL",0,0,0,nil);   *derat=(Int)t;
  /* implementation de la librairie Open GL dans xbvlisp... */
  /* opérations liées aux fenêtres Open GL */
  mkat("GLwinopen", NSUBR,gl_winopen,0,nil);
  mkat("GLwinset",SUBR1,gl_winset,0,nil);
  mkat("GLcallback",SUBR2,gl_addcallback,0, nil);
  mkat("GLdump", SUBR3, gl_dump_window,0, nil);
  mkat("GLloadTiff", NSUBR, gl_read_tiff,0, nil);
  mkat("GLloadRGB", NSUBR, gl_read_rgb,0, nil);
  mkat("GLpolygonStipple", SUBR1, gl_load_polygon_stipple,0, nil);
  /* projections */
  mkat("GLortho",NSUBR,gl_ortho,0,nil);
  mkat("GLfrustum",NSUBR,gl_frustum,0,nil);
  mkat("GLperspective",SUBR3,gl_perspective,0,nil);
  mkat("GLautopersp", SUBR0,gl_auto_perspective,0,nil);
  mkat("GLlookat",NSUBR,gl_lookat,0,nil);
  mkat("GLpolarview",NSUBR,gl_polarv,0,nil);
  /* action sur l'image */
  mkat("GLimrotateOrder", SUBR3, gl_rotate_order,0, nil);
  mkat("GLimtranslate",SUBR3,gl_translate_image,0,nil);
  mkat("GLimrotate",SUBR3,gl_rotate_image,0,nil);
  mkat("GLimscale",NSUBR,gl_scale_image,0,nil);
  /* RAZ */
  mkat("GLclear",SUBR0,gl_clear,0,nil);
  /* nomage */
  mkat("GLloadname",SUBR1,gl_loadnom,0,nil);
  mkat("GLpushname",SUBR1,gl_pushnom,0,nil);
  mkat("GLpopname",SUBR0,gl_popnom,0,nil);
  /* gestion des matrices */
  mkat("GLmmode",SUBR1,gl_mmode,0,nil);
  mkat("GLpushmatrix",SUBR0,gl_pushmatrice,0,nil);
  mkat("GLpopmatrix",SUBR0,gl_popmatrice,0,nil);
  mkat("GLloadidmatrix",SUBR0,gl_loadidmatrix,0,nil);
  mkat("GLscale",NSUBR,gl_echelle,0,nil);
  mkat("GLtranslate",SUBR3,gl_translat,0,nil);
  mkat("GLrotate",SUBR3,gl_rotate,0,nil);
  mkat("GLrotateAxis", SUBR3, gl_rotate_axis,0, nil);
  /* autres contrôles sur les matrices */
  mkat("GLgetMatrix",SUBR1, gl_get_matrix,0, nil);
  mkat("GLloadMatrix", SUBR1, gl_matrix_load,0, nil);
  mkat("GLmultMatrix", SUBR1, gl_matrix_mult,0, nil);
  mkat("GLgetMatrixValue", SUBR3, gl_get_matrix_value,0, nil);
  mkat("GLsetMatrixValue", NSUBR, gl_set_matrix_value,0, nil);
  mkat("GLfreeMatrix", SUBR1, gl_free_matrix,0, nil);
  mkat("GLgetImageMatrix", SUBR1, get_image_transformation,0, nil);
  /* contrôle de l'environnement Open GL */
  mkat("GLget", SUBR1, gl_get_valeur,0, nil);
  mkat("GLenable", SUBR1, gl_permet,0, nil);
  mkat("GLisenable", SUBR1, gl_is_enable,0, nil);
  mkat("GLdisable", SUBR1, gl_disable,0, nil);
  mkat("GLflush",SUBR0,gl_flush,0,nil);
  /* gestion des attributs */
  mkat("GLpushAttrib", FSUBR, gl_pousse_attrib,0, nil);
  mkat("GLpopAttrib", SUBR0, gl_tire_attrib,0, nil);
  mkat("GLcopyAttrib", SUBR3, gl_copy_attrib,0, nil);
  /* gestion des listes */
  mkat("GLislist", SUBR1, gl_islist,0, nil);
  mkat("GLrootlist",SUBR2,gl_setrootlist,0,nil);
  mkat("GLgenlist", SUBR1,gl_genlist,0,nil);
  mkat("GLnewlist", SUBR1,gl_newlist,0,nil);
  mkat("GLendlist", SUBR0,gl_endlist,0,nil);
  mkat("GLdellist", SUBR1,gl_dellist,0,nil);
  mkat("GLcalllist",SUBR1, gl_calllist,0, nil) ;
  mkat("GLpushnewlist",SUBR2,gl_pushnewlist,0,nil);
  /* lumières, matériaux et textures */
  mkat("GLblend",SUBR2, gl_fonction_blend,0, nil);
  mkat("GLlight",SUBR3, gl_lumiere,0, nil);
  mkat("GLmodel",SUBR2, gl_model,0, nil);
  mkat("GLmaterial",SUBR3, gl_materiel,0, nil);
  /* couleur */
  mkat("GLcolor",NSUBR,gl_couleur,0,nil);
  mkat("GLColor",NSUBR,gl_Couleur,0,nil); 
  /* textures */
  mkat("GLtexTiff", SUBR1, gl_def_tiff_texture,0, nil);
  mkat("GLtexRGB", SUBR1, gl_def_rgb_texture,0, nil);
  mkat("GLtexData", SUBR3, gl_def_data_texture,0, nil);
  mkat("GLtexCoord", SUBR2, gl_texture_coord,0, nil);
  mkat("GLtexEnv", SUBR2, gl_texture_env,0, nil); 
  mkat("GLtexParameter", SUBR3, gl_texture_parameter,0, nil); 
  mkat("GLtexGen", SUBR3, gl_texture_gen,0, nil); 
  /* dessin de formes prédéfinies */
  /* contrôle du rendu ... pour cylindre et disk */
  mkat("GLUsetvalue",SUBR2, glu_setvalue,0,nil);
  mkat("GLUdelquad",SUBR1,glu_delquad,0,nil);
  mkat("GLcylindre", NSUBR,gl_cylinder,0,nil);
  mkat("GLdisk",NSUBR,gl_disk,0,nil);
  /* autres */
  mkat("GLsphereWire",SUBR1,gl_sphere_wire,0,nil);
  mkat("GLcubeWire",SUBR1,gl_cube_wire,0,nil);
  mkat("GLboxWire",SUBR3,gl_box_wire,0,nil);
  mkat("GLtorusWire",SUBR2,gl_torus_wire,0,nil);
  mkat("GLcylinderWire",SUBR2,gl_cylinder_wire,0,nil);
  mkat("GLicosahedraWire",SUBR1,gl_icosahedra_wire,0,nil);
  mkat("GLoctahedraWire",SUBR1,gl_octahedra_wire,0,nil);
  mkat("GLtetrahedraWire",SUBR1,gl_tetrahedra_wire,0,nil);
  mkat("GLdodecahedraWire",SUBR1,gl_dodecahedra_wire,0,nil);
  mkat("GLconeWire",SUBR2,gl_cone_wire,0,nil);
  mkat("GLsphereSolid",SUBR1,gl_sphere_solid,0,nil);
  mkat("GLcubeSolid",SUBR1,gl_cube_solid,0,nil);
  mkat("GLboxSolid",SUBR3,gl_box_solid,0,nil);
  mkat("GLtorusSolid",SUBR2,gl_torus_solid,0,nil);
  mkat("GLcylinderSolid",SUBR2,gl_cylinder_solid,0,nil);
  mkat("GLicosahedraSolid",SUBR1,gl_icosahedra_solid,0,nil);
  mkat("GLoctahedraSolid",SUBR1,gl_octahedra_solid,0,nil);
  mkat("GLtetrahedraSolid",SUBR1,gl_tetrahedra_solid,0,nil);
  mkat("GLdodecahedraSolid",SUBR1,gl_dodecahedra_solid,0,nil);
  mkat("GLconeSolid",SUBR2,gl_cone_solid,0,nil);
  mkat("GLsphere",NSUBR,gl_sphere,0,nil);
  mkat("GLrect",NSUBR,gl_rect,0,nil);
  mkat("GLrectf",NSUBR,gl_rectf,0,nil);
  mkat("GLpoly",NSUBR,gl_polyg,0,nil);
  mkat("GLellipse",NSUBR,gl_circle,0,nil);
  /* formes par GLbgn/GLend */
  mkat("GLbgn",SUBR1,gl_bgn,0,nil);
  mkat("GLend",SUBR1,gl_fini,0,nil);
  mkat("GLlineWidth",SUBR1,gl_epaisseur,0,nil);
  mkat("GLlineStipple",SUBR2,gl_style,0, nil);
  mkat("GLvertex",NSUBR,gl_vertex,0,nil);
  mkat("GLnormal",NSUBR,gl_normal,0,nil);
  mkat("GLpolymode",SUBR2,gl_polym,0,nil);
  /* nurbs et autres courbres */
  mkat("GLsetnurbs",SUBR2,gl_set_nurbs,0,nil);
  mkat("GLcurve",SUBR3,gl_nurbs_curve,0,nil);
  mkat("GLsurface",SUBR3,gl_nurbs_surface,0,nil);
  mkat("GLswaptmesh",SUBR0,gl_swap_tmesh,0,nil);
  mkat("GLmesh2", NSUBR, gl_bezier_mesh_2,0, nil);
  mkat("GLgrid2", NSUBR, gl_bezier_grid_2,0, nil);
  mkat("GLeval2", SUBR2, gl_bezier_eval_2,0, nil);
  mkat("GLmap2", NSUBR, gl_bezier_map_2,0, nil);
  mkat("GLmesh1", SUBR3, gl_bezier_mesh_1,0, nil);
  mkat("GLgrid1", SUBR3, gl_bezier_grid_1,0, nil);
  mkat("GLeval1", SUBR1, gl_bezier_eval_1,0, nil);
  mkat("GLmap1", NSUBR, gl_bezier_map_1,0, nil);
  /* strings via Hershey */
  mkat("GLfont",SUBR1,glhfont,0,nil);
  mkat("GLfontpath",SUBR1,glhpath,0,nil);
  mkat("GLchar",NSUBR,glhdrawchar,0,nil);
  mkat("GLstr",NSUBR,glhstr,0,nil);
  mkat("GLtextsize",SUBR3,glhtextsize,0,nil);
  mkat("GLboxtext",NSUBR,glhboxtext,0,nil);
  mkat("GLboxfit",SUBR3,glhboxfit,0,nil);
  mkat("GLtextangl",SUBR1,glhtextangl,0,nil);
  mkat("GLfixedwidth",SUBR1,glhfixedwidth,0,nil);
  mkat("GLcentertext",SUBR1,glhcentertext,0,nil);
  mkat("GLrigthjustify",SUBR1,glhrigthjustify,0,nil);
  mkat("GLleftjustify",SUBR1,glhleftjustify,0,nil);
  mkat("GLstring",NSUBR,gl_string,0,nil);
  /* racine de l'interface de navigation dans une image GL */
  mkat("GLdrive", 0,0,0, nil);
  /* autres fonctions locales (non liées à Open GL) */
  mkat("GLgetXYangulaire",NSUBR,gl_get_circle_pos,0,nil);
  mkat("GLgetXYZ",NSUBR,gl_get_real_position,0,nil);


#endif /* GL */
  
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


