/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *                  aide.c
 *  		aidvlisp	Michel Saint-Tourens
 *			version Patrick Greussay, 10 Novembre 1982
 *			adaptation: Harald Wertz, 7 Avril 1983 
 *			Ernesto Sendoya : interface X dec. 1990
 */

#include <vlisp.h>
#include <xwin.h>
#include <Xbvlisp.h>
#include <aide.h>
#include <xaide.h>
#include <vlisp_files.h>

LigneAide tt;

int nbs;
Int cmpsujs();


/* fonctions */
#ifndef STDC_HEADERS
Int aide(); /* nsubr */
void select_aide();  /* action */
void liste_aide();   /* action */
void enleve_aide();  /* action */
#endif

/* 

 */
bVlispFonc aide()  /* (aide {{sujet} {widget} {ficher-d'aide}}) nsubr */
{
  Int fd,i;
  String *   sim_list, * full_list, name ;

  a2 = cdr(a1);
  a3 = car(cdr(a2)); /* fichier d'aide */
  a2 = car(a2);      /* widget de sortie */
  a1 = car(a1);      /* topic */

  if(a3 != nil) { /* specification du nom de l'aide */
    push(Aide_src);
    push(Aide_index);
    push(Aide_section);
    if(isatom(a3)) a3 = ((ATOMEPTR) a3)->pn + (2/2);
    else a3 = a3+(2/2);
    Aide_src = strcpy(XtMalloc(strlen((char *)a3)+5), (char *)a3);      strcat(Aide_src, ".dat");
    Aide_index = strcpy(XtMalloc(strlen((char *)a3)+6), (char *)a3);    strcat(Aide_index, ".:dat");
    Aide_section = strcpy(XtMalloc(strlen((char *)a3)+6), (char *)a3);  strcat(Aide_index, ".:sec");
  }
  
  if((fd = OPENINDEX()) == 0 ) {
    wst("Impossible d'ouvrir le fichier d'aide.\n"); XBell(display, 99); XBell(display, 99);
    goto fin_aide;
  }
  
  if(!isWidget(a2) && *ataide != (Int)nil) /* gestion de l'aide par la spécification d'une fonction lisp */
    { a1 = cons((Int *)*ataide, a2); pc=eval; return; }

  if(isnil(a1)){  /* aide sans arguments */
    /* on cree la liste de tous les sujets d'aide */
    if(full_list = initListAide(fd, NULL)){
      createListAide(full_list);
      a1 = t;
    }
    else{
      wst("Aucun SUJET trouvee, aide impossible\n");
      a1 = nil;
    }
    close(fd);
    goto fin_aide;
  }
  /* un deuxime argument ? */
  new_aide_o = new_list_o = NULL;
  if(isWidget(a2)) {  /* changer la sortie des infos. */
    Widget w = GET_WIDGET_FROM_ATOME(a2);
    if(XtIsSubclass(w, xbvlispWidgetClass))
      new_aide_o = w;
    else if(XtIsSubclass(w, listWidgetClass))
      new_list_o = w;
  }

  
  /*  le nom du sujet(s) demandes  */
  if(isatom(a1)) name = STRPNAME(a1);
  else if(isustr(a1)) name = (String) (a1 + 2/2);
  else {
    wst("je n'ai pas d'aide pour ce type d'objets.\n");
    close(fd);
    goto fin_aide;
  }

  if(*name == '~'){
    if(*(name + 1)){ /* de sujets similaires */
      if(sim_list = initListAide(fd, (name + 1))){
	createListAide(sim_list);
	a1 = t;
      }
      else {
	wst("Il n'y a pas des sujets similaires disponibles.\n");
	a1 = nil;
      }
    }
    close(fd);
    goto fin_aide;
  }
  read(fd,&nbs,sizeof(int)); /* no d'objets ? */
  for(i = 0; read(fd,&tt,sizeof(tt)) > 0; i++) {
    *(index(tt.ligne,'\n'))='\0';
    if(strcmp(tt.ligne, name) == 0){
#ifndef XtSpecificationRelease
      XtListReturnStruct objetbid;
      objetbid.index = i;
#else
      XawListReturnStruct objetbid;
      objetbid.list_index = i;
#endif
      objetbid.string = name;
      close(fd);
      affObjet(NULL,0,&objetbid);
      a1 = t;
      goto fin_aide;
    }
  }
  wst("Il n'y a pas encore d'aide a ce sujet.\n"); 
  a1 = nil;
  close(fd);
fin_aide:
  if(a3 != nil) { 
    XtFree(Aide_src);
    XtFree(Aide_section);
    pop_cast(Aide_section, (CHAR*));
    pop_cast(Aide_index, (CHAR*));
    pop_cast(Aide_src, (CHAR*));
  }
  derec;
}

