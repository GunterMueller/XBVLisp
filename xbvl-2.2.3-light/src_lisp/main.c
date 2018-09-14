/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	main.c		pour vlisp
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 10 Fevrier 83
 */

#include <vlisp.h>

#include <dirent.h>
#include <unistd.h>

#include <xwin.h>
#include <vlisp_files.h>
#include <Xbvlisp.h>
#include <aide.h>
#include <xaide.h>

extern Int unefois;
				/* pour l'environnement et les arguments */
char **savenviron=0;
Int svlargc=(Int)1; char **svlargv=(Int)0;
CHAR *Aide_src = NULL, *Aide_index = NULL, *Aide_section=NULL;

char version[]="foobar";
extern char **environ;
extern char titre1[],titre2[],titre3[],titre4[],titre5[];

/* pour la reprise de session */
Int ree = (Int)-1;		/* pour le onintr */
struct stat sbuf;
/* char *sbrk();   Contredit la déclaration dans unistd.h    */

CHAR main_top_name[20];
#ifdef X_USE_LATIN1
/* prise en compte des accents */
CHAR tabch[256]={
#else
CHAR tabch[128]={
#endif
  0, 12, 12, 12,  12, 12, 12, 12,	/* nul  CA  CB  CC   CD  CE  CF  bel */
  9,  9,  9,  9,   9,  9, 12, 12,	/*  bs tab  lf  CK   CL  cr  CN  CO  */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  CP  CQ  CR  CS   CT  CU  CV  CW  */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  CX  CY  CZ esc   Cas C]  Cfl C_  */
  9, 12, 11, 13,  12, 12, 12, 10,	/* esp   !   "   #    $   %   &    ' */
  4,  5, 12, 12,  12, 12,  8, 12,	/*  (    )   *   +    ,   -   .   sl */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  0    1   2   3    4   5   6    7 */
  12, 12, 12, 1,  12, 12, 12, 12,	/*  8    9   :   ;    <   =   >    ? */
  12, 12, 12, 12,  12, 12, 12, 12,	/* at    A   B   C    D   E   F    G */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  H    I   J   K    L   M   N    O */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  P    Q   R   S    T   U   V    W */
  12, 12, 12,  6,  18,  7, 12, 12,	/*  X    Y   Z   [  antsl ]  vfl   _ */
  10, 12, 12, 12,  12, 12, 12, 12,	/*  `    a   b   c    d   e   f    g */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  h    i   j   k    l   m   n    o */
  12, 12, 12, 12,  12, 12, 12, 12,	/*  p    q   r   s    t   u   v    w */
  12, 12, 12, 16,  12, 17, 12, 0	/*  x    y   z   {   |   }   ~   del */
#ifdef X_USE_LATIN1
  , 0,  0,  0,  0,   0,  0,  0,  0,
  0,  0,  0,  0,   0,  0,  0,  0,
  0,  0,  0,  0,   0,  0,  0,  0,
  0,  0,  0,  0,   0,  0,  0,  0,
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,     
  12, 12, 12, 12,  12, 12, 12, 12,	
  12, 12, 12, 12,  12, 12, 12, 12,
  12, 12, 12, 12,  12, 12, 12, 12
#endif
};

struct macro_fval tabfval[128];

#ifdef SFI_TRACE

static Int sfi_trace_on = 0;
FILE *trace_out;

#endif

