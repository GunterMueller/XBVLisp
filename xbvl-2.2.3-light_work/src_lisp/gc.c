
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *      gc.c            garbage-collecting
 *                      Patrick Greussay  27 Oct 82
 *                      Harald Wertz Decembre 82
 *                      Harald Wertz Fevrier 84
 *                      Olivier Blanc Avril 90
 *                      Damien Ploix Frevrier 93 : gc des chaines
 *                      Damien Ploix Mai 93 : ajout des sub de recuperation de 
 *                                            l'etat de la memoire
 */

#include <vlisp.h>

static CHAR *gcppn, *gcbstr, *gchstr; /* garbage chaine */

Int erstr();
#ifdef STDC_HEADERS
void mark(Int *), strgci() ;
/* void *malloc(UInt), *free(char *) ; */ /* GM */
void *malloc(size_t);
void *free(char *) ;
#else
void mark(), strgci() ;
char *malloc(), *free() ;
#endif

# define at_ptr(x) ((struct atome *)x)
/* # define strmark(x) { if(ismstr(x)) (Int *) (x) = savestr(x) ;  else mark(x); } */ /* GM */
# define strmark(x) { if(ismstr(x)) (x) = savestr(x) ;  else mark(x); }
# define unmark(x)   ((x) & ~markmask)
# define ismarked(x) ((x) & markmask)
# define ismstr(x)   ((CHAR *)(x) >= hdsysstr && (CHAR *)(x) < hstr)

Int *savestr(str) 
register Int *str; 
{
  register Int  s_len, *retadr, xx ;

  if((CHAR *) str < hdsysstr) return str ;    /* chaine system : on ne fait rien */

  if((Int)str & 02) return str ;                     /* marque dans l'ancienne */

  if(!gcppn) return (Int*)((Int)str | 02);

  if((*str) & 02 && (CHAR*)(*str) > hdsysstr) return (Int *)*str ; /* autres ... */ 

  s_len = *str + sizeof(Int) ;
  memcpy(gcppn, (CHAR *)str, s_len); 
  xx = (Int)(hdsysstr + (gcppn - gcbstr)) | 03;
  *(Int **)str = retadr =  (Int *)xx;
  gcppn += s_len; 

  aux1 = (Int) gcppn; aligner(aux1); gcppn = (CHAR *) aux1;     /* nouvelle adresse... */
  return retadr;                                 /* l'adresse definitive... */
}



