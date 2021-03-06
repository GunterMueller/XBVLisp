/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	vlisp.h
 *	Patrick Greussay	23 Oct 82
 *	Harald Wertz          Decembre 82
 */

#ifndef vlisp_h
#define vlisp_h

#include <stdio.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <setjmp.h>
jmp_buf savej;

#ifdef INT64
typedef long Int; /* on doit avoir sizeof(Int) = sizeof(Int *) */
typedef unsigned long UInt;
#else
typedef int Int;
typedef unsigned int UInt;
#endif

Int onintr();

/* auel sorte de police de caract�re est manipul�e par X */
#ifdef X_USE_LATIN1
typedef unsigned char CHAR;
#else
typedef char CHAR;
#endif

  /* Pour faire portable avec UNIX Systeme V:
   * This trick is used to distinguish between SYSV and V7 systems.
   * We assume that L_ctermid is only defined in stdio.h in SYSV
   * systems, but not in V7 or Berkeley UNIX.
   */
#ifdef L_ctermid
# define SYSV
#endif

#define bVlispFonc Int

/* #define NO_VFLO  * et pas de flotants */

#ifndef NO_VFLO
Int onflo();
#include <math.h>
#endif

#undef DEBCAR
#ifdef DEBCAR 
#include "assert.h"
#define car(x)	(assert(!isnum(*x) && ((UInt)*x & 4 )),Car(x))
#define cdr(x)	(assert(!isnum(*(x+1)) && ((UInt)*(x+1) & 4 )),Cdr(x))
#endif

/* bits de tags */
/* Pour PROLOG */
#define VARIABLE 	01
#define CLAUSE		02
#define EVALUABLE	04

#ifndef NO_VFLO
double atof(),sqrt(),sin(),cos();
#endif
/* pour les definitions dependant du site */
/* 
#define KILLCHAR '' 
#define SCROLL	{++LIGNE;}
*/

#define SAVFIL		"session.vlisp"
#define GCSTRFIL	"/tmp/gXXXXXX"

/*   pour forcer a multiple de 4 (16 bits) ou 8 (32 bits) ou 16 (64 bits)
 *   (2*sizeof(Int)-1)
 */

#define ALIG	(2*sizeof(Int)-1)
#define ANNOT	4
#define UNANNOT(ptr) (Int*)((UInt)(ptr) & (UInt)ANNOT)
#define DOANNOT(ptr) (Int*)((UInt)(ptr) | (UInt)ANNOT)

/* pour les arguments de vlisp */
Int vlargc ;
char **vlargv ;

#define aligner(x)	x +=ALIG;x &= ~ALIG
  
#define push(x)	        *++p = (Int) x
#ifdef STDC_HEADERS
#define pop(x)		x = (Int) *(p--)
#define xtopst(x,via,y)	{ y = (Int *)(*p); *p = (Int)x; x = (Int *)y; }
#else
#define pop(x)		x = (*p--)
#define xtopst(x,via,y)	{ y = (Int *)(*p); *p = (Int)x; x = (Int *)y; }
#endif
#define pop_cast(x,t)   x = t (*p--)
#define rec(to,cont)		{pc = (Int(*)())to; *++p=(UInt)cont;}
#define derec			{pc= (Int(*)())*p--;return 0;}
#define pushf(f)		*++p=(UInt)f

  
#define popf			pc = (Int(*)())*p--
#define isnotlist(x)		((Int)x < (Int)blst || (Int)x > (Int)hlst)
#define islist(x)		((Int)x >= (Int)blst && (Int)x <= (Int)hlst)
#define isnil(x)		(x==nil)
#define isnotnil(x)		(x!=nil)

/* v�rification des caract�res : le > 200 est pour la prise en compte des accents */
#define is_imprimable(c) (isprint(c)||((unsigned)c)>200)
#define is_lettre(c)   (is_imprimable(c)&& !isspace(c)&&c!='\n'&&c!='('&&c!=')')

#ifndef NO_VFLO
/* modification de la definition des nbrs */
#define isnum(x) ((Int)(x) <= (Int)hflo) 
#else
/* et des entiers */
#define isnum(x) ((Int)(x) <= (Int)hnum)
#endif