main(argc,argv)
     Int argc;
     char **argv;
{
  extern Int loop();

  vlargc=argc; vlargv=argv;

#ifdef SFI_TRACE
  if(!(trace_out = fopen("xbvl.trace.out","w"))) {
    fprintf(stderr, "Cant open trace\n");
    exit(0);
  }
#endif

  tryrestore(argc,argv);	/* c'est peut etre une session sauvee */
  
  p = (Int *)1000;
  titel= &titre5[0];
  Aide_src = INFILE;   /* aide system */
  Aide_index = DFILE;
  Aide_section = SFILE;
  
  indinit();	/* dans la file indinit.c */
  
  strcpy(main_top_name, "xbvl\0");
  X_init();     /* initialisation de toolkits, Widgets, Interaction...*/

#if (OPEN_GL | MESA_GL)
  glLibInit();
#endif

#ifdef DEBUG
  printf("bnum=%u,hnum=%u,bstr=%u,hstr=%u,bat=%u,hat=%u\n",
	 (Int)bnum,(Int)hnum,(Int)bstr,(Int)hstr,(Int)bat,(Int)hat);
  printf("blst=%u,hlst=%u,bpil=%u,hpil=%u\n",
	 (Int)blst,(Int)hlst,(Int)bpil,(Int)hpil);
  printf("freenb=%u,freel=%u,freeat=%u,p=%u,ppn=%u\n",
	 (Int)freenb,(Int)freel,(Int)freeat,(Int)p,(Int)ppn);
#endif
  pc=debut;gci(); 
#ifndef NO_VFLO
  *atPI = (Int)craflo(M_PI);
#endif
  loop();
#ifdef SFI_TRACE
  fclose(trace_out);
#endif
}

#ifdef SFI_TRACE

sfi_trace() /* (sfi-trace) */
{
  sfi_trace_on = (sfi_trace_on == 0);
  derec;
}

sfi_mark() /* (sfi-mark) */
{
  fprintf(trace_out, "SFI\n");
  derec;
}

#endif

extern Int debut1(), reinit(), iniitt();

Int debut()/* a la 1ere entree */
{
  p = (Int *)bpil; *p = 0; pbind = 0;wherefrom = 0;
  reinit();
  /* attention! hack a changer dependant du fenetre active!!!! FENETRE */
  pc=debut1;
}
Int debut1()
{
  dski=(Int)1; fdes=fopen("config.ini","r");
  rec(libi,debut_1);}

Int debut_11(){
  extern Int LINES, COLS, WIDTH, HEIGHT;

  reinit();
  /* creation des atomes LINES et COLS */
  *atWIDTH = (Int)cranb(WIDTH);
  *atHEIGHT = (Int)cranb(HEIGHT);
  *atLINES = (Int)cranb(LINES);
  *atCOLS = (Int)cranb(COLS);
  *atrmarg = getXbvlColonnes(Xbvlisp_courr) - 2;
		
  dski=(Int)2; fdes = fopenFile("BVLISP_SYS", BVLISPSYS);
  rec(libi,debut_2);
}

Int debut_1(){rec(titre,debut_11);}

Int debut_3()
{
  char b[80];
  reinit();
#if 0
/* Ernesto: on charge rien pour l'instant, 
   la sauvegarde et restitution de l'environnemen a refaire 
  if(vlargc>1){
  dski=8; aux1=open(".bvlisp-der",0);
    if(aux1>=0)
      {
	read(aux1,b,80); *(index(b,' '))='\0'; /* ATTENTION au blanc */
	wst("On charge: "); wst(b); wst("\n");
	close(aux1); fdes=fopen(b,"r");
	unefois=0;
      }
    rec(libi,reent);}
  else pc=reent;
*/    
#endif
 pc = reent;
}

Int debut_22()
{
  reinit();
  dski=3; fdes=fopen("vlisp.ini","r");
  rec(libi,debut_3);
}

Int debut_2()
{
  reinit();
  dski=6; fdes=fopen("bvlisp.ini","r");
  rec(libi,debut_22);
}

Int onintr1()
{
  pop(prcln);
  pop(opti);
  pop(soubpt);
  pop(sava1);
  pop(prnxt);
  pop(lcou);
  pop_cast(vlexpl, (Int *));
  pop(isvlexpl);
#ifndef NO_VFLO
  pop(fz);
  pop(fy);
  pop(fx);
  pop(fa2);
  pop(fa1);
#endif
  pop_cast(int_p1, (Int *));
  pop_cast(savp, (Int *));
  pop_cast(form, (Int *));
  pop(na2);pop(na1);
  pop_cast(pchar2, (CHAR *));pop_cast(pchar, (CHAR *)); pop(aux1);
  pop_cast(a5,(Int *));pop_cast(a4,(Int *));pop_cast(a3,(Int *));pop_cast(a2,(Int *));pop_cast(a1,(Int *));
  pop_cast(pc, (Int (*)())); pop_cast(p, (Int *));
  return;
}

