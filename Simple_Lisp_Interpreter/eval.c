/*
 *       eval.c
 */

#include "main.h"

#define DEBUGG      /* pour debugger */

eval()  /* place dans a0 le resultat de l'evaluatiojn de a0 */
{
debut_eval:
   if (IS_INT(a0)) /* a0 entier --> auto-evaluant */
      return;
   if (IS_AT(a0))  /* a0 variable --> valeur ou erreur si undef */
   {
      if ( CVAL(a0) == undefined )
         err_s ("Atome %s, valeur indefinie\n", p_names[a0]);
      a0 = CVAL (a0);
      return;
   }
   if (!IS_CONS(a0))   /* si non-entier, non-atome et non-liste alors PBS */
      err("Objet a evaluer inconnu %d\n", a0);

   a4 = CAR (a0);  /* c'est une liste, donc appel de fonction */
evalcar:
   if (IS_INT(a4))
      err ("Nombre en position fonctionnelle %d\n", val_int(a4) );
   if (IS_AT(a4))  /* c'est un atome */
   {
      switch (a4)
      {
         /* --------- subrs 0 args ---------  */
      case 15:    /* oblist */
      case 26:    /* dumpat */
      case 27:    /* dumpcons */
      case 28:    /* gc */
      case 29:    /* nbfree */
      case 34:    /* read */
      case 40:    /* dumppile */
         if (CDR(a0) != nil)
            goto nargs_incorrect;
         subrs_0();
         return;
      case 25:    /* exit */
         printf ("Bye ...\n");
	 exit (0);
         /* --------- subrs 1 args ---------  */
      case 4:     /* car */
      case 5:     /* cdr */
      case 6:     /* add1 */
      case 7:     /* sub1 */
      case 10:    /* print */
      case 24:    /* null */
      case 35:    /* numberp */
      case 36:    /* atom */
      case 37:    /* listp */
      case 38:    /* not */
      case 42:	  /* load */
         if (CDDR(a0) != nil)
            goto nargs_incorrect;
         push (a4);      /* sauver repere de fonction */
         a0 = CADR (a0); /* prendre 1er arg */
         eval();         /* l'evaluer --> res dans a0 */
         pop(a4);        /* repere de fonction dans a4 */
         subrs_1();
         return;

         /* --------- subrs 2 args ---------  */
      case 12:    /* eq */
      case 14:    /* times */
      case 16:    /* plus */
      case 17:    /* difference */
      case 18:    /* lt */
      case 19:    /* gt */
      case 20:    /* le */
      case 21:    /* ge */
      case 23:    /* cons */
      case 30:    /* rplaca */
      case 31:    /* rplacd */
      case 32:    /* quotient */
      case 33:    /* remainder */
         if (CDR(CDDR(a0)) != nil)
            goto nargs_incorrect;
         push (a4);
         a0 = CDR(a0);
         push (CADR(a0));   /* sauver le 2eme arg */
         a0 = CAR(a0);
         eval();            /* evalue le 1er arg */
         a1 = a0;
         pop(a0);
         push (a1);         /* echange du sommet de pile avec a0 */
         eval();            /* evalue de 2eme arg */
         a1 = a0;
         pop (a0);          /* a1 = val du 2eme arg, a0 = val du 1er arg */
         pop (a4);
         subrs_2();
         return;
         /* --------- subrs 3 args ---------  A FAIRE  */
         /* --------- fsubrs ---------  */
      case 3:      /* quote */
         a0 = CADR(a0);
         return;
      case 39:     /* list */
         a0 = CDR(a0);
         evlis();
         return;
      case 41:     /* progn */
         a0 = CDR(a0);
         progn();
         return;
      case 9:      /* defun */
         a0 = CDR (a0);
         a1 = CAR (a0);  /* le nom de la fonction */
         if (!IS_AT(a1))
         {
            obj_print (a1);
            err ("*** Le nom de fonction doit etre un atome\n");
         }
         a2 = CADR (a0); /* liste des args */
         if (!IS_CONS(a2))
         {
            obj_print(a2);
            err ("*** Les parametres doivent etre en liste\n");
         }
         a3 = CDDR (a0); /* le corps de la fonction */
         a4 = cons (lambda, cons(a2, a3));
         CVAL(a1) = a4;
         a0 = a1;
         return;
      case 11:     /* setq */
         a0 = CDR(a0);
         if (!IS_AT(CAR(a0)))
         {
            obj_print (CAR(a0));
            err ("*** La variable doit etre un atome\n");
         }
         push(CAR(a0));       /* sauver la variable */
         a0 = CADR(a0);       /* prendre l'expr a evaluer */
         eval();
         pop (a1);
         CVAL(a1) = a0;       /* affecter */
         return;
      case 13:     /* if */
         if (CDDR(CDDR(a0)) != nil)
            goto nargs_incorrect;
         a0 = CDR(a0);
         push(CDR(a0));     /* sauver (exp-vrai exp-faux) */
         a0 = CAR(a0);
         eval();
         pop(a1);
         a0 = (a0 == nil) ? CADR(a1) : CAR(a1);
         goto debut_eval;
      case 22:     /* cond */
         a0 = CDR(a0);
         for ( ;; )
         {
            if (a0 == nil)
                 return;            /* plus de clauses  */
            push (a0);              /* sauver les clauses */
            if ( !IS_CONS(a0) || !IS_CONS(CAR(a0)) )
            {
             obj_print (a0);
             putchar('\n');
             obj_print (CAR(a0));
             putchar('\n');
             err ("*** Les clauses doivent etre en listes\n");
            }
            a0 = CAAR(a0);          /* test de la 1ere */
            eval();
            pop(a1);                /* clauses dans a1 */
            if (a0 != nil)          /* test vrai ? */
            {
              a1 = CDAR(a1);        /* le reste de la clause dont le test
                                       est vrai dans a1 */
              if (a1 == nil)
                   return;          /* reste vide */
              a0 = a1;              /* reste non vide */
              progn();              /* progner le reste */
              return;
            }
            a0 = CDR(a1);           /* avancer dans les clauses */
         }
      default:
         if (CVAL(a4) == undefined)
            goto inconnu;          /* nom de fonction inconnu */
         a4 = CVAL(a4);            /* cas de variable en position fonct. ?? */
         goto evalcar;
      }  /* fin de switch(a4) */
   }     /* fin de if(IS_AT(a4)) */

   if (!IS_CONS(a4))
      err ("Elt inconnu en position fonctionnelle %d\n", a4);

   if (CAR(a4) == lambda)    /* la fonction est une liste */
   {
      /* on evalue les args qui sont ds le cdr de a0
         a4 = (lambda LARGS e1 ... eN)
         a0 = ((lambda LARGS e1 ... eN) arg1 ... argN)
       */
      push(a4);      /* sert dans evlis */
      push(a2);      /* sert dans evlis */
      push(a0);
      a0 = CDR(a0);
      evlis();
      a1 = a0;  /* ici : a0 = (eval(arg1) ... eval(argN)) */
      pop(a0);
      pop(a2);
      pop(a4);
      bind(CADR(a4), a1);  /* lier les args evalues de l'appel avec les
                              parametres de la lambda
                            */
      push(-2);            /* marqueur pour unbind() apres erreur */
      a0 = CDDR(a4);
      progn();
      pop(a1);           /* poper le marqueur -2 pour unbind() apres erreur */
      unbind();            /* on restitue les anciennes CVAL des parametres */
      return;
   }

   /* la fonction est une liste non lambda  --> PBS */
   err ("Liste non lambda en position fonctionnelle %d\n", a4);

   /* la fonction est un atome inconnu */
inconnu:
      err_s ("Fonction standard inconnue %s\n", p_names[a4]);
nargs_incorrect:
      err_s ("%s : Nb d'arguments incorrect\n", p_names[a4]);

}

