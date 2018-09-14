/*
 *     mem.c
 */

#include "main.h"

char *malloc();

#define DEBUGG    /* pour debugger */

#define MAX_AUTO_EVAL 3
int n_atoms;
/*
int hash_table[MAXHASH];
*/

#define N_ATOMS 43
char *p_names[MAX_ATS] = {
   "nil", "undefined", "t", "quote", "car", "cdr", "add1", "sub1", "lambda",
   "defun", "print", "setq", "eq", "if", "times", "oblist", "plus",
   "difference", "lt", "gt", "le", "ge", "cond", "cons", "null", "exit",
   "dumpat", "dumpcons", "gc", "nbfree", "rplaca", "rplacd", "quotient",
   "remainder", "read", "numberp", "atom", "listp", "not", "list", "dumppile",
   "progn", "load",
   0 };

init_stack()
{
   sp = B_STACK;
}

init_atomes()
{
register int i, n_hash;

   init_hash();

   /* les atomes auto-evaluants sont leur propre valeur */
   for (i=nil ; i<MAX_AUTO_EVAL ; i++)
   {
      CVAL(i) = i;
      n_hash = hashname(p_names[i]);
      hash_table[n_hash] = cons (i, hash_table[n_hash]);
   }

   /* et les autres ont la valeur indefinie */
   for ( ; i<N_ATOMS ; i++)
   {
      CVAL(i) = undefined;
      n_hash = hashname(p_names[i]);
      hash_table[n_hash] = cons (i, hash_table[n_hash]);
   }

   for ( ; i<MAX_ATS ; i++)
   {
      CVAL(i) = undefined;
   }

   /* nb d'atomes initiaux connus */
   n_atoms = N_ATOMS;
}

init_listes()     /* fabrique une liste libre
                     place dans  freelist  l'adresse du 1er doublet libre */
{
register int i;

   maxcons = (H_CONS - B_CONS + 2);   /* nb max de doublets */
   freelist = nil;
   for (i=B_CONS ; i<=H_CONS ; i+=2)
   {
      CDR(i) = freelist;
      freelist = i;
   }
}

int cons_r0, cons_r1;   /* pour le garbage */

int cons(x,y)      /* ramene le nouveau doublet dument rempli avec x et y */
int x, y;
{
int res;

   if(freelist == nil)
   {
      cons_r0 = x;
      cons_r1 = y;
      gc();
   }
   CAR(freelist) = x;
   res = freelist;
   freelist = CDR(freelist);
   CDR(res) = y;
   return(res);
}

uncons (x)         /* remet un doublet libre dans la freelist */
int x;
{
   CDR(x) = freelist;
   freelist = x;
}

push(x)   /* pour empiler x en verifiant que la pile n'est pas debordee */
int x;
{
   if (sp == MAXMEM-1)
   {
/**** Inutile : fait apres setjmp, permet le unbind() des parametres
      init_stack();    * reinitialiser la pile *
 */
      err("Pile debordee\n");
   }
   mem[++sp] = x;
}

/* GARBAGE COLLECTOR */

char *freebits;          /* pointeur de bitmap */
int maxbits;             /* taille en octets de la bit-map */
                         /* freebits[ n >> 4 ] : octet ou est le bit
                            correspondant au doublet
                            (1 << (n & 7)) : position dans l'octet, et on
                            divise encore par 2 pour calculer le no de doublet
                            i.e. 0, 1, 2, ..., MAXCONS-1
                          */

#define mark(n)  freebits[(n-B_CONS) >> 4] |= (1 << (((n) >> 1) & 7))
#define marked(n) (freebits[(n-B_CONS) >> 4] & (1 << (((n) >> 1) & 7)) )

gc()      /* GC : Mark and Sweep */
{
register int n, i;
register char *s;

   printf ("GC Commence\n");

   /* allocation et initialisation de la bitmap */
   freebits = malloc(maxbits=(maxcons+7) >> 3);
   if (!freebits)
   {
      err("GC impossible : plus de memoire disponible\n");
   }

/***** Ancienne version creant pbs a malloc dans 'vsch'
   for (s=freebits, n=maxbits ; n-- ; )
      *s++ = 0:
 */
   for (s=freebits, n=0; n<maxbits ; n++ )   /* test d'arret : n=0  ??? */
   {
      s[n] = 0;
   }

   for (i=0 ; i<n_atoms ; i++)    /* on marque les valeurs des atomes */
      marquer (CVAL(i));

   marquer (a0);       /* on marque les registres */
   marquer (a1);
   marquer (a2);
   marquer (a3);
   marquer (a4);
   marquer (cons_r0);
   marquer (cons_r1);

   for (i=B_STACK ; i<=sp ; i++)   /* on marque les choses empilees */
      marquer (mem[i]);

   for (i=0 ; i<MAXHASH ; i++)	   /* on marque la table de hash */
      marquer(hash_table[i]);

   n = balayer();        /* recuperation de ce qui n'est pas marque */

   if ( !n )
      err ("GC: plus de doublets");
   else
      printf ("GC: %d doublets libres\n", n);

   free (freebits);                           /* liberer la bit-map */
/*
   return (n);
*/
}

marquer (x)          /* GC-Marquage */
register int x;
{
   if ( IS_CONS(x) )
   {
      if (marked(x))
         return;
      mark(x);
      marquer (CAR(x));
      marquer (CDR(x));
   }
}

int balayer ()       /* GC-Balayage */
{
register int i, nbcons;

   nbcons = 0;
   freelist = nil;   /* devrait l'etre deja ? */

   for (i=B_CONS ; i<=H_CONS ; i+=2)
   {
      if ( !marked(i) )
      {
         CDR(i) = freelist;
         freelist = i;
         nbcons++;
      }
   }
   return (nbcons);
}

/*
 *       Fonctions pour le Hash Coding
 */

init_hash()
{
register int i;

   for (i=0 ; i<MAXHASH ; i++)
      hash_table[i] = nil;
}

int hashname (nom)
char *nom;
{
register int n;

   for (n=0 ; *nom ; ++nom)
      n += *nom;
   if (n < 0)
      n = -n;
   return (n % MAXHASH);
}

/* FIN de fichier */