Int iniott()/* initialisations sorties terminal */
{
  for(oubpt=0; oubpt < (MAX_COLS - 1) ; oubpt++)bufou[oubpt]=' '; 
  oubpt= *atlmarg;
  /* la limite de la ligne cree par bVLISP */
  lcou = (Int)(getXbvlColonnes(Xbvlisp_courr) -  CARS_MARGE_DROITE);
  vlexpl=(Int *)(isvlexpl=0);
}
Int reent(){/* pour reentrer en reinitialisations au toplevel */
  /* imprimer le signe de reentree dans l'athmosphere */

  /* init pile */
  TRACE=DML=0;
  p=(Int *)bpil; *p=0; pbind=0;
  /*  setjmp(savej); */
  wst(".Ree\n");
		
  *atprompt=(Int)atlst;
  *atlmarg=0;
/* POUR DEBUGER C'EST BON--mais seulement pou debug-----*
  signal(SIGBUS,onintr);
  signal(SIGIOT,onintr);
  signal(SIGSYS,onintr);
  signal(SIGBUS,onintr);
  signal(SIGSEGV,onintr);
/* -----------------------------------------------------*/
  signal(SIGUSR1,(void (*)(int))onintr);
  signal(SIGINT,(void (*)(int))onintr);
  signal(SIGQUIT,(void (*)(int))onintr);

  reinit();
  pc=toplv;
}	/* pour l'auto empilement de toplv_2 */

  /* 
   * VRAIE FONCTION C, appelee par : reinit(); 
   */
Int reinit()
{	
  /* init entrees */
  iniitt();
  
  /* init toplevel */
  btop=buftop;
  optop=0;
  
  /* init sorties */
  /* CA CHAMGE QQCH ? ERNESTO */
  iniott();
  opti=0;
  
  /* init files --- provisoire */
  dski=0;
}

/* 
  FORME ACCEPTEE PAR ZCC
   loop(){for(;;)pc();}
*/

#if 0
static Int re_start = False;
#endif

Int loop()
{
  register Int xx, yy = 1;
  for(;;)
    {
      xx=((Int)pc)&(~01); 
      pc = (Int(*)())xx; 
#ifdef SFI_TRACE
      if(sfi_trace_on) {
	fprintf(trace_out, "%u\n", (UInt) ((Int) pc - (Int) main));
      }
#endif
      (*pc)();
#if 0   
      /* change par pushf(rpt) dans save_jump */
      if (re_start == True){
	/* re_start = False; */
	rpt();
	/*longjmp(savej,0);*/
      }
#endif
      /* de temps a autre voir s'il y a
	 des evenements a traiter  */
      if(yy++ & 2048) { XteLoop(); yy = 0; }  
    }
}

Int toplv()/* (toplevel)  subr 0 */
{
  pushf(toplv_1); rec(readu,eval); 
}

Int toplv_1(){
  bufou[oubpt++]= '='; --lcou; MARKED=0;
  *(it) = (Int)a1;
  pushf(toplv_2); rec(prini,terpri);
}

Int toplv_2()/* meme si toplevel est redefini on repasse la a chaque boucle */
{
/*  pushf(toplv_2); pushf(apply); push(attop); pushf(popa1); !!!!!!!!!! */
  a1=attop; pc=apply;
}

Int *cons(x,y)
     Int *x,*y;
{
  if(!freel){push(x); push(y); gci(); p-=2;} 
  *(freel)=(Int)x; 
  x=freel; 
  freel=cdr(freel); 
  *(x+1)=(Int)y;
  /*	*CDR(x)=y; */
  return(x);
}

Int *ncons(x)
     Int *x;
{
  return(cons(x,nil));
}

Int *cranb(n)
Int n;
{
  Int *x;
  
  if(n>=0 && n<(Int)bnum)return((Int *)n);
  if(!freenb)gci();
  x=freenb; freenb= (Int *)*(freenb); *(x+1)=0; *x=n;
#ifdef DEBUGGC
  printf("cranb: je cree une cell d'adresse %u pour le nb %d\n",(Int)x,n);
#endif
  return(x);
}

/*
 * interpretation de l'entier x 
 */

