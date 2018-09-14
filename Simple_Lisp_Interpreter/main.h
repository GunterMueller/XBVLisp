/*
 *       main.h
 */

#include <stdio.h>

#include <setjmp.h>
jmp_buf jmp_top;
jmp_buf jmp_file;

#include <stdio.h>        /* pour pouvoir lire un fichier de definitions */
FILE *Stdin, *File;

#define MAXMEM        8092
int mem[MAXMEM];      /* la memoire des objets Lisp */


#define CAR(adr)      mem[adr]        /* acces aux doublets */
#define CDR(adr)      mem[adr+1]
#define CAAR(adr)     (CAR(CAR(adr)))
#define CADR(adr)     (CAR(CDR(adr)))
#define CDAR(adr)     (CDR(CAR(adr)))
#define CDDR(adr)     (CDR(CDR(adr)))


#define B_CONS        256    /* 1er doublet */
#define H_CONS        6998   /* dernier doublet */

#define B_STACK       7000

#define MAX_ATS       256   /* nb d'atomes maximum possible */
#define MAXHASH       23
#define CVAL(adr)     mem[adr]  /* acces aux atomes */
int hash_table[MAXHASH];	/* table pour le hash coding */


#define IS_AT(adr)    (adr >= 0 && adr < B_CONS)      /* tests de type */
#define IS_CONS(adr)  (adr >= B_CONS && adr < H_CONS+2)
#define IS_STACK(adr) (adr >= B_STACK && adr < MAXMEM)
#define IS_INT(adr)   (adr >= MAXMEM && adr <= MAXMEM+24000)

#define make_int(x)   (x+MAXMEM)    /* creation + acces aux nombres */
#define val_int(x)    (x-MAXMEM)

#define pop(x)     x = mem[sp--]

int sp;               /* le pointeur de pile */
int freelist;         /* le pointeur de liste (des doublets) vide */
int maxcons;          /* nb max de doublets allouables */
int a0, a1, a2, a3, a4; /* les registres Lisp */

#define nil           0
#define undefined     1
#define t             2
#define quote         3
#define lambda        8


extern char *p_names[MAX_ATS]; /* tables des noms des atomes */
extern int n_atoms;            /* le nb d'atomes connus */


/* FIN de fichier */