/* pour differencier les entiers des flotants */
#define isint(x) ((Int)x <= (Int)hnum)
#define isstr(x) (((Int)bstr <= (Int)x && (Int)x < (Int)hstr) || ( (Int)bsysstr < (Int)x && (Int)x < (Int)hsysstr))
/* on cast les tests a des operations sur des entier */
/* test chaine utilisateur */
#define isustr(x)	        ((Int)bstr <= (Int)x && (Int)x < (Int)hstr)
#define isatom(x)		((Int)bat <= (Int)x  && (Int)x <= (Int)hat)
#define isnotatom(x)		((Int)x < (Int)bat  ||  (Int)x > (Int)hat)

#ifndef NO_VFLO
#define isflo(x)		((Int)bflo <= (Int)x && (Int)x <= (Int)hflo)
#define isnotflo(x)		((Int)x < (Int)bflo || (Int)x > (Int)hflo)
#endif

#include "fons.h"

#define SIZAT (sizeof(struct atome))
#define	SIZNUM	(2*sizeof(Int))
#define	SIZLST	(2*sizeof(Int))
#define	SIZPIL	sizeof(Int)

#ifndef NO_VFLO
#define SIZFLO	(sizeof(float)+sizeof(float))
#endif

/* d�finition de la taille allou� pour chaque type, le total fait 5Mb en 32bits et 9.43Mb en 64bits */
#define TNUM	(111956*SIZNUM) 
#define TSTR	 400000         /* old 70000 */
#define TAT	( 10000*SIZAT)
#define TLST	(500000*SIZLST) /* old 50000 */
#define TFLO	( 30000*SIZFLO) /* old 3000 */
#define TPIL	( 15000*SIZPIL)  /* taille de la pile... */

/* bords de zones */

#ifndef NO_VFLO
CHAR *bflo,*hflo, *busrflo;  /* usr flo car pour l'implementation de GL les */
#endif                       /* flotants sont beaucoups utilise et un certain nombre des flo */
                             /* est reserve aux i/o  (D.P) */

/* limites de zones memoire */    

CHAR *bnum,*hnum,*bstr,*hstr,*bat,*hat,*blst,*hlst,*bpil,*hpil, *hdsysstr;

/* accus */
Int *a1,*a2,*a3,*a4,*a5,*A1;
Int *form;
CHAR *PCH;

/* pour l'expression d'appel de session relancee */
Int *session;

/* accus numeriques */
Int na1,na2;

#ifndef NO_VFLO
double fa1,fa2; float fx,fy,fz;
#endif

/* def d'atome */
struct atome {
  Int *cval; /* la valeur de l'atome */
  Int *pval; /* liste de proprietees */
  Int *fval; /* valeur fonctionnelle */
  Int *ofval;/* old-valeur fonctionnelle */
  char ftyp; /* type d'atome */
  char oftyp;/* old-type */
  char tags; /* pour prolog */
  char err;  /* traitement des erreurs d'arguments (D.P) */
#ifdef INT64
  char unused[4]; /* pour l'allignement sur 64 bits */
#endif
  Int *ival; /* les annotations */
  Int *pakage; /* il faudrait le changer par un *char pakage
		   et ne garder la que le nom du package pour accelerer
		   la recherche des atomes, A Voir */
  Int *obj;    /* pour la couche objet, objet de mes preocupations */
  /* il existe une duplication d'information, etudier comment optimiser */
  struct atome *nexa;     /* liste d'atomes du systeme */
  struct atome *next_at ; /* new nexa, pour faire de hash-coding, Ernesto */
  Int *oval;
  Int *pn;
} ;

typedef struct atome ATOME, *ATOMEPTR;

struct macro_fval { 
  Int *fval;
  char ftyp;
  char unused[sizeof(Int *) - 1]; /* pour l'allignement */
} ;

Int *atmc ; /* atome utilise pour l'execution des macro caracteres (D.P) */

/* ptr de pile et pc et link de blocs */
Int *p, *savp;
Int (*pc)();
Int *pbind;
/* ptr nb libre */
Int *freenb;
/* ptr liste libre */
#ifndef NO_VFLO
Int *freeflo;			/* ptr flo libre */
Int *atPI; /* atome pour PI */
#endif

Int *freel;
/* ptr atome libre et autres */
Int *freeat,
  *derat;	/* adr der atome connu */

/* ptr octet libre en zone string */
CHAR *ppn;

