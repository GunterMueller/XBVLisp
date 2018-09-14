/*
 *       main.c
 */

#include "main.h"

main()
{
static int nbtop = 0;
   init_listes();
   init_atomes();
   setjmp (jmp_top);
   init_read();
   init_stack();
   Stdin = stdin;
   if (++nbtop == 1)
      load ("llisp.ini");
   toplevel();
}

toplevel()
{
   for ( ;; )
   {
      printf ("? "); fflush (stdout);
      lisp_read();
      eval();
      printf ("= ");
      lisp_print();
      putchar ('\n'); fflush (stdout);
   }
}

/* routine d'erreur : obj est un entier, adresse (index dans mem) d'un elt */
err(fmt, obj)
char *fmt;
int obj;
{
int i;
   printf (fmt, obj);
   while (sp > B_STACK)
      if (pop(i) == -2)
         unbind();
   longjmp (jmp_top, 0);
}

/* routine d'erreur : obj est une chaine (p_name) d'un elt */
err_s(fmt, obj)
char *fmt;
char *obj;
{
int i;
   printf (fmt, obj);
   while (sp > B_STACK)
      if (pop(i) == -2)
         unbind();
   longjmp (jmp_top, 0);
}

load (name)
char *name;
{
   if ((File = fopen (name, "r")) == NULL)
   {
      printf ("Le fichier %s n'existe pas\n", name);
   }
   else
   {
      printf ("Chargement de %s\n", name);
      Stdin = File;
      if (setjmp(jmp_file))
         goto fini;
      while (!feof(Stdin))
      {
         lisp_read();
         eval();
         lisp_print();
         putchar ('\n');
      }
fini:
      fclose (File);
      Stdin = stdin;
      init_read();
   }
}

debug(s, x)
char *s;
int x;
{
   printf ("%s", s);
   obj_print (x);
   putchar ('\n');
}

/* FIN de fichier */
