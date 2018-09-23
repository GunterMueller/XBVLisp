
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	fil.c		traitement des files
 *			Patrick Greussay  27 Oct 82
 *			extensions: Harald Wertz 15 Avril 83
 *                                  Damien Ploix 22 Avril 93 
 *                                      (ajout de la conservation de nom de fichiers...)
 */

#include <vlisp.h>
#ifdef HAVE_DIRNAME
#include <libgen.h>
#endif


Int unefois=1;
Int tinclude[20],tinc= -1;
Int tlibpil[10],tlibp= -1;
static Int *tincname[10], *tlibname[15];
static Int op_dski[10], op_num=-1;
static FILE *op_fp[10];

#ifndef STDC_HEADERS
extern char *getenv();
#else
extern char *getenv(char *);
#endif

static CHAR *
ChkFileName(f_name)
CHAR *f_name;
{
  char *dir_name, *pch, have_dir;
  if(*f_name != '~') return f_name;
  aux1=(Int)ppn; aligner(aux1); ppn=(CHAR*)aux1;
  if(*(f_name+1) == '/') {
    if(dir_name = getenv("HOME")) 
      for(pch = ppn ; *pch++ = *dir_name++; );
    else return NULL;
  }
#ifdef HAVE_DIRNAME
  else {
    for(pch = f_name; pch && pch != '/'; pch++) ;
    if(pch) *pch = '\0', have_dir = 1;
    else have_dir = 0;
    dir_name = dirname(f_name);
    if(have_dir) *pch = '/';
    for(pch = ppn; *pch++ = *dir_name++ ; ) ;
  }
#endif
  ++f_name;
  for( ; *pch++ = *f_name++ ; );
  return ppn ;
}

initlibname() {
  newstr("config.ini\0"); tlibname[0] = (Int*)oppn;
  newstr("vlisp.sys\0"); tlibname[1] = (Int*)oppn;
  newstr("vlisp.ini\0"); tlibname[2] = (Int*)oppn;
  newstr("bvlisp.ini\0"); tlibname[3] = (Int*)oppn;
  newstr(".vlisp-der\0"); tlibname[4] = (Int*)oppn;
}

Int *
infname() {
  switch(dski){
  case 1: return tlibname[0]; /* config.ini */
  case 2: return tlibname[1]; /* vlisp.sys */
  case 3: return tlibname[2]; /* vlisp.ini */
  case 4: return tlibname[5+tlibp];
  case 6: return tlibname[3]; /* bvlisp.ini */
  case 7: return tincname[tinc/2];
  case 8: return tlibname[4]; /* .vlisp-der */
  default: return nil;
  }
}
Int probef(){/* (probef nomfile)  subr 1 */
	CHAR *pch2; Int fd;
	a4=a1; if(isatom(a4))a4=((struct atome *)a4)->pn; pch2=(CHAR *)STR_FROM_STRLSP(a4);
	pch2 = ChkFileName(pch2);
	if((fd=open(pch2,0))<0)a1=nil;else close(fd);
	derec;
}

Int lib(){/* (lib nomfile)  fsubr */
	register CHAR *pch1,*pch2;
	CHAR *sder=".bvlisp-der"; FILE *fd;

	dski=4; a4=car(a1); push(a4);  /* nom de file */
	if(isatom(a4))a4=((struct atome *)a4)->pn; pch2=(CHAR*)STR_FROM_STRLSP(a4);
	pch2 = ChkFileName(pch2);
	aux1=(Int)ppn; aligner(aux1); ppn=(CHAR*)aux1;	/* on cale */

	for(pch1=ppn; *pch1++ = *pch2++;);	
	if((fdes=fopen(ppn,"r"))==NULL){
	  for(pch2=".vlisp",--pch1; *pch1++ = *pch2++;);
	  fdes=fopen(ppn,"r");
	  if(fdes != NULL) {
	    tlibname[tlibp+1]=a4; 
	    if(unefois)
	      {
		unlink(sder); 
		if((fd=fopen(sder,"w")) != NULL)
		  {
		    strcat(ppn," ");	/* ATTENTION au blanc */
		    fputs(ppn,fd); fclose(fd);
		  }
		unefois=0;
	      }
	  }
	}
	pc=libi;}