bVlispFonc section() /* (sections {fichier d'aide}) subr1 */
{
  int fd, n_sec, i;
  LigneAide s;

  if(!isnil(a1) && isustr(a1)) {
    a1 += 2/2;
    a3 = t;
    push(Aide_section);
    Aide_section = strcpy(XtMalloc(strlen((char *)a1)+6), (char *)a1);  strcat(Aide_index, ".:sec");
  }
  else a3 = nil;

  if(!(fd = OPENSECTION())) {
    wst("Impossible d'ouvrir le fichier de sections ");  wst(Aide_section); wst("\n");
    pop_cast(Aide_section, (CHAR*));
    a1 = nil;
    derec;
  }
  a1 = a2 = nil;
  read(fd, &n_sec, sizeof(int));
  while(read(fd, &s, sizeof(s)) > 0) {
    if(s.ligne[strlen(s.ligne)-1] < 32) s.ligne[strlen(s.ligne)-1] = '\0';
    newstr(s.ligne);
    if(isnil(a1)) a2 = a1 = cons((Int*)oppn, nil);
    else {
      *(a2+1) = (Int)cons((Int*)oppn, nil);
      a2 = cdr(a2);
    }
  }
  close(fd);

  if(a3 == t) {
    pop_cast(Aide_section, (CHAR*));
    XtFree(Aide_section);
  }
  derec;
}

bVlispFonc listesection() /* (listesection num|nom [fichier]) */
{
  int fd_sec, fd_ind, n_sec, i, b_pos = 0, h_pos = 0, s_num = -1;
  char *s_name = NULL;
  Int *tete_liste;
  LigneAide s;

  if(isnil(a1)) derec;
  if(isnum(a1)) s_num = valnb(a1);
  else if(isustr(a1)) s_name = STR_FROM_STRLSP(a1);
  
  if(a2 != nil) {
    push(Aide_index);
    push(Aide_section);
    if(isatom(a2)) a2 = ((ATOMEPTR) a2)->pn + (2/2);
    else a2 = a2+(2/2);
    Aide_index = strcpy(XtMalloc(strlen((char *)a2)+6), (char *)a2);    strcat(Aide_index, ".:dat");
    Aide_section = strcpy(XtMalloc(strlen((char *)a2)+6), (char *)a2);  strcat(Aide_index, ".:sec");
  }
  
  if(!(fd_sec = OPENSECTION()) ||
     !(fd_ind = OPENINDEX())) {
    wst("Impossible d'ouvrir le fichier d'aide.\n"); XBell(display, 99); XBell(display, 99);
    a1 = nil;
    if(a2 != nil) { XtFree(Aide_section); pop_cast(Aide_section, (CHAR*)); XtFree(Aide_index); pop_cast(Aide_index, (CHAR*)); }
    derec;
  }
  
  read(fd_sec, &n_sec, sizeof(int));
  for(i = 0; read(fd_sec, &s, sizeof(s)) > 0; i++) 
    if(i == s_num || (s_name && !strncmp(s_name, s.ligne, strlen(s_name)))) {
      b_pos = s.pos;
      if(read(fd_sec, &s, sizeof(s)) > 0) h_pos = s.pos;
      else h_pos = -1;
      break;
    }

  a1 = nil;
  if(b_pos > 0) {
    read(fd_ind, &n_sec, sizeof(int));
    while(read(fd_ind, &s, sizeof(s)) > 0) {
      if(h_pos > 0 && s.pos >= h_pos) break;
      if(s.pos >= b_pos) {
	if(s.ligne[strlen(s.ligne)-1] < 27) s.ligne[strlen(s.ligne)-1] = '\0';
	newstr(s.ligne);
	/* tri par insertion... */
	if(a1 == nil) a1 = tete_liste = cons((Int *)oppn, nil);
	else if(strcmp((char *) STR_FROM_STRLSP(car(a1)),(char*)(oppn+sizeof(Int))) >= 0) a1 = cons((Int *)oppn,a1);
	else {
	  for(tete_liste = a1; cdr(tete_liste) != nil &&
		strcmp((char *) STR_FROM_STRLSP(car(cdr(tete_liste))), (char *) (oppn + sizeof(Int))) < 0; 
	      tete_liste = cdr(tete_liste));
	  *CDR(tete_liste) = (Int)cons((Int *)oppn,cdr(tete_liste));
	}
      }
    }
  }
  close(fd_sec);
  close(fd_ind);
  if(a2 != nil) { XtFree(Aide_section); pop_cast(Aide_section, (CHAR *)); XtFree(Aide_index); pop_cast(Aide_index, (CHAR*)); }
  derec;
}

