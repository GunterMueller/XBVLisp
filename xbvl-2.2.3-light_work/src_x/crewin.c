/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *                    crewin.c
 * les fonctions de base VLISP pour la creation des fenetres X
 *             (creation des fenetres X )
 *                  Ernesto Sendoya
 *                    janvier  91                                      
 */
#include <Objets.h>


/*
 * Fonction General de creation de widgets.
 */

Int xcreeWidget ()  /* (xcreeWidget nom  classe widgetPere {  ressource1 ... )  nsubr*/
{
  Int n, *ato;
  CHAR *nom, *wclasse;
  Widget newWidget = NULL, w_pere;
  WidgetClass widget_class;
  ArgList argg;
  struct atome *rep_atome;
  
  
  if(get_noms(&nom,&wclasse,&n) != 2){
    if(n == 1)XtFree(nom);
    a1 = nil;
    derec;
  }
  
  /* quelle classe de  Widget? */
  if((widget_class = getWidgetClassFromString(wclasse)) == NULL){
    a1 = nil;  XtFree(nom); XtFree(wclasse); 
    derec;
  }
  if(widget_class == applicationShellWidgetClass){ /* une racine de hierarchie */
    getArgList(NULL, widget_class, &argg, &n); /* on reccupere des eventuels ressources */
    if(!(newWidget =  XbvlCreateApplicationShell(nom, XBVL_APP_CLASSE_NAME, argg, n , &rep_atome))){
      XtFree(nom); XtFree(wclasse);
      a1 = nil;
      derec;
    }
    a1 = (Int *)rep_atome;
    if(*atcareful==(Int)t && (pbind>0||*atcrewid!=(Int)nil)) {
      /* D.P */
      /* on met la fonction creatrice dans la PListe de l'atom genere */
      if(!pbind) a5= (Int *)*atcrewid;
      else{a5= (Int *)*(pbind-2); a5 = (Int *)*(a5+2);}
      adapteival(atcrewid,a1,a5);
    }
    XtFree(nom); XtFree(wclasse);
    derec;
  }
  /* dans a1 le widget pere ? */
  ato = car (a1);

  if(isWidget(ato))  w_pere = GET_WIDGET_FROM_ATOME(ato);
  else { 
    XtFree(nom); XtFree(wclasse);
    a1 = nil;
    derec;
  }
  /* il ne nous reste que faire la creation du nouveaux widgette */
  a1 = cdr(a1);			/* en avancant pour */
  getArgList(XtClass(w_pere), widget_class, &argg, &n); /* en reccuperant des eventuels ressources */

  if(widget_class == xbvlispWidgetClass) /* un Widget d'interaction avec xbVLISP */
       newWidget = CreateManagedXbVlispWidget(nom,            w_pere, argg, n, &rep_atome);
  else newWidget = XbvlCreateManagedWidget(nom, widget_class, w_pere, argg, n, &rep_atome);

  if(!newWidget){		/* DEBUG */
    printf("ERRor, Impossible de creer le Widget <%s>\n", nom);
    XtFree(nom); XtFree(wclasse);
    a1 = nil;
    derec;
  }

  if(argg && n){		/* on libere la place ? */  XtFree((String)argg); }
  XtFree(nom); XtFree(wclasse);

  /* un nouveau widget */
  interneObjetWidget(rep_atome, newWidget);
  a1 = (Int *)rep_atome;

  if(*atcareful==(Int)t && (pbind>0||*atcrewid!=(Int)nil)) {
    /* D.P */
    /* on met la fonction creatrice dans la PListe de l'atom genere */
    if(!pbind) a5= (Int *)*atcrewid;
    else{a5= (Int *)*(pbind-2); a5 = (Int *)*(a5+2);}
    adapteival(atcrewid,a1,a5);
  }
  derec;
}


#if 0
/* :::: PLUS UTILISE (optimisation) ::::: (D.P)
 * retourne l'atome avec le nom passe en parametre
 * s'il existe, nil si non.
 * on le reccupere directement de la hash table 
 * si le hash-coding fonctione.
*/
Int * GetAtome(name)
     CHAR *name;
{
  struct atome *att = lookfor_atome(name);
  return att ? (Int *)att : nil;
}
#endif
/*
 * return un element dont la chaine passee
 * en argument est la representation externe.
 * Si l'element est un atome qui n'existe pas l'atome est cree.
 */
Int * GetListElement(name)
     CHAR * name;
{
  Int *att;
  
  if((att = (Int *)lookfor_atome(name)) != NULL) return att;
  if(isString(name)){
    genstring(name);
    return a1;
  }
  if(isInteger(name))
    return cranb(atoi(name));
#ifndef NO_VFLO
  if(isFloat(name))
    return craflo(atof (name));
#endif
  /* DEBUGG */
  printf( "%s :Element Non Trouve\n", name);
  return genat(name); /* on cree un atome */
}



/* 
 * retrouve ou genere un atome du nom passe en argument
 * l'atome est place dans a5
 */
Int * genat(name) 
     CHAR *name;
{
  extern Int DEJA ;

#if 0

/* #ifdef HASH_CODING */
  /* une petite optimisation */
  if(a5 = (Int *)lookfor_atome(name)){
    DEJA = 1;
    return a5;
  }
/* #endif  */

  a5=nil; 
  DEJA = 0;
  if (a5 = cherchat(oppn)){
    DEJA = 1;
    ppn=oppn;
    return a5;
  }
  return (a5 = creatom(oppn));
#endif

  if(a5 = (Int *) lookfor_atome_hash(name, &DEJA)) {DEJA=1; return a5;}
  else {
    newstr(name);
    if(ppn>hstr) gcstrsv();
    a5=(Int *)creatom_hash(oppn, DEJA);
    DEJA = 0;
  }
  return a5;
}