Int   libi(){/* from vlisp.sys, vlisp.ini et lib */
	if(fdes==NULL) switch(dski){
			case 1 : /* config.ini */
			  derec;
			case 2 : /* vlisp.sys */
			  wst("bvlisp.sys absente\n"); 
			  derec;
			case 3:  /* vlisp.ini */
			  wst("vlisp.ini absente\n"); 
			  derec;
		        case 8:  /* vlisp-der */
			  derec;
			case 6:  /* bvlisp.ini */
			  wst("bvlisp.ini absente\n"); 
			  derec;
			case 4:  /* lib */
			  wst(" * file absente : "); 
			  pop_cast(a1,(Int *));pushf(reent); rec(prini,terpri); return;
		    }
	tlibpil[++tlibp]=(Int)p; inbpt=bufin; *inbpt=0; pc=(Int(*)())lib_2;
  }

Int   lib_2(){/* lib top level loop */
	push(lib_2); rec(readu,eval);}


Int eof(){/* (eof)  subr 0 */
	fclose(fdes);

	if(((struct atome *)atEOF)->fval == 0) p=(Int*)tlibpil[tlibp--];

	switch(dski){
	case 1 : /* config.ini */
	case 2 : /* vlisp.sys */
	case 3 : /* vlisp.ini */
	case 8 : /* .vlisp-der */
	case 6 : /* bvlisp.ini */
	  dski=0;
	  break;
	case 4: /* lib */
	  pop_cast(a1,(Int *));  /* nom file */
	  dski=0;
	  break;
	case 7: /* include */
	  dski=tinclude[tinc--]; fdes=(FILE *)tinclude[tinc--];
	  a1=t;break;
	case 5: /* input */
	  dski=0;a1=t;break;
	case 9: /* open */
	  dski = op_dski[op_num];
	  fdes = op_fp[op_num--];
	}
	aux1=rdprd; iniitt(); 
	if(aux1>0){wst(" * il manque des fermantes\n");dski=0;
		    pc=(Int(*)())reent;}
	if(((struct atome *)atEOF)->fval == 0) popf;
	else {a1=cons(lambda,((struct atome *)atEOF)->fval);a1=ncons(a1);
	pc=(Int(*)())eval;} 
}
Int linput(){/* subr 1, (input nom_de_fichier), si nom_de_fichier = () */
	/* retour vers lecture sur terminal			  */
	register CHAR *pch1, *pch2;
	a4=a1;
	if(isnil(a4))
		{fclose(fdes);aux1=rdprd;iniitt();dski=0;derec;}
	aux1=(Int)ppn; aligner(aux1); ppn=(CHAR*)aux1;
	if(isatom(a4))a4=((struct atome *)a4)->pn; pch2=STR_FROM_STRLSP(a4);
	pch2 = ChkFileName(pch2);
	for(pch1=ppn; *pch1++ = *pch2++;);
	if((fdes=fopen(ppn,"r"))==NULL){
		for(pch2=".vlisp",--pch1; *pch1++ = *pch2++;);
		fdes=fopen(ppn,"r");}
	if(fdes==NULL){a1=nil; derec;}
	dski=5;
	tlibpil[++tlibp]=(Int)p;inbpt=bufin;*inbpt=0;derec;}