progn()  /* suppose une liste d'expressions a evaluer dans a0
            valeur de la derniere dans a0 a ramener
          */
{
   a1 = a0;
   for ( ;; )
   {
      if (a1 == nil) return;   /* plus d'expressions a evaluer */
      push(a1);                /* sauver suite d'expressions */
      a0 = CAR(a1);            /* On evalue la 1ere */
      eval();
      pop(a1);                 /* suite d'expressions dans a1 */
      a1 = CDR(a1);            /* on y avance */
   }
}

evlis()     /* suppose une liste d'expressions (e1 e2 ... eN)
               dans a0, ramene (eval(e1) eval(e2) ... eval(eN))
               a4 = crochet principal
               a2 pointe sur le dernier CONS alloue
               le CAR de chaque CONS alloue contient  eval(eI)
             */
{
   a4 = a2 = cons(nil, nil);
   push(a4);
   for ( ;; )
   {
      if (a0 == nil) break;
      push(a0);
      push(a2);
      a0 = CAR(a0);
      eval();
      pop(a2);
      CDR(a2) = cons(a0,nil);
      a2 = CDR(a2);
      pop(a0);
      a0 = CDR(a0);
   }
   pop(a4);
   a0 = CDR(a4);
   uncons (a4);
}

bind(x,y)      /* x = liste de variables ; y = liste de valeurs
                  1. preserver les anciennes valeurs de variables
                  2. placer les valeurs dans les CVAL des variables
                */
int x, y;
{
int z;
   z = x;

   push(-1);           /* partie 1 */
   while (z != nil)
   {
      push (CVAL (CAR (z)));  /* pusher les anciennes valeurs */
      push (CAR (z));         /* pusher les adresses des variables */
      z = (CDR(z));
   }

   while (x != nil)    /* partie 2 */
   {
      CVAL( CAR( x)) = CAR (y);  /* affecter a la variable sa nvelle valeur */
      x = CDR(x);
      y = CDR(y);
   }
}

unbind()
{
int x, y;

   for( ;; )
   {
      pop(x);                  /* popper la variable */
      if (x == -1) return;     /* fin de deliaison */
      pop(y);                  /* popper la valeur */
      CVAL(x) = y;             /* restituer l'ancienne valeur */
   }
}


/* FIN de fichier */
