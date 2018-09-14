/*
 *     read.c
 */

#include <ctype.h>
#include "main.h"

char read_buf[128];
int pos_read_buf;

init_read()        /* indique que le buffer est vide */
{
   pos_read_buf = 80;
}

unread_char(ch)    /* remettre le caractere dans le buffer */
char ch;
{
   read_buf[--pos_read_buf] = ch;
}

char read_char()   /* ramene un caractere */
{
char ch;

   if (pos_read_buf >= 80)              /* buffer vide ? */
   {
      if (fgets (read_buf, 128, Stdin) == NULL)     /* en lire un */
      {
         if (Stdin == stdin)            /* fin de fichier clavier --> sortie */
         {
            printf("Bye ...\n");
	    exit (0);
         }
         longjmp(jmp_file, 1);          /* fin de fichier disque --> jmp */
      }
      pos_read_buf = 0;                 /* et prendre a partir de 0 */
   }
   ch = read_buf[pos_read_buf++];       /* extraire le caractere */
   if (ch == '\n')
      pos_read_buf = 80;                /* pour la prochaine fois */
   return (ch);
}

lisp_read()     /* lit qqch dans a0 */
{
   a0 = obj_read();
}

int obj_read()
{
int res;
char ch;

   do           /* lire jusqu'au 1er caractere significatif */
   {
      ch = read_char();
   }
   while (isspace(ch) || ch == ')' );

   if (ch == '\'')    /* caractere quotant ? */
   {                  /* construire (quote OBJET-LU) */
      res = obj_read();
      res = cons(quote, cons(res, nil));
      return (res);
   }

                /* selon la nature du 1er caractere
                   chiffre  -->  lire un entier
                   parenthese ouvrante  -->  lire une liste
                   lettre  -->  lire un atome litteral
                 */
   if (isdigit(ch))
      res = int_read(ch);
   else
   if (ch == '(')
      res = cons_read();
   else
   if (isalpha(ch))
      res = atom_read(ch);
   else
   {
      err ("ERREUR: Lecture de caractere inconnu %c\n", ch);
   }

   return (res);
}

int int_read(ch)     /* lecture d'entier */
char ch;
{
int val = 0;         /* initialisation de l'accumulateur */

   do                /* on fait un Horner classique */
   {
      val = (val * 10) + (ch - '0');
      ch = read_char();
   }
   while (isdigit(ch));

   unread_char(ch);  /* rendre le caracere non chiffre */
   return (make_int(val));    /* retourner un nombre Lisp */
}

int atom_read(ch)    /* lecture d'atome */
char ch;
{
int res;
int n_hash;
char atbuf[128];
int i;

   i = 0;
   do                /* d'abord lire le reste de la chaine */
   {
      atbuf[i++] = ch;
      ch = read_char();
   }
   while (isalpha(ch) || isdigit(ch));

   atbuf[i] = '\0';  /* finir la chaine */
   unread_char(ch);


   /* voir si la chaine est ou non le p_name d'un atome deja connu */
   n_hash = hashname(atbuf);
   for (i=hash_table[n_hash] ; i!=nil ; i=CDR(i) )
   {
	if (!strcmp(atbuf,(char *)p_names[CAR(i)]) )
	{
	   res = CAR(i);
	   goto fini;
	}
   }

   res = creatom(atbuf);
   hash_table[n_hash] = cons (res, hash_table[n_hash]);

   fini:
      return(res);
}

int creatom(nom_at)
char *nom_at;
{
int res;
char *pch;
char *malloc();

   if (n_atoms >= MAX_ATS)
      err ("Trop d'atomes\n");

   /* +1  a cause du '\0' final de la chaine a stocker */
   pch = malloc(strlen(nom_at) + 1);
   if (pch == NULL)
      err ("Plus de memoire pour interner un ATOME\n");
   strcpy (pch, nom_at);
   res = n_atoms;
   CVAL(res) = undefined;
   p_names[n_atoms++] = pch;
   return (res);
}

int cons_read()       /* on vient de lire une ouvrante */
{
char ch;
int res;
int x;

   /* technique d'accrochage de bords encore */
   res = x = cons (nil, nil);
   for ( ;; )
   {
      do      /* lire un caractere significatif */
      {
         ch = read_char();
      }
      while (isspace(ch));

      if (ch == ')')   /* fermante tout de suite  -->  () */
         goto fini;
      unread_char (ch);    /* rendre le caractere lu */
      CDR(x) = cons(obj_read(), nil);
      x = CDR(x);
   }
   fini:
      x = CDR(res);
      uncons(res);         /* remettre le doublet de tete dans la freelist */
      return (x);

      return (CDR(res));   /* le doublet de tete est perdu ? */
}

/* FIN de fichier */