Int lopen()
{/* fsubr, (open fichier mode), ramene le fdes */
  /* register CHAR *pch1,*pch2, mode[2]; */
  register CHAR *pch1,*pch2;
  CHAR mode[2];
  FILE *fp;
	       
  mode[1] = 0;
  a4=car(a1);
  aux1=(Int)ppn; aligner(aux1); ppn=(CHAR*)aux1;

  if(isatom(a4)) a4=((struct atome *)a4)->pn;
  else if(!isstr(a4)) {a1=nil; derec;}
  pch2=STR_FROM_STRLSP(a4);
  pch2 = ChkFileName(pch2);
  for(pch1=ppn; *pch1++ = *pch2++;);
  a4=cdr(a1); a4=car(a4);
  if(isatom(a4)) a4=((struct atome *)a4)->pn;
  if(!isstr(a4)) mode[0] = 'r';
  else {
    a4 = (CHAR *)STR_FROM_STRLSP(a4);
    mode[0] = *((CHAR *)a4);
  }
  if((fp=fopen(ppn,mode))==NULL){
    for(pch2=".vlisp",--pch1; *pch1++ = *pch2++;);
    fp=fopen(ppn,mode);
  }
  if(fp==NULL) a1=nil; 
  else {
    a1=cranb((Int)fp);
    op_dski[++op_num] = dski;
    op_fp[op_num] = fdes;
    dski = 9; fdes = fp;
  }
  derec;
}

Int lclose()
{/* subr1, (close fdes), ramene t */
  if(dski == 9) { /* dans un open */  
    fdes=(FILE *)valnb(a1);
    if(fclose(fdes) == EOF) a1=nil;
    else a1=t; 
    fdes = op_fp[op_num];
    dski = op_dski[op_num--];
  }
  derec;
} 


Int freadp(){/* subr1, (fread fdes) */
  /* un -bug- Ernesto, c'esp pas un fsubr fdes=(FILE *)valnb(car(a1)); */
  fdes=(FILE *)valnb(a1);
  push(dski);
  dski=5;
  tlibpil[++tlibp]=(Int)p;inbpt=bufin;*inbpt=0;rec(readu,fread1);}

Int fread1(){/* auxiliaire, remet en etat */
	pop(dski);
	popf;}

Int rtext(){/*  (rtext at)   subr 1 */
	FILE *fd; CHAR *pch; CHAR b[132];

	a4=a1; a2=nil;
	if(isatom(a4))a4=((struct atome *)a4)->pn;
	pch=STR_FROM_STRLSP(a4);
	pch = ChkFileName(pch);
	if(!(fd=fopen(pch,"r"))){a1=nil; derec;}
	while(fgets(b,254,fd) != NULL)
	{
#if 0
	  /* simplifique E. */
	  newppn(); pch=b;
	  while(*ppn++ = *pch++)if(ppn>hstr)gcstrsv();
	  a1=oppn; lpn=ppn-oppn-sizeof(Int); *a1=lpn;
#endif
	  /* creation uniquement si pas que des espaces... 
	     pratique en espace...(D.P)... */
	  pch = b;
	  while(*pch == ' ') ++pch;
	  if(*pch == '\n') continue;
	  newstr(b);
	  a1 = (Int*)oppn;
	  a2=cons(a1,a2);
	}
	fclose(fd); a1=a2; a2=nil;
	pc=(Int(*)())nreverse;
}

Int include(){/*  (include file)  fsubr */
	register CHAR *pch1,*pch2;	
	FILE *faux1;

	a4=car(a1); if(isatom(a4))a4=((struct atome *)a4)->pn; pch2=STR_FROM_STRLSP(a4);
	pch2 = ChkFileName(pch2);
	aux1=(Int)ppn; aligner(aux1); ppn=(CHAR*)aux1;	/* on cale */
	for(pch1=ppn; *pch1++ = *pch2++;);

	if((faux1=fopen(ppn,"r"))==NULL)
	{
	  for(pch2=".vlisp",--pch1; *pch1++ = *pch2++;);
	  faux1=fopen(ppn,"r");
	}
	if(faux1==NULL)
	{
	  wst(" * file include absente : "); rec(prini,terpri); return;
	}

	tinclude[++tinc]=(Int)fdes; tinclude[++tinc]=dski;
	tincname[tinc/2]=a4;
	dski=7; fdes=faux1;
	pc=(Int(*)())libi;
}
				
