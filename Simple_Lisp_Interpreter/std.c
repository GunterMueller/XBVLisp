/*
 *      std.c
 */

#include "main.h"

subrs_0()      /* les subrs 0 args : numero dans a4, place resultat dans a0 */
{
   switch(a4)
   {
      case 15:     /* oblist */
         a0 = nil;
         for (a1 = n_atoms-1 ; a1 >= 0 ; a1 --)
            a0 = cons(a1 , a0);
         return;
      case 26:         /* dumpat : affiche les atomes connus */
         a0 = nil;
         {
         int i;
         int cpt = 0;
            for (i=0 ; i<n_atoms; i++)
            {
             printf ("%d %s  CVAL:%d - ", i, p_names[i], CVAL(i));
             obj_print(CVAL(i));
             putchar ('\n' );
             if (++cpt == 18)
             {
                if (getchar() != '\n')
                   break;
                cpt = 0;
             }
            }
         }
         return;
      case 27:        /* dumpcons : dump les doublets */
         a0 = nil;
         printf ("Freelist : %d\n", freelist);
         {
         int i;
         int cpt = 1;
            for (i=H_CONS ; i>=B_CONS ; i-=2)
            {
             printf ("%d  CAR:%d  CDR:%d\n", i, CAR(i), CDR(i));
             if (++cpt == 18)
             {
                if ( getchar() != '\n')
                   break;
                cpt = 0;
             }
            }
         }
         return;
      case 28:        /* gc : provoque un GC */
         a0 = nil;
         gc();
         return;
      case 29:        /* nbfree : ramene le nb de doublets libres */
         {
         int i, n;
            for (i=freelist, n=1 ; i!=nil ; i=CDR(i), n++);
            a0 = make_int(n);
        /*    a0 = make_int (sp);   pour connaitre taille pile  */
         }
         return;
      case 40:        /* dumppile */
         a0 = nil;
         {
         int i, cpt;
            cpt = 1;
            printf ("sp = %d\n", sp);
            for (i=B_STACK ; i<=sp ; i++)
            {
               printf ("%d  ", i);
               if (mem[i] >= 0)
                  obj_print (mem[i]);
               else
                  printf ("%d", mem[i]);
               putchar('\n');
               if (++cpt == 18)
               {
                  if (getchar() != '\n')
                     break;
                  cpt = 0;
               }
            }
         }
         return;
      case 34:        /* read */
         lisp_read();
         return;
   }  /* fin de switch(a4)  */
}

subrs_1()      /* les subrs 1 args : numero dans a4, place resultat dans a0 */
{
char buf[BUFSIZ];
   switch(a4)
   {
      case 4:          /* car */
         if (!IS_CONS(a0))
         {
            obj_print(a0);
            err ("\n*** car : l'arg doit etre une liste\n", 0);
         }
         a0 = CAR(a0); return;
      case 5:          /* cdr */
         if (!IS_CONS(a0))
         {
            obj_print(a0);
            err ("\n*** cdr : l'arg doit etre une liste\n", 0);
         }
         a0 = CDR(a0); return;
      case 6:          /* add1 */
         if (!IS_INT(a0))
         {
            obj_print(a0);
            err ("\n*** add1 : l'arg doit etre un entier\n", 0);
         }
         a0 = make_int(val_int(a0) + 1); return;
      case 7:          /* sub1 */
         if (!IS_INT(a0))
         {
            obj_print(a0);
            err ("\n*** sub1 : l'arg doit etre un entier\n", 0);
         }
         a0 = make_int(val_int(a0) - 1); return;
      case 10:         /* print */
         obj_print(a0); putchar('\n'); return;
      case 24:         /* null */
      case 38:         /* not */
         a0 = (a0 == nil) ? t : nil; return;
      case 35:         /* numberp */
         a0 = (IS_INT(a0)) ? t : nil; return;
      case 36:         /* atom */
         a0 = (IS_AT(a0) || IS_INT(a0)) ? t : nil; return;
      case 37:         /* listp */
         a0 = (IS_CONS(a0)) ? t : nil; return;
      case 42:	       /* load */
	 if (!IS_AT(a0))
	 {
	    obj_print (a0);
	    err ("\n***load : l'arg doit etre un atome\n", 0);
	 }
	 strcpy(buf, p_names[a0]);
	 strcat(buf, ".lsp");
	 load(buf);
	 a0 = t; return;
   }  /* fin de switch(a4)  */
}

subrs_2()      /* les subrs 2 args : numero dans a4, place resultat dans a0 */
{
   switch(a4)
   {
      case 12:         /* eq */
         a0 = (a1 == a0) ? t : nil; return;
      case 14:         /* times */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** times : les args doivent etre des nombres\n", 0);
         }
         a0 = make_int(val_int(a0) * val_int(a1)); return;
      case 16:         /* plus */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** plus : les args doivent etre des nombres\n", 0);
         }
         a0 = make_int(val_int(a0) + val_int(a1)); return;
      case 17:         /* difference */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** difference : les args doivent etre des nombres\n", 0);
         }
         a0 = make_int(val_int(a0) - val_int(a1)); return;
      case 18:         /* lt */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** lt : les args doivent etre des nombres\n", 0);
         }
         a0 = (val_int(a0) < val_int(a1)) ? t : nil; return;
      case 19:         /* gt */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** gt : les args doivent etre des nombres\n", 0);
         }
         a0 = (val_int(a0) > val_int(a1)) ? t : nil; return;
      case 20:         /* le */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** le : les args doivent etre des nombres\n", 0);
         }
         a0 = (val_int(a0) <= val_int(a1)) ? t : nil; return;
      case 21:         /* ge */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** ge : les args doivent etre des nombres\n", 0);
         }
         a0 = (val_int(a0) >= val_int(a1)) ? t : nil; return;
      case 23:         /* cons */
         if ( !IS_CONS(a1) && (a1 != nil) )
         {
            obj_print(a1);
            err ("\n*** cons : le 2eme arg doit etre une liste\n", 0);
         }
         a0 = cons (a0, a1); return;
      case 32:         /* quotient */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** times : les args doivent etre des nombres\n", 0);
         }
         a0 = make_int( (int) (val_int(a0) / val_int(a1)) ); return;
      case 33:         /* remainder */
         if ( !IS_INT(a0) || !IS_INT(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** times : les args doivent etre des nombres\n", 0);
         }
         a0 = make_int(val_int(a0) % val_int(a1)); return;
      case 30:         /* rplaca */
         if ( !IS_CONS(a0) )
         {
            obj_print(a0);
            err ("\n*** rplaca : le 1er arg doit etre une liste\n", 0);
         }
         CAR(a0) = a1;
         return;
      case 31:         /* rplacd */
         if ( !IS_CONS(a0) || !IS_CONS(a1) )
         {
            obj_print(a0);
            putchar('\n');
            obj_print(a1);
            err ("\n*** rplacd : les args doivent etre des listes\n");
         }
         CDR(a0) = a1;
         return;
   } /* fin de switch(a4) */
}

/* FIN de fichier */
