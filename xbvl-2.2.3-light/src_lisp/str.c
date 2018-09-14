/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	string.c
 *		Ernesto Sendoya G.  20 Oct 92
 *   pour une nouvelle implemantation de chaines
 */

#include <vlisp.h>


/* on interne les chaines dans un space memore
 * gere dynamiquement.
 * Les chaines sont partagee et accedees par une
 * hash table.
 */

/* 
 * On interne une chaine dans un emplacement de type :
 */
 
typedef 
struct bvlisp_chaine {
  CHAR init ;         /* chaine initialise */
  Int count ;         /* nbr. d'objets qui pointent vers cette chaine */
  CHAR *string;       /* la chaine */
  struct bvlisp_chane *next; /* pour le hash */
} BVLSTRING;


bVlispFonc print_strings()/* (print_strings ) subr0 */
{
  CHAR *ptr;
  Int i= 0, l;
  Int * tmp = nil;

  aux1 = (Int)bstr; 
  aligner(aux1);
  ptr = (CHAR *)aux1;
  while((l = *(Int *)ptr) && (Int)ptr < (Int)hstr){
    tmp = cons((Int *)ptr, tmp);
    i ++;
    ptr += sizeof(Int) ;
    printf( " l = %d [%d], s = %s <->", l, strlen(ptr), ptr); fflush(stdout);
    ptr += l;
    aux1 = (Int)ptr;
    aligner(aux1);
    ptr = (CHAR *) aux1;
  }
  printf("\ndeb: bstr = %d, fin = %d [%d k]\n",
	 (Int)bstr, (Int)ptr, (Int)((Int)ptr-(Int)bstr)/1000);
  printf("===> no chaines = %d\n", i); fflush(stdout);
  a1 = (Int *)tmp;
  derec;
}