/* adresses d'atomes utiles */
Int *nil,*undef,*atlst,*atcons,*atlist,*atmcons,*lambda,*quote,*t;
Int *ateof,*it,*atlmarg,*atrmarg,*atread,*atprint,*ateval;
Int *atcls,*atrandom,*atprlen,*atSELF,*attop;
Int *atsplice,*atunquo,*atqua,*atubv,*atubf,*attyp;
Int *atindvar,*atindfun,*aterpil, *atertyp;
Int *atuser,*atprompt;
Int *atclosure,*atprotect,*atlock;
Int *aterror,*atubv,*atubf,*atframe,*atself,*at_exit,*atescape,*atpil;
Int *ateol, *atclock, *atintr, *lfread, *lfprint, *atANNOT;

Int *atprolog,*atperl,*atdol,*atassert;

CHAR *bsysstr,*hsysstr,*Indvar,*Indfun,*Errtyp,*Errlec,*Errpil,*Errself,*Errexit;

/* pour moi (= Harald) */
Int *atANTISL, *atLINES, *atCOLS/*, *atLISPSTD */, *atWIDTH, *atHEIGHT;
Int *atcareful,*atlocalvar,*atglobalvar,*atall_functions,*atall_clauses,*atexample, *atcrewid;
Int *atprog,*atattach,*atnreverse,*atnconc,*atplaca,*atset,*atincr,*atnth;
Int *atdecr,*atplacd,*atplacb,*atnewl,*atnextl,*atsetq,*atlet,*atde;
Int *atcond,*atsele,*atentree,*atsortie,*atminus,*atcomm,*atdone,*atundo;
Int *atgo,*atapply,*atONINTR,*atSTEP,*atstep,*atpackage,*atEOF;
Int *atprogn,*atpcomm,*atall,*atbvlisp, *atgc;
Int wherefrom;

Int *atobase,*atibase;	/* pour la lecture et l'impression de nombres */

Int EXTREAD;		/* pour l'analyse */

Int DML;		/* pour les dml */

/* pour la trace et d'autre choses similaire */
Int TRACE;

/* pour implode */
Int *impl,		/* la liste de caracteres ou de chiffres */
  isimpl;		/* =1 : on est dans implode, =0 : lecture normale */

/* pour explode */
Int *vlexpl,		/* la liste qui contiendra l'explosion */
  isvlexpl;		/* =1 : on est dans explode, =0 : sortie normale */

/* pour gensym */
Int ngsym;

/* auxiliaires */
Int *int_p1;
Int aux1;
CHAR *pchar,*pchar2;


extern CHAR tabch[];
extern struct macro_fval tabfval[];

/* pour les entrees */
Int rdprd;			/* compteur niveau parentheses */
Int MARKED;			/* pour l'editeur */

#if 0
CHAR BUFIN[MAX_COLS];		/* pour moi ... (Harald) */
CHAR YANK[129];			/* encore pour le meme .. */
#endif

#define MAX_COLS 512
CHAR *inbpt,bufin[MAX_COLS];	/* buffer entree terminal et son ptr */
Int ringur;			/* le caractere d'avance */
Int tlu;			/* type caractere ou objet lu */
CHAR *oppn;			/* base de pname lu courant */
Int lpn,			/* long pname lu (0 inclus) */
       sign;
     
     /* pour le toplevel */
CHAR *btop,buftop[MAX_COLS];
Int optop;			/* pour status toplevel */
     
     /* pour les sorties */
CHAR *titel;
Int LIGNE;			/* compteur de lignes */
Int DEBWIN;			/* debut fenetre interaction */
CHAR bufou[MAX_COLS];		/* buffer sortie terminal */
Int oubpt,			/* son index */
  lcou,			/* nb de places libres restantes ds ligne */
  prnxt,		/* pour place 1 esp ou non avant impression */
  sava1, soubpt,	/* pour sauver ds prato/pvch */
  opti,			/* status prInt */
  prcln;		/* pour compter le nb d'elts imprimes */
     

     /* pour le gc */
Int ngc,		/* nb de gc's */
  strngc,		/* nb de gc's de strings */
  pgc;			/* =1 imprimer, =0 non */
     
     /* pour les erreurs */
CHAR *errst;
     
     /* pour les files --- provisoire */