Int valnb(x) 
     Int *x;
{
  return( (((Int)x < (Int)bnum || (Int)x > (Int)hnum) && (Int)x < (Int)blst) ? (Int)x : *x );
}

Int iniitt()/* initialisations entrees terminal */
{
  rdprd=0;MARKED=0;
  impl=(Int *)(isimpl=0);
  inbpt=bufin; *bufin=0; ringur=0;
}

/* iniott etait ici, a mieux reorganiser la communication
   entre bVLISP et X */

Int sessp()/* (session e)  subr 1 */
{
  wst ( "A refaire.\n" );
  derec;
#if 0
  Int savef;
  
  session=a1;	/* on sauve l'expression de relance */
  ree = -1;	/* pour re-sauver dans la session sauvee */
  unlink(SAVFIL);
  savef=creat(SAVFIL,0666);
  write(savef,version,(unsigned int)(sbrk(0)-version));
  close(savef);
  wst("\nSession sauvee sous le nom de : "); wst(SAVFIL); wst("\n");
  stop();
#endif
}

Int stop()/* (stop)  subr 0 */
{
  exit(0);
}


Int loop11()
{
  register Int xx;
  for(;;)
    {
      xx=(Int)pc; xx&=~01; pc = (Int(*)())xx; (*pc)();
    }
}
/* pour catcher les DEL */
Int onintr(sig)
     Int sig; 
{
  if(sig!=SIGQUIT && sig!=SIGINT && sig!=SIGUSR1){
    wst("internal error-trap!\n");RPT();
  }
  if(((struct atome *)atONINTR)->fval != 0 && sig != SIGQUIT){ 
    /* pour trapper par l'utilisateur */
    signal(sig,(void (*)(int))onintr);
    push(p);
    push(pc);push(a1);push(a2);push(a3);push(a4);push(a5);
    push(aux1);push(pchar);push(pchar2);
    push(na1);push(na2);
    push(form);push(savp);push(int_p1);
#ifndef NO_VFLO
    push(fa1);push(fa2);push(fx);push(fy);push(fz);
#endif
    push(isvlexpl);push(vlexpl);push(lcou);push(prnxt); 
    push(sava1);push(soubpt);push(opti);push(prcln);
    if(p>(Int *)hpil)
      {pc=RPT;return;}
    a1=cons(lambda,((struct atome *)atONINTR)->fval);
    a1=ncons(a1);
    pushf(onintr1);push(eval);pc=terpri;
    loop11();
  }
  /*	else if(sig!=3 && sig!=2) 
	{pushf(RPT);pushf(terpri);pc=prini;loop();} */
  else RPT();
}
/* pour arreter l'interpreteur par une action de l'utilisateur
   SIGUSR1 est genere par l'utilisateur */
onintr_action(sig)
     Int sig; 
{
  if(sig!=SIGUSR1) return;
  if(((struct atome *)atONINTR)->fval != 0 && sig != SIGQUIT){ 
    /* pour trapper par l'utilisateur */
    signal(SIGINT,(void (*)(int))onintr);
    push(p);
    push(pc);push(a1);push(a2);push(a3);push(a4);push(a5);
    push(aux1);push(pchar);push(pchar2);
    push(na1);push(na2);
    push(form);push(savp);push(int_p1);
#ifndef NO_VFLO
    push(fa1);push(fa2);push(fx);push(fy);push(fz);
#endif
    push(isvlexpl);push(vlexpl);push(lcou);push(prnxt); 
    push(sava1);push(soubpt);push(opti);push(prcln);
    if(p>(Int *)hpil)
      {pc=RPT;return;}
    a1=cons(lambda,((struct atome *)atONINTR)->fval);
    a1=ncons(a1);
    pushf(onintr1);push(eval);pc=terpri;
    loop11();
  }
  /*	else if(sig!=3 && sig!=2) 
	{pushf(RPT);pushf(terpri);pc=prini;loop();} */
  else RPT();
}

#if 0
Int RPT(){ 
  Int reponse;

  XSync(display, False);
  XteLoop();
  faireConfirmation(interface_courr, &reponse, "Sortir de bvlisp (O/N) ?", 1); 
  if(reponse == True){
    stop();
  }
  else longjmp(savej,0); 

}
#endif

