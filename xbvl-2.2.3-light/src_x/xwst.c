/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *        xwst.c
 *	Ernesto Sendoya
 *       janvier  91
 */

#include <vlisp.h>
#include <Xbvlisp.h>


/*  
 *	Sort la chaine dans l'interface passee en argument.
 *      S'il n'y a pas d'interface en argument 
 *      la chaine est sortie sur l'interface courante.
 *	NSBR0
 */

bVlispFonc xwst()       /* (xwst "string" { top { win }} ) */
{
  Int *at = car(a1);
  Widget w;
  CHAR *string;
  
  if((isWidget(at) && (w = GET_WIDGET_FROM_ATOME(at))
      && (XtIsSubclass((Widget)w, xbvlispWidgetClass)))){
    
    a1 = cdr(a1);
    if(get_oneName(&string) == False){
      a1 = nil;
      derec;
    }
    InsertStringXbvlisp(w, string);
    XtFree(string);
    a1 = t;
  } else a1 = nil;
  derec;
}


#if 0
/* 	wst affiche une chaine sur la fenetre lisp
 *		fait du scroll si necessaire
 * OPTIMISE PAR #DEFINE DANS VLISP.H (d.p)
 */

Int wst(string)
     CHAR *string;
{
  if(Xbvlisp_courr)
    InsertStringXbvlisp(Xbvlisp_courr, string);
  /*  XteLoop(); */
  return 0;
} 
#endif
/*
 * affiche la chaine dans la fenetre passee en parametre,
 * retourne la position du dernier caractere.
 * On suppose qu'il y a assez de place dans la fenetre
 * pour contenir la chaine.
 */
Int appendText(fenetre, debut, chaine)
     Widget fenetre;
     Int debut;
     CHAR *chaine;
{
  XtTextBlock text;
  Int fin, string_length;

  string_length = strlen(chaine);

  text.firstPos = 0;
  text.format = FMT8BIT;
  text.ptr = chaine;
  text.length = string_length;

  fin = debut + string_length;
  if(eXtTextReplace(fenetre,
		    (XtTextPosition) debut,
		    (XtTextPosition) fin, 
		    &text) == XawEditDone) 
    return fin;
  return -1;
}


/*
 *  	remplace un texte avec controle de l'operation
 *      ---------------------------------------------
 *                -pour debugger-
 */
eXtTextReplace(w, start_pos,  end_pos, text)
     Widget w;
     XtTextPosition start_pos, end_pos;
     XtTextBlock  *text;
{
  Int done;
  if(text->length < 0 || start_pos > end_pos ){
    printf("\nReplace Erreur:\tlength %d,  %s ", text->length, text->ptr);
    printf(":\t%d, %d\n", start_pos,end_pos);
    return XawEditError;
  }
  
  done = XtTextReplace(w, start_pos,  end_pos, text);
  
  switch (done){
  case XawEditDone :
    return XawEditDone;
  case XawPositionError:
    printf("\nXawPositionError:\t%d, %d, %s\n", start_pos,end_pos,text->ptr);
    return XawPositionError;
  case XawEditError : 
    printf("\nXawEditError:\t%d, %d, %s\n", start_pos,end_pos,text->ptr);
    return XawEditError;
    default :
      printf("XawBizarError :\n\tstart_pos==%d,end_pos== %d,done==%d,str== %s",
	     start_pos,end_pos,done,text->ptr);
    return XawEditError;
  }
  return done;
}

 
/*			PreparAppend
 *                      ------------          
 *  description : prepare la chaine a afficher
 *  args : 	     *un buffer pour recuperer la chaine
 *		     *la chaine a etre affichee.
 *		     *la colonne courante
 *		     *la taille de la ligne
 */

