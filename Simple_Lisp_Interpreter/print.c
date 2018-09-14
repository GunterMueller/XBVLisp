/*
 *        print.c
 */

#include "main.h"

#define DEBUGG     /* pour debugger */

lisp_print()   /* imprime l'objet dans a0 */
{
   obj_print(a0);
}

obj_print(obj)
int obj;
{
   if (IS_AT(obj))
   {
      printf("%s", p_names[obj]);
      return;
   }
   if (IS_INT(obj))
   {
      printf("%d", val_int(obj));
      return;
   }
   if (IS_CONS(obj))
   {
      list_print(obj);
      return;
   }
   err ("Objet a imprimer inconnu : %d\n", obj);
}

list_print (obj)
int obj;
{
   putchar( '(' );
   for ( ;; )
   {
      obj_print (CAR(obj));
      if ( (obj = CDR(obj)) == nil)
         break;
      putchar ( ' ' );
   }
   putchar ( ')' );
}

/* FIN de fichier */