#if 0
Int rpt()
{
    longjmp(savej,0);
  if(re_start == True){
    re_start = False;
    /* ici etait le longjump */
  }
}
#endif
Int RPT(){ 
  char c = 0;
  while(c != 'c') {
    wst("\n");
    wst("(s)ortir de bvlisp ou (c)ontinuer ? (s/c):");
    c=xgetch1(NULL);
    xinsch(NULL,c, NULL);
    switch(c){
    case 's' :/*  stop(); */
      exit(0);
    case 'c' :
      /* longjmp(savej,0); */
      reent();
      break;
      default  : continue;
    }
  }
}
     
/*
 * routine d'acces a longjump
 * L'extructure de toolkits est endomagee
 * si le jump interromp la communication du serveur avec ses 
 * clients.
 */
/* ARGSUSED */
XtActionProc jump_start(w, event, params, num_params)
     Widget w;
     XKeyEvent *event;
     String * params;
     Cardinal * num_params ;
{
  printf("In jump_start\n");
  RPT();
#if 0
  pushf(rpt);
  /* seconde version */
  re_start  = True;
  /* premier version */
  /* un bug qui m'a enbete longtemps */
  synchron();
  kill(getpid(), SIGUSR1);
  *longjmp(savej,0); 
#endif
}

#if 0
Int synchron()
{

  XSync(display, False);
  XteLoop();
  sleep(5);
  XFlush(display);
  XteLoop();
}
#endif

/*
 * lance un programme et reccupere ses sortie
 * sur la fenetre X courrente.
 * Si on lancer un programme interactif il faut utiliser
 * (ssh "prog ags..")
 */
Int sh(){/* (sh e)  subr 1 */
  extern Int errno, get_off();
  FILE *tache;
  char buf[BUFSIZ];


  if(isnil(a1))derec;
  if((tache = popen(STR_FROM_STRLSP(a1), "r")) == NULL){
    char exec_err[128];
#ifdef ERRNO
    sprintf(exec_err,
	    "execution de %s impossible : %s\n",
	    a1 + (2/2), strerror(errno)
	    );
    wst(exec_err);
#else
    wst("Erreur pendant l'exécution\n");
#endif
  }
  alarm(5);  /* un timeout pour ne pas rester bloquee */
  signal(SIGALRM, (void (*)(int))get_off);
  while(fgets(buf, BUFSIZ -1, tache) != NULL){
    wst(buf);
    alarm(5);
  }
  alarm(0); /* canceler l'alarme */
  pclose(tache);
  derec;
}
/* Changement du repertoire courant (D.P. 04/96)
 */
Int change_repertoire()
{
  char *dir_name;
  if(!isstr(a1)) {a1 = nil; derec;}
  dir_name = (char *) (a1+(2/2));
  if(!chdir(dir_name)) a1 = t;
  else a1 = nil;
  derec;
}

/* Recuperation des fichier d'un repertoire (D.P, 11/94)
 */
Int ls()
{
  char *dir_name = (isstr(a1) ? (char *) (a1+(2/2)) : ".\0"), tmp_str[512];
  DIR *odir = opendir(dir_name);
  struct dirent *dir_element;
  struct stat elem_stat;
  Int *lst_head, *tmp;

  a1=nil;
  if(!odir) derec;
  while((dir_element = readdir(odir)) != NULL) {
    if((dir_element->d_name[0] == '.' && dir_element->d_name[1] != '.') ||
       dir_element->d_name[0] == '#' ||
       dir_element->d_name[strlen(dir_element->d_name)-1] == '~') continue;
    sprintf(tmp_str, "%s/%s\0", dir_name, dir_element->d_name);
    stat(tmp_str, &elem_stat);
    if(elem_stat.st_mode & S_IFDIR) sprintf(tmp_str, "%s/\0", dir_element->d_name);
    else sprintf(tmp_str, "%s\0", dir_element->d_name);
    newstr(tmp_str);
    if(a1 == nil) a1 = lst_head = cons((Int *)oppn, nil);
    else if(strcmp(STR_FROM_STRLSP(car(a1)), oppn+sizeof(Int)) >= 0) a1 = cons((Int *)oppn,a1);
    else {
      for(lst_head = a1; cdr(lst_head) != nil &&
	  strcmp(STR_FROM_STRLSP(car(cdr(lst_head))), oppn + sizeof(Int)) < 0; 
	  lst_head = cdr(lst_head));
      *CDR(lst_head) = (Int)cons((Int *)oppn,cdr(lst_head));
    }

  }
  closedir(odir);
  derec;
}
/*
 * retour au toplevel
 */