void gci() /* gc interne */
{
  Int ndoub,nnb,nnat, type_gc, l_mem;
#ifndef NO_VFLO
  Int nflo;
#endif
  Int *x; UInt v;
  Int xx;
  extern Int *ce,*cn,*clcp,*ilitlist,*iliteral;

  if(!pgc) {
    wst("GC"); 
    if(!freeflo) 
      wst(" Flotants "), type_gc = 0;
    if(!freeat)  wst(" Atomes "), type_gc = 1;
    if(!freel) {
      wst(" Listes ");
      type_gc = 2;
    }
    if(!freenb)  wst(" Nombres "), type_gc = 3;
    if(ppn>hstr) wst(" Chaines "), type_gc = 4;
  }

  /* pas de DEL pendant le GC */
  signal(SIGINT,SIG_IGN);
  /* on sauve les registres */
  push(a1); push(a2); push(a3); push(a4);

  /* on sauve les registres lovlisp */
  push(ce); push(cn); push(clcp); push(ilitlist); push(iliteral);

  /* on marque les atomes */
  for(x=(Int *) derat; x ; x=(Int *) ((struct atome *)x)->nexa)
    {
      strmark(at_ptr(x)->cval) ;
      strmark(at_ptr(x)->pval) ;
      if(ATOME_FTYP(x) >= EXPR) strmark(at_ptr(x)->fval) ; 
      if(at_ptr(x)->ival != nil) strmark(at_ptr(x)->ival) ;
      if(at_ptr(x)->pakage != nil) strmark(at_ptr(x)->pakage) ;
    }

  /* on marque la pile */

  for(x=p; x >= (Int *) bpil ; --x) strmark(x);

  /* on marque la table des fval des macro caracteres */
  for(xx = 0; xx < 128; xx++)
    {
      if(tabfval[xx].fval) strmark(tabfval[xx].fval);
    }
          
  /* on met en place la nouvelle mémoire de chaine */
  if(gcppn && (l_mem = gcppn - gcbstr) > 0) {
    memset((void *) hdsysstr, 0, (Int) (hstr - hdsysstr));
    memcpy(hdsysstr, gcbstr, l_mem); 
    aux1 = (Int)hdsysstr + l_mem; aligner(aux1); 
    oppn = ppn = (CHAR*)aux1 ; /* reaffecte le pointeur de travail */
    gcppn = NULL;                   /* invalidation du str garbage */
  }

  /* on balaye la zone liste */
  for(freel=(Int *) (ndoub=(Int)NULL),x=(Int *)blst; x <= (Int *)hlst; x+=2) {
    
    if((v = *(x)) & 02){ *(x) &= ~03; } /* normal big ou chaîne */
    else if(v & 01){                                            /* small */
        aux1=v;
#ifndef NO_VFLO
        v-=(UInt)bflo; v>>=2;  *(x)=v;
#else
        v-=(UInt)bnum; v>>=2;  *(x)=v;
#endif

      }
      else{++ndoub; 
      *(x+1)=(Int)freel;*(x)=(Int)NULL;
      freel=x;}
  }


  /* on balaye la zone nombre */
  for(freenb=(Int *) (nnb=0),x=(Int *)bnum; x <= (Int *) hnum; x+=2)
    if(*(x+1)) *(x+1)=0;
    else{++nnb; *(x)=(Int)freenb;freenb=x;}

#ifndef NO_VFLO
  /* on balaye la zone flo */
  for(freeflo=(Int *) (nflo=0),x=(Int *)bflo; x <= (Int *)hflo; x+=4)
    {
      if(*(x+2)==1)*(x+2)=0;
      else if(x >= (Int*)busrflo) {++nflo; *(x)=(Int)freeflo; freeflo=x;}
    }
#endif

  /* on balaye la zone atome */
  for(freeat=NULL,nnat=0,x=(Int *)atuser,derat=x-(SIZAT/sizeof(Int)); x <= (Int *)hat;
      x+=(SIZAT/sizeof(Int)))
    {
      xx=at_ptr(x)->oftyp;
      if((Int)at_ptr(x)->pn & 02) { at_ptr(x)->pn = (Int*)((Int)at_ptr(x)->pn & ~03); }

      if(xx & 0200)     /* est-il marque par quelqu'un ? */
        {               /* oui, on demarque! */
          xx &= ~0200; at_ptr(x)->oftyp=xx; 
          at_ptr(x)->nexa=(ATOMEPTR)derat; derat=x;
        }
      else              /* est-ce un atome ABSOLUMENT sans valeur ? */
        if(   at_ptr(x)->cval==undef 
              && at_ptr(x)->pval==nil 
              && at_ptr(x)->fval==0 
              && at_ptr(x)->ival==nil
              && at_ptr(x)->obj == nil) 
          {             /* oui on le met en liste libre d'atomes! */
            at_ptr(x)->nexa=(ATOMEPTR)freeat; freeat=x; ++nnat; 
            if(STRPNAME(x)){          /* atome reel */
              if(at_ptr(x)->pakage != nil && (Int)at_ptr(at_ptr(x)->pakage)->pn & 02) {
                at_ptr(at_ptr(x)->pakage)->pn = (Int*)((Int)at_ptr(at_ptr(x)->pakage)->pn & 03);
              }
              enleve_atome((ATOMEPTR)x); /* et on l'enleve de la hash-table */
            }
          }
        else
          {
            at_ptr(x)->nexa=(ATOMEPTR)derat; derat=x;
          }
    }

  /* modification d'Olivier: inutile de le faire 2 fois ...*/
  signal(SIGINT,(void (*)(int))onintr);         /* on peut reinterrompre */
  /* visualisation s'il y a lieu */

  if(pgc)
    {
      wst("GC="); wint(ngc); wst(",listes="); wint(ndoub);
      wst("\n");
      wst(",nombres="); wint(nnb);
#ifndef NO_VFLO
      wst(",flottants="); wint(nflo);
#endif
      wst("\n");
      wst(",atomes="); wint(nnat);
      wst(",pile="); wint(((hpil-bpil)/sizeof(Int))+1);
      wst(",chaines="); 
      if(!gcppn) wint((aux1=hstr-ppn) < 0 ? 0 : aux1);
      else wint((aux1=gchstr-gcppn) < 0 ? 0 : aux1);
      wst("\n");
    }
  else {
    switch(type_gc) {
    case 0 : wint(nflo); break;
    case 1 : wint(nnat); break;
    case 2 : wint(ndoub); break;
    case 3 : wint(nnb); break;
    case 4 : wint((aux1=hstr-ppn) < 0 ? 0 : aux1); break;
    }
    wst(" Done\n");}

  /* problemes ? */
  signal(SIGINT,(void (*)(int))onintr);         /* on peut reinterrompre */
  if(!ndoub){wst("Catastrophe: plus de doublets\n"); RPT();}
  if(!nnb){wst("Catastrophe: plus de nombres\n"); RPT();}
#ifndef NO_VFLO
  if(!nflo){wst("Catastrophe: plus de flottants\n"); RPT();}
#endif

  if(!nnat)
    {wst("Catastrophe: plus d'atomes\n"); RPT();}

  signal(SIGINT,SIG_IGN);
  ++ngc;  /* un gc de plus ! */

  /* on retablit les registres lovlisp */
  pop_cast(iliteral, (Int *)); pop_cast(ilitlist, (Int *)); 
  pop_cast(clcp, (Int *)); pop_cast(cn, (Int *)); pop_cast(ce, (Int *));
  /* on retablit les registres */
  pop_cast(a4,(Int *)); pop_cast(a3,(Int *)); pop_cast(a2,(Int *)); pop_cast(a1,(Int *));
}