Int dski;		/* =0 pour terminal, =1 pour files */
FILE *fdes;		/* file-descriptor */
Int *libpil;		/* ou lib stocke son pointeur de pile */
     
     /* types de blocs */
#define MRKEV	(-1)
#define LAMBDA	0
#define WHESC	1
#define CLOSURE	2
#define PROTECT 3
#define LOCK	4
     
     /* types de fonctions */
/* system */
#define SUBR0	1
#define SUBR1	2
#define SUBR2	3
#define SUBR3	4
#define NSUBR	5
#define FSUBR	6
/* utilisateur */
#define EXPR	7
#define FEXPR	8
#define MACRO	9
#define ESCAPE	10
#define MACOUT	11
     
/* traitement des erreurs de typage */
#define ERR_NONS   1 /* nombres et chaines interdits */
#define ERR_NONL   2 /* nombres et listes interdits */
#define ERR_NONUM  3 /* nombres interdits */
#define ERR_NOSTR  4 /* chaines interdites */
#define ERR_ATOME  5 /* atome obligatoire */
#define ERR_NOZERO 6 /* z�ro valeur impossible */
#define ERR_ARG1 (1 << 4) /* test sur le premier argument */
#define ERR_ARG2 (2 << 4) /*             second           */
#define ERR_ARG3 (3 << 4) /*             troisi�me        */
#define ERR_ARGN (4 << 4) /*          tous les arguments (NSUBR...) */

#define ATOME_FTYP(at)  ((struct atome *)at)->ftyp
#define ATOME_ERR(at)   ((struct atome *)at)->err
#define ATOME_ERR_ARG(at) (ATOME_ERR(at) & 0xF0)
#define ATOME_ERR_NUM(at) (ATOME_ERR(at) & 0x0F)
     
     /* types des caracteres */
#define BCOM	1
#define ECOM	2
#define QCH	3
     
#define LPAR	4
#define RPAR	5
#define LCRO	6
#define RCRO	7
#define DOT	8
     
#define SEP	9
#define MAC	10
#define STR	11
#define NOR	12
#define SHAR	13
#define AT	14
#define MACL	15
#define LANNOT	16
#define RANNOT	17
#define HINIB   18

     /* pour objlis() dans in.c */
#define AT	14
/* #define ANTISL	17 Ernesto: var. non utilise, remplace par atANTISL ? */

/* type des donnee rendus par les atomes */
#define VIDE     0
#define STRING    1
#define NAME      2
#define INTEGER   3
#define FLOAT     4
#define NUMBER    5
#define INCONNU   6
#if 0
/* les donnees Xt */
#define WIDGETTE  7
#define RESSOURCE 8
#define ERROR     -1
#endif

#define LISTE     9   /* type LISP */

/* pour faire de Hash-coding */

#define  NHASH_PRIME  (1<<10) /* DOIT ETRE UNE PUISSANCE DE DEUX */

#define  TABSTOPS      8
#define RETOUR_LIGNE -1
#define TABULATION   -2


/* pour extraire des objets lisp et des champs d'atomes */
/*------------------------------------------------------*/
/* extrait la chaine de caracteres de repressentation lisp de chaines */

#define STR_FROM_STRLSP(str)  ((CHAR *) (str + 2/2))

#define ADRPNAME(at)    (Int *)((struct atome *) at)->pn
#define STRPNAME(at)    (CHAR *)(ADRPNAME(at) ? (ADRPNAME(at) + 2/2) : NULL)

/* pour optimisation */

#define wst(str)             (Int) printf( "%s", (CHAR *)str)
#define creatom(str)         (Int *) creatom_hash((CHAR *)(str), -1)
#define cherchat(str)        (Int *) lookfor_atome_hash((CHAR *)((str)+sizeof(Int)), NULL)
#define lookfor_atome(name)  (struct atome *) lookfor_atome_hash((CHAR *)(name), NULL)

extern Int my_Exec_Flag; /* provisoire ---------------------------
			    pour controler l'excution des fonctions,
			    Mecanisme deja prevu, mais j'oublie le
			    nom de la variable puisque avec la mienne
			    Katastrofe.*/

// Added by Druid
#include "proto.h"
#define CARS_MARGE_DROITE 1
#define CALLBACK_INPUT 9999

#endif