Int get_off()
{
  wst("utilisez (ssh \"prog args ...\")\n");
  derec;
}
Int ssh(){/* (sh e)  subr 1 */
  char command[128];
  if(isnil(a1))derec;
  sprintf(command,"xterm -e %s", a1+(2/2));
  system(command);
  derec;
}

Int config(){/* (config . l)  fsubr */
  /* (config nombres chaines atomes listes pile) */
#ifndef NO_VFLO
  /* Ernesto, grace au prototypage, on ajoute les flotants */
  Int flotants, nombres, chaines, atomes, listes, pile;


  if(init_oneINT( &flotants ) == ERROR){
    a1 = nil;
    derec;
  }
#else
  Int nombres,chaines,atomes,listes,pile;
#endif
  
  nombres=valnb(car(a1));
  chaines=valnb(car(((struct atome *)a1)->pval)); 
  a1=cdr(((struct atome *)a1)->pval);
  atomes=valnb(car(a1)); listes=valnb(car(((struct atome *)a1)->pval));
  a1=cdr(((struct atome *)a1)->pval);
  pile=valnb(car(a1));
  
  aligner(chaines);
#ifndef NO_VFLO
  init(flotants * SIZFLO, nombres*SIZNUM, chaines,atomes*SIZAT, listes*SIZLST, pile*SIZPIL);
#else
  init(nombres*SIZNUM,chaines,atomes*SIZAT,listes*SIZLST,pile*SIZPIL);
#endif
  if(fdes)close((Int)fdes); iniitt(); dski=0;
  p=(Int *)bpil; *p=0; pbind=0;
  reinit();
  pc=debut_1;
}


/*
 *	pour restaurer une session sauvee
 */

tryrestore(argc,argv)
     Int argc;
     char **argv;
{
  Int i; Int fd; struct stat sbuf2;
  
  fd=0;
  for(i=1; i<vlargc; i++)
    if(vlargv[i][0]=='-' && vlargv[i][1]=='s' && i < vlargc - 1)
      {
	fd=open(vlargv[i+1],0);
	if(fd<0){wst("Impossible.\n"); exit(0);}
      }
  if(fd==0)return;
  fstat(fd,&sbuf2);
  brk(version + sbuf2.st_size);
#ifdef DEBUGENV
  printf("haut=%u\n",(Int)(version+sbuf2.st_size));
#endif
  savenviron=environ;
  svlargc=vlargc; svlargv=vlargv;
  
  read(fd,version,(unsigned int)sbuf2.st_size);
  
  environ=savenviron;
  vlargc=svlargc; vlargv=svlargv;
  
  close(fd);
  
#if 0
  /* quelques initialisations peut-etre ... */
  initer();
#endif
  
  /* et on reprend la session! */
  p=(Int*)bpil; *p=0; pbind=0;
  /*  setjmp(savej); */
  ree++;
  signal(SIGINT,(void (*)(int))onintr);
  signal(SIGQUIT,(void (*)(int))onintr);
  /* attention! hack a changer dependant du fenetre active!!!! FENETRE */
  if(ree > 0){
    reinit();	/* A ELIMINER	  */
    /* *atrmarg=COLS; */
    pc=toplv_2;
  }
  else{
    a1=session;
    *atrmarg=80; 
    rec(eval,toplv_1);
  }
  loop();
}

#ifdef DEBUGENV
Int lisenv(){
  Int i; char **env;
  printf("adr imm environ=%u\n",(Int)&environ);
  printf("adr environ=%u\n",(Int)environ);
  env=environ;
	for(i=1; i<7; i++)
	  {
	    printf("adr env=%u\n",(Int)*env);
	    printf("%s\n",*env);
	    env++;
	  }
}
#endif