void mark(x) register Int *x;{
        register Int *y; unsigned v; Int xx;

        for(;;){

#ifndef NO_VFLO
          if(isflo(x)){*(x+2)=1; return;}
#endif
          if(x< (Int *) bnum)return;
          xx=(Int)x; if(xx & 01)return;
          if(x <= (Int *) hnum){*(x+1)=1; return;}
          if(x <= (Int *) hstr) return ;
          if(x <= (Int *) hat)
          {
            if(x >= atuser) /* cas atome non-initial */
            {
                /* heuristique si on trouve dans la pile une adr de code */
                /* esperons que ca tiendra le choc ! */
              if(!isustr(at_ptr(x)->pn))return;
              if(ismstr(at_ptr(x)->pn) && (Int)!at_ptr(x)->pn & 03) 
                at_ptr(x)->pn = (Int*)savestr(at_ptr(x)->pn);
              xx=(Int) at_ptr(x)->oftyp; xx |= 0200; 
              at_ptr(x)->oftyp=(CHAR)xx; 
              return;
            }
            return;
          }

          if(x > (Int *) hlst)return;
          if(*(x) >= (Int)bnum && *(x) <= (Int)hnum) *(((Int *)*(x))+1) = 1;
#ifndef NO_VFLO
          if((v= (UInt) *(x)) < (UInt) bflo){
            v<<=2; v|=01; v+=(UInt)bflo;        /* small */
#else
          if((v= (UInt) *(x)) < (UInt) bnum){
            v<<=2; v|=01; v+=(UInt)bnum;        /* small */
#endif
            *(x)=v; x=cdr(x); continue;
          }
          if(v & 01) return;

          if(ismstr((CHAR *)v)) *(x) = (Int) savestr(v) ;
          else{
            y= (Int*)*(x); *(x) |=  03;         /* big */
            
            if(!isnum(y) && ((UInt)y & (UInt)ANNOT))
              {y=(Int*)((UInt)y & (UInt)~ANNOT);}
            mark(y);
          }
            
          if(ismstr((CHAR *)*(x+1))) {
            *(x+1)=(Int) savestr(*(x+1));
            return;
          }
          if(!isnum(*(x+1)) && ((UInt)*(x+1) & (UInt)ANNOT)) {
            *(x+1) |= 03;
            x=(Int*)((UInt)*(x+1) & (UInt)~ANNOT);
          }
          else 
            x= (Int*)*(x+1);
       }
          
}

void gcstrsv(){
        CHAR *oxppn; Int loxppn;
        oxppn=(oppn+sizeof(Int)); 
        loxppn=strlen(oxppn); *ppn='\0';
        strgci();
        if(ppn+loxppn > hstr)erstr();
        newppn();
        while(*ppn++ = *oxppn++); --ppn;
}

void strgci()
{
  Int l_mem = (Int) (hstr - hdsysstr) ;
  CHAR *xgcstr ;
  if(atgc == t) {
    wst("\n");
    wst("Effectuer le GC ?");
    xgetch1(NULL);
  }

  /* memoire temporaire de la taille de la memoire de chaine */
  if((xgcstr = (CHAR *) malloc(l_mem+4))) {
    aux1 = (Int) xgcstr ;
    aligner(aux1) ;
    gcppn = gcbstr = (CHAR *)aux1;
    gchstr = gcppn + l_mem;
  }
  else gcppn = NULL ; /* pas assez de memoire pour le gc... */
  gci();
  if(xgcstr) (void) free(xgcstr) ;                  /* liberation de la memoire temp */
}

Int erstr(){
        wst("Catastrophe: plus de chaines\n"); ppn=oppn; onintr();
        return 0;
}


Int gc(){/* (gc e)  subr 1 */
        pgc=(isnil(a1)) ? 0 : 1;  
        strgci();
        derec;
}

/*  fonctions de base pour recuperer l'etat d'occupation de la memoire
 */
 
/*---------------------------------------------------------------
 *      atomuses
 *      retourne le pourcentage d'utilisation de la memoire atome
 ---------------------------------------------------------------*/
bVlispFonc atomuses() /* (atomuses) */
{
  Int nbat;
  struct atome *acnt = (struct atome *) freeat;
  
  for(nbat = 0; acnt; nbat++, acnt = (struct atome *) acnt->nexa) ;
  
  a1 = cons(cranb(nbat), cranb(((hat - bat)/sizeof(struct atome)) - nbat));
  derec;
}

/*---------------------------------------------------------------
 *      struses
 *      retourne le pourcentage d'utilisation de la memoire chaine
 ---------------------------------------------------------------*/
bVlispFonc struses() /* (struses) */
{
  a1 = cons(cranb(hstr - ppn), cranb(ppn - bstr));
  derec;
}
/*---------------------------------------------------------------
 *      listuses
 *      retourne le pourcentage d'utilisation de la memoire list
 ---------------------------------------------------------------*/
bVlispFonc listuses() /* (listuses) */
{
  Int nlst, *lcnt = freel;

  for(nlst = 0; lcnt; ++nlst, lcnt = cdr(lcnt)) ;
  a1 = cons(cranb(nlst), cranb(((hlst - blst -1)/sizeof(Int *)) - nlst));
  derec;
}
/*---------------------------------------------------------------
 *      flouses
 *      retourne le pourcentage d'utilisation de la memoire flotant
 ---------------------------------------------------------------*/
bVlispFonc flouses() /* (flouses) */
{
  Int nflo, *fcnt = freeflo;

  for(nflo = 0; fcnt; nflo++, fcnt = car(fcnt)) ;
  a1 = cons(cranb(nflo), cranb(((hflo - bflo)/sizeof(float)) - nflo));
  derec;
}
/*---------------------------------------------------------------
 *      numuses
 *      retourne le pourcentage d'utilisation de la memoire nombre
 ---------------------------------------------------------------*/
bVlispFonc numuses() /* (numuses) */
{
  Int nbnum, *ncnt = freenb;
  for (nbnum = 0; ncnt; ++nbnum, ncnt = car(ncnt)) ;
  a1 = cons(cranb(nbnum), cranb(((hnum - bnum)/sizeof(Int)) - nbnum));
  derec;
}
/*---------------------------------------------------------------
 *      piluses
 *      retourne le pourcentage d'utilisation de la pile
 ---------------------------------------------------------------*/
bVlispFonc piluses() /* (piluses) */
{
  a1 = cons(cranb(((Int) hpil - (Int) p) / sizeof(Int)),
            cranb(((Int) p - (Int) bpil) / sizeof(Int)));
  derec;
}

