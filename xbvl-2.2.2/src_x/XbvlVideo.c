/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*  Des fonction Video du Widget D'interaction avec xbVLISP
 *  	         XbvlVideo.c
 *        Ernesto Sendoya, avril 92
 */

#include <vlisp.h> 

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <XbvlispP.h>
#include <stdio.h>

#ifdef STDC_HEADERS
Int make_cleolXbvl( XbvlispWidget wind,  CHAR **addr_buf);
Int make_cleosXbvl( XbvlispWidget wind,  CHAR **addr_buf);
Int xwleftXbvl(XbvlispWidget wind, Int times);
#endif

bVlispFonc cleol() {/*  (cleol win)   nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    make_cleolXbvl((XbvlispWidget)w, (CHAR **)NULL);
    a1 = t;
  }  else a1 = nil;
  derec;
}

bVlispFonc cleos() {/*  (cleos top  nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    make_cleosXbvl((XbvlispWidget)w, (CHAR **)NULL);
    a1 = t;
  } else a1 = nil;
  derec;
}

bVlispFonc poscur(){/*  (poscur [win] y x )  nsubr */
  XbvlispWidget bvl_w ;
  Int y, x, n;
  Int *at = car(a1);

  if(isnum(at) && (bvl_w = (XbvlispWidget) Xbvlisp_courr)) 
    ;    /* l'action se passe sur le Widget xbvlisp courant */
  else if(isWidget(at)
	  && (bvl_w = (XbvlispWidget)GET_WIDGET_FROM_ATOME(at))
	  && (XtIsSubclass((Widget)bvl_w, xbvlispWidgetClass))) {
    a1 = cdr(a1); /* avancer */ 
  } else {
    a1 = nil;
    derec;
  }
  if(get_ints(&y, &x, &n) != 2){
    a1 = nil;
    derec;
  }
  if(y < 0 || x < 0){ /* valeurs negatives ? */
    a1 = nil;
    derec;
  }
  setCursorPosition(bvl_w, y, x);
  a1 = t;
  derec;
}

bVlispFonc droite(){/*  (right win n )   nsubr */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    XbvlispWidget bvl_w = (XbvlispWidget)w;
    Int ebuf_length = bvl_w->xbvlisp.lignes * bvl_w->xbvlisp.pages * bvl_w->xbvlisp.colonnes;
    Int n, new_pos, pos = XtTextGetInsertionPoint(w);
    
    a1 = cdr(a1);
    if(isnil(a1))derec;
    if(init_oneINT(&n) != INTEGER)
      n  = 1;
    if(pos + n >= ebuf_length)
      new_pos = ebuf_length - 1;
    else new_pos = pos + n;
    
    XtTextSetInsertionPoint(w, (XtTextPosition)new_pos);
    a1 = cranb(new_pos - pos);
  } else a1 = nil;
  derec;
}

bVlispFonc shift_droite(){/*  (shift_right win n )   nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    XbvlispWidget bvl_w = (XbvlispWidget)w;
    Int n;
    
    a1 = cdr(a1);
    if(isnil(a1))derec;
    if(init_oneINT(&n) != INTEGER)
      n = 1;
    n = xwrightXbvl(bvl_w,n);
    if(n) a1 = cranb(n);
    else a1 = nil;
  }
  else  a1 = nil;
  derec;
}
/*
 * positionne le curseur, et efface le caractere, a gauche.
 * retourne nil si echec, 
 * retourne la nouvelle position si l'operation est correcte.
 */
bVlispFonc gauche(){/*  (left win)   nsubr  */
  Int *at = car(a1);
  Int pos;
  Widget w;

  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))
     && (pos = XtTextGetInsertionPoint(w))) {
    Int n, new_pos;
    
    a1 = cdr(a1);
    if(isnil(a1))derec;
    if(init_oneINT(&n) != INTEGER)
      n = 1;
    if(pos - n < 0)
      new_pos = 0;
    else new_pos = pos - n;
    
    XtTextSetInsertionPoint(w, (XtTextPosition)new_pos);
    a1 = cranb(new_pos);
  }   else a1 = nil;
  derec;
}
/* 
 * decale le texte a gauche du curseur.
 * les derniers caracteres de la ligne seront perdus.
 */