/* 
 * action:
 * affiche les informations existantes sur le premier mot
 * d'une selection.
 ARGSUSED*/
void select_aide( w, event, params, num_params)
     Widget w;
     XKeyEvent *event;
     String *params;
     Cardinal * num_params;
     
{
  Int fd;
  Int i,nb;
  CHAR *editbuf, *ptr, mot[81], c;
  XtTextPosition    pos1 = 0, pos2 = 0;
  XtTextGetSelectionPos(w, &pos1, &pos2);
  
  if(pos1 >= pos2) return;
  
  XtTextUnsetSelection(w);
  XtTextInvalidate(w, pos1, pos2);

  /*
  dans la config. de widget Texts, avec la relaise 4 ca ne marche pas !  
  XteGetValue(w, XtNstring, &editbuf);
  */
  
  /*   editbuf = interface_courr->text.buffer; */
  ptr = editbuf + (unsigned int)pos1;

  /* bien se positionner sur le mot selectionne */
  for ( ; pos1 < pos2 && ((c = *ptr) == ' ' || c == '(' || c == ')');
       pos1 += 1)
    ptr += 1;
  /* copier le mot selectionne */
  for ( i = 0; 
       (pos1 + i)< pos2 && i < 80 &&
       (c = *ptr) && c != ' ' && c != '\n' && c != '(' && c != ')';
       i += 1, ptr += 1)
    *(mot + i) = c;
  *(mot + i) = 0;

  nb= 0 ;

  if((fd = OPENINDEX()) == 0 ) {
    wst("J'arrete.\n");
    return;
  }

  read(fd,&nbs,sizeof(int)); /* no d'objets ? */
  if(isatom(a1)) a1=((struct atome  *)a1)->pn;
  for(i = 0; read(fd,&tt,sizeof(tt)) > 0; i++) {
      *(index(tt.ligne,'\n'))='\0';
      if(strcmp(tt.ligne, mot) == 0){
	XtListReturnStruct objetbid;
#ifndef XtSpecificationRelease
	objetbid.index = i;
#else	
	objetbid.list_index = i;
#endif
	objetbid.string = mot;
	close(fd);
	affObjet(NULL,0,&objetbid);
	return;
      }
    }
  close(fd);
  return;
}

/* action: pour pouvoir declencher l'aide a partir du menu
 * principale.
 * liste les sujets d'aide et donne acces aux informations.
 ARGSUSED*/
void liste_aide(w, event, params, num_params) 
     Widget w;
     XKeyEvent *event;
     String * params;
     Cardinal * num_params;
{
  String *  full_list ;
  Int nb;
  Int fd;
  
  nb= 0 ;

  if((fd = OPENINDEX()) == 0 ) {
    wst("Impossible d'ouvrir le fichier d'aide.\n");
    return;
  }
  if(*ataide != (Int)nil) { wst("Pour lancer "); wst(STRPNAME(*ataide)); wst(" tapez ("); wst(STRPNAME(*ataide));  wst(")\n"); }

  /* creer le widget d'aide */
  if(full_list = initListAide(fd, NULL))
    createListAide(full_list);
  else wst("Aucun SUJET trouvee, aide impossible\n");
  close(fd);
  return;
}