Int PreparAppend(cpy_buf, string, offset, cols)
     CHAR *cpy_buf, *string;
     Int offset, cols;
{

#define PADDING  (Int) ' '

  Int n, nolignes = 0, is_exacte = 0, i;
  Int place = cols - offset - 1;
  CHAR *p = string;

  while((n = next_mot(string)) != 0){
    if(n > 0){
      is_exacte = 0;
      if( n >= cols-1 ){ /* on est oblige de couper le mot */

	if(place <= 0) *cpy_buf++ = '\n', place = cols - 1, ++nolignes; /* nouvelle ligne */

	strncpy(cpy_buf, string, place);
	cpy_buf += place;
	string += place;

	*cpy_buf++ = '\n' ;
	++nolignes;
	place = cols - 1;
      }
      else if ( n > place ) { /* affichage a la ligne suivante */
	++nolignes;
	memset((void *) cpy_buf, PADDING, place);
	cpy_buf += place;
	*cpy_buf++ = '\n';	
	strncpy(cpy_buf, string, n);
	cpy_buf += n;
	string += n;
	place = cols - 1 - n;
      }
      else { /* cas normal */
	strncpy(cpy_buf, string, n);
	cpy_buf += n;
	string += n;
	if( n == place ) { /* arrive a la fin de ligne */
	  *cpy_buf = '\n';
	  ++cpy_buf;
	  ++nolignes;
	  place = cols - 1;
	  is_exacte = 1;
	}
	else place -= n;
      }
    }
    else {  
      /* traitement des mots speciaux
       * ici il faudrait un marquage des lignes 
       * reelles pour une re-exposition corretce. 	
       */
      Int l_motavant, spaces_tab ;
      switch(n){
      case RETOUR_LIGNE : /* return charriot */
	{/* si  plusieurs retours a la ligne qui se suivent is_exacte = 0 */
	  if(is_exacte && place == cols-1) is_exacte = 0;
	  else{
	    memset((void *) cpy_buf, PADDING, place);
	    cpy_buf += place;
	    *cpy_buf++ = '\n';	
	    place = cols - 1;
	    ++nolignes;
	  }
	  ++string; /* la ligne de la fenetre et de la chaine */
	}
	break;
      case TABULATION : /* une tabulation */
	l_motavant = 0;
	while(p < string && l_motavant <= TABSTOPS){
	  CHAR c = *(string - l_motavant - 1);
	  if(is_imprimable(c)&&!isspace(c)) ++l_motavant;
	  else break;
	}
	if(l_motavant < TABSTOPS) 
	  spaces_tab = (TABSTOPS - l_motavant);
	else spaces_tab = TABSTOPS;
	i = (place > spaces_tab) ? place : spaces_tab;
	memset((void *) cpy_buf, PADDING, i);
	cpy_buf += i;
	place -= i;
	++string;
	break;  
      default : /* pour l'instant rien d'autre */
	break;
      }
    }
  }
  *cpy_buf = 0;/* fin chaine */
  return nolignes;
}

Int next_mot( str )
     CHAR *str;
{
  Int n = 0;
  CHAR c;
  
  while(c = *str){
    switch(c){
    case '\n' :
      if(n) return n;
      else return RETOUR_LIGNE;
    case '\t' :
      if(n) return n;
      else return TABULATION;
    case ' ' :
    case '(' :
    case ')' :
      if(n) return n;
      return 1;
    case 0 :
      if(n) return n;
      else return 0; /* fin chaine */
    default:
      break;
    }
    ++n;
    ++str;
  }
  return n;
}
/*
 * travail d'initialisation du buffer d'edition
 * remplit de blancs et coupe les lignes.
 */
formatBuffer(buffer, lignes, cols)
     CHAR *buffer;
     Int lignes, cols;
{
  Int i;
  
  for(i = 0; i < lignes ; i++){
    memset((void *) (buffer + cols*i),(Int) ' ', cols-1);
    *(buffer + cols*i + cols-1) = '\n';
  }
  *(buffer + cols*lignes - 2) = ' ' ;/* le dernier retour a la ligne elimine */
  *(buffer + cols*lignes - 1) = 0 ;/* fin de chaine */
  return 0;
}
#if 0
/*
 * Le texte a afficher est trop grand,
 * formatage du texte pour remplacer le contenu de la fenetre.
 * Retourne la position du curseur.
 */
Int preparTrunque(trunque_buf, moislignes, cols)
     CHAR *trunque_buf;
     Int moislignes, cols;
{
  Int k = 0,n = 0;

  while(moislignes-->0)
    n += tailleNextLigne(trunque_buf);
  while(*(trunque_buf + n)) /* decalage de texte */
    *(trunque_buf + k++) = *(trunque_buf + n++);
  n = k; /* position du curseur */
  while(k%cols) /* remplissage */
    *(trunque_buf + k++) = ' ';
  return n;
}
#endif