bVlispFonc shift_gauche(){/*  (left win)   nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    Int n;
    
    a1 = cdr(a1);
    if(isnil(a1))derec;
    if(init_oneINT(&n) != INTEGER)
      n =  1;
    if((n = xwleftXbvl((XbvlispWidget)w, n)) > 0)
      a1 = cranb(n);
    else a1 = nil;
  }  else a1 = nil; 
  derec;
}
/* 
 * Efface le caractere a gauche du curseur.
 *          Pareille que backspace.
 */
Int xwbackXbvl(wind)
     XbvlispWidget wind;
{
  Int pos, col_courr;

  if((pos =  XtTextGetInsertionPoint((Widget)wind))
     && (col_courr = pos % wind->xbvlisp.colonnes)){ /* si caractere a gauche */
    XtTextSetInsertionPoint((Widget)wind,(XtTextPosition)pos - 1); /* reculer */
    return xwleftXbvl(wind, 1);                                    /* effacer */
  }
  return 0; /* rien a effacer */
}
  
/* extension des capacites video X */
/* efface le caractere a gauche du curseur de la fenetre nommee */
bVlispFonc back(){/*  (backspace win)   nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    if(xwbackXbvl( (XbvlispWidget)w, 1) )
      a1 = t;
    else a1 = nil;
  }  else a1 = nil;
  derec;

}

/*
 * retourne une liste avec la position du curseur (y x).
 */
bVlispFonc cursorpos(){/* (cursorpos [win]) nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if( (isnil(a1) && (w = Xbvlisp_courr))
     || (isWidget(at) && (w = GET_WIDGET_FROM_ATOME(at))
	 && (XtIsSubclass(w, xbvlispWidgetClass))) ){
    Int  ligne_courr, col_courr;
    getCursorPosXbvl((XbvlispWidget)w, &ligne_courr, &col_courr);
    a1 = cons(cranb((Int)ligne_courr), cons(cranb((Int)col_courr), nil));
  } else a1 = nil;
  derec;
}
     /*----------------------*/
     /* Mouvement du curseur */
     /*----------------------*/
/*
 * curseur une ligne en bas
 */  

bVlispFonc bas(){/*  (down win)   nsubr */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    XbvlispWidget bvl_w = (XbvlispWidget)w;
    Int pos = XtTextGetInsertionPoint(w);
    Int ligne_courr = pos/bvl_w->xbvlisp.colonnes;

    if(ligne_courr + 1 < bvl_w->xbvlisp.lignes * bvl_w->xbvlisp.pages){
      pos += bvl_w->xbvlisp.colonnes;
      XtTextSetInsertionPoint(w, (XtTextPosition)pos);
      a1 = t;
    } else a1 = nil;
  }  else a1 = nil;
  derec;
}
      
/*
 * curseur la ligne en haut
 */

bVlispFonc haut(){/*  (up win)   nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    XbvlispWidget bvl_w = (XbvlispWidget)w;
    Int  pos = XtTextGetInsertionPoint(w);
    Int ligne_courr = pos/bvl_w->xbvlisp.colonnes;

    if(ligne_courr > 0  && bvl_w->xbvlisp.lignes * bvl_w->xbvlisp.pages > 0){
      pos -= bvl_w->xbvlisp.colonnes;
      XtTextSetInsertionPoint(w, (XtTextPosition)pos);
      a1 = t;
    } else a1 = nil;
  }  else a1 = nil;
  derec;
}
/*
 * curseur au debut du texte
 */
bVlispFonc home(){/*  (home win)   nsubr  */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    XtTextSetInsertionPoint(w, (XtTextPosition)0);
  } else a1 = t;
  derec;
}

/*---------------------------------------------------------------
 * fait de scrolling de "n_lignes" a partir de la ligne
 * "dici", la position du curseur est restituee.
 ---------------------------------------------------------------*/
bVlispFonc xscroll()/* (xscroll win dici n_linges ) nsubr */
{
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    Int n_lignes, dici, n;
    Position pos;
    
    a1 = cdr(a1);
    if(get_ints( &dici, &n_lignes, &n) != 2){
      a1 = nil;
      derec;
    }
    /* sauvegarder la position du curseur */
    pos  =  XtTextGetInsertionPoint(w);
    scrollTextXbvlisp (w, dici, n_lignes); 
    /* restituer la position du curseur */
    XtTextSetInsertionPoint(w, pos);
    a1 = t;
  }  else a1 = nil;
  derec;
}


/****************************auxiliaires*************************************/

/*
 * ramene la position du curseur, en caracteres,  d'un Widget Xbvlisp.
 */

void getCursorPosXbvl(w, _ligne_courr, _col_courr )
     XbvlispWidget w;
     Int * _ligne_courr, * _col_courr;
{
  XbvlispWidget wbvl = (XbvlispWidget)w;
  XtTextPosition top_pos, pos;
  top_pos = XtTextTopPosition((Widget)wbvl);
  pos = XtTextGetInsertionPoint((Widget)wbvl);
  
  pos -= top_pos;
  *_ligne_courr = pos / wbvl->xbvlisp.colonnes;
  *_col_courr =  pos % wbvl->xbvlisp.colonnes;
}
/*
 * positionne le curseur d'un Widget Xbvlisp.
 */

void setCursorPosition(w, ligne, col)
     XbvlispWidget w;
     Int  ligne;
     Int col;
{
  Int t_lignes =  w->xbvlisp.lignes*w->xbvlisp.pages;
  Int ligne_courr = (ligne >= t_lignes ) ? t_lignes - 1 : ligne ;
  Int col_courr = (col >= w->xbvlisp.colonnes ) ? w->xbvlisp.colonnes - 2 : col;

  XtTextSetInsertionPoint((Widget)w, (XtTextPosition) ligne_courr * w->xbvlisp.colonnes + col_courr);
}
/*
 * Ramene la position du curseur et de la widget d'interaction
 * en pixel.
 */
Boolean getCursorCoordsXbvl(wind , x_courr, y_courr, root_x, root_y)
     Widget wind;
     Position *y_courr, *x_courr, *root_y, *root_x;
{
  Position  x, y;
  Int col_courr, ligne_courr;
  XFontStruct *font = NULL;
    

  /*
   * Position du curseur dans la fenetre de l'interface
   */
  
  /* en caracteres */
  getCursorPosXbvl((XbvlispWidget)wind, &ligne_courr, &col_courr);
  
  if(font || getWidgetFont((Widget)wind, &font)){
    
    /* voila les pixels */
    *x_courr = x = col_courr  * FONTWIDTH(font);
    *y_courr = y = ligne_courr * FONTHEIGHT(font);
    
    /*
     * translate coordinates in application top-level window
     * into coordinates from root window origin.
     */
    XtTranslateCoords(wind,                       /* le Widget */
		      x, y,                  /* coors dans l'interface */
		      root_x, root_y);     /* coords dans la racine principal */
    return True;
  }
  return False;
}
/* 
 * Le texte a partir de la position du curseur a la fin de la ligne
 * est decale vers la gauche de <times> caracteres.
 * Les caracteres a droite du curseur sont perdus.
 */

Int xwleftXbvl(wind, times)
     XbvlispWidget wind;
     Int times;
{
  Int pos, col_courr, rien_afaire,k,n = 0;
  CHAR adecaler[512], *editbuf, *ptr_deb;
  
  if(times < 1) return 0;
  
  pos =  XtTextGetInsertionPoint((Widget)wind);
  col_courr = pos % wind->xbvlisp.colonnes;

  GetXbvlispString(wind, editbuf);
  ptr_deb =  editbuf + pos;
  
  rien_afaire = 1;
  while(*ptr_deb && *ptr_deb != '\n'){
    n += 1;
    if(*ptr_deb != ' '){
      rien_afaire = 0;
      break;
    }
    ptr_deb += 1;
  }
  if (rien_afaire || n == 0) return 0;

  ptr_deb =  editbuf + pos;
  n =  wind->xbvlisp.colonnes -  col_courr - 1;

  times = (times > n) ?  n : times;
  /* times decalages et remplissages a faire */
  n = n - times;
  if(n)
    strncpy(adecaler, ptr_deb + times, n);
  k = 0;
  while(k < times)
    *(adecaler + n + k++ ) = ' ';
  *(adecaler + n + k) = 0;

  appendText((Widget)wind, pos, adecaler);
  return times;
}

/* 
 * Le texte a partir de la position du curseur a la fin de la ligne
 * est decale ver la droite de <times> caracteres.
 * Les caracteres le plus a droite du curseur sont perdus.
 */
Int xwrightXbvl(wind, times)
     XbvlispWidget wind;
     Int times;
{
  Int pos, col_courr, rien_afaire,k, n = 0;
  CHAR adecaler[512], *editbuf,  *ptr_deb;
  
  if(times < 1) return 0;

  pos =  XtTextGetInsertionPoint((Widget)wind);
  col_courr = pos % wind->xbvlisp.colonnes;
  GetXbvlispString(wind, editbuf);
  ptr_deb =  editbuf + pos;
  
  rien_afaire = 1;
  while(*ptr_deb && *ptr_deb != '\n'){
    n += 1;
    if(*ptr_deb != ' '){
      rien_afaire = 0;
      break;
    }
    ptr_deb += 1;
  }
  if (rien_afaire || n == 0) return 0;

  ptr_deb = editbuf + pos;
 
  n =  wind->xbvlisp.colonnes -  col_courr - 1 ;

  /* un decalage et un remplissage a faire */
  times = (times < n) ? times : n;
  k = 0;
  while(k < times)
    *(adecaler + k++) = ' ';
  k = n - times ;
  if(k)
    strncpy(adecaler + times, ptr_deb , k);
  *(adecaler + n) = 0;

  appendText((Widget)wind, pos, adecaler);
  return times;
}

/* 
 * Efface le contenu de la fenetre jusqu'a la fin du texte.
 */

Int make_cleosXbvl(wind, addr_buf) 
     XbvlispWidget wind;
     CHAR ** addr_buf;
{
  return clean_to_addrXbvl(wind, 
			   wind->xbvlisp.lignes * wind->xbvlisp.colonnes * wind->xbvlisp.pages,
			   addr_buf);
}

/* 
 * Efface le contenu de la fenetre jusqu'a la fin de la ligne.
 */

Int make_cleolXbvl(wind, addr_buf)
     XbvlispWidget wind;
     CHAR **addr_buf;
{
  Int end, ligne_courr, pos;
  
  pos =  XtTextGetInsertionPoint((Widget)wind);
  ligne_courr = pos / wind->xbvlisp.colonnes;
  end = (ligne_courr + 1)* wind->xbvlisp.colonnes - 1;
  return clean_to_addrXbvl(wind, end, addr_buf);
}

/*
 * Efface le texte de la fenetre passee en
 * parametre jusqu'a la position end.
 */

Int clean_to_addrXbvl(wind, end, addr_buf)
     XbvlispWidget wind;
     CHAR **addr_buf;
     Int end;
{

  CHAR *ptr, *editbuf ;
  Int  cars,pos;

  GetXbvlispString(wind, editbuf);
  pos =  XtTextGetInsertionPoint((Widget)wind);

  if(pos >= end) return 0;

  ptr = editbuf + pos; /* le texte va etre efface a partir d'ici */
  if(cars = netoy_text(ptr, end - pos, False)){
    CHAR *btmp;
    XtTextBlock text;

    if(addr_buf) save_texte(addr_buf, ptr, end - pos);
    btmp = XtNewString(ptr);
    netoy_text(btmp, end - pos, True);
    
    text.firstPos = 0;
    text.ptr = btmp;
    text.format = FMT8BIT;
    text.length = end - pos ;
    eXtTextReplace((Widget)wind , pos,  end , &text);
    XtFree(btmp);
  }
  else if(addr_buf) *addr_buf = NULL;
  return cars;
}
#if 0
/*
 * enleve tous les caracteres differents du blanc et du retour.
 * si flag == True; les compte seulement si flag == False
 */
Int netoy_text(ptr, length, mode)
     CHAR *ptr;
     Int length, mode;
{
  Int n, cnt;
  for(cnt = n = 0; n < length; ptr++, n++)
    if(*ptr == '\n'|| *ptr == ' ') ; /* on ne touche pas */
    else {
      cnt += 1;
      if(mode == True)	*ptr = ' ';
    }
  return cnt;
}

/*
 *  Prepare le kill buffer.
 *  Une limite imposee de BUFSIZ (512-1024) caracteres.
 */
Int save_texte(abuf, ptr, length)
     CHAR **abuf;
     CHAR *ptr;
     Int length;
{
  Int n;
  CHAR buffer[BUFSIZ + 1], *new;

  if(length <= 0) return 0;
  if(length > BUFSIZ) length = BUFSIZ;
  strncpy(buffer, ptr, length);
  
  for(n = 0; n < length;  n++)
    if(buffer[n] == '\n') 
      buffer[n] = ' ' ; /* on ne sauve pas les retours */
  while(n > 0)
    if(buffer[n - 1] == ' ') 
      n -= 1;
    else break;
  buffer[n] = 0;
  if(n){ /* qqc. a sauver */
    new = XtNewString(buffer);
    *abuf = new;
  }
  return n;
}
#endif

/******************************************************************************/


