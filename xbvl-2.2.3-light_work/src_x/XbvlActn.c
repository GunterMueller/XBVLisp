/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*  Des actions pour le widget de communication avec xbVLISP.
 *  	         XbvlActn.c
 *     pour le Widget D'interaction avec xbVLISP
 *        Ernesto Sendoya, avril 92
 */

#include <vlisp.h> 

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <XbvlispP.h>
#include <stdio.h>


/*******************************************************************/
/*                                                                 */
/*        ACTIONS Specifiques du Widget D'Interaction              */
/*    un widget dedie a la communication avec l'interprete         */
/*                                                                 */
/*******************************************************************/
/*
 * le caractere a droite du curseur est efface
 */
/* ARGSUSED */
void delete_next_characterXbvl( w, event, params, num_params )
     XbvlispWidget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  xwleftXbvl(w , 1);
}

/*
 * l'atome a droite du curseur est efface
 */
/* ARGSUSED */
void delete_next_atomeXbvl( w, event, params, num_params )
     XbvlispWidget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  CHAR *editbuf,c;
  Int pos, ref, rien_afaire, n;
  
  ref = pos = (Int)  XtTextGetInsertionPoint((Widget)w);
  GetXbvlispString(w, editbuf);
  
  for( rien_afaire = 1;
      (c = *(editbuf + pos)) != '\n' &&  c ;
      pos += 1){
    if(c != ' '){
      if(rien_afaire) rien_afaire = 0;
      else if(c == ')' || c == '(' ) break;
    }
    else if(!rien_afaire) break;
  }
  if(rien_afaire) return ;
  n = pos - ref;
  xwleftXbvl(w , n);
}

/*
 * l'atome a gauche du curseur est efface
 */
/* ARGSUSED */
void delete_prev_atomeXbvl( w, event, params, num_params )
     XbvlispWidget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  CHAR *editbuf,c;
  Int pos, ref, rien_afaire, n;

  ref = pos =  (Int)  XtTextGetInsertionPoint((Widget)w);
  GetXbvlispString(w, editbuf);
  
  for( rien_afaire = 1;
      (c = *(editbuf + pos)) != '\n' &&  pos > 0 ;
      pos -= 1){
    if(c != ' '){
      if(rien_afaire) rien_afaire = 0;
      else if(c == ')' || c == '(' ) break;
    }
    else if(!rien_afaire)break;
  }
  if(rien_afaire) return ;
  
  if((c = *(editbuf + pos)) == '\n') 
    pos += 1;
  if(*(editbuf + ref) != ' ' && c != '(' && c != ')' && c != '\n') 
    pos += 1;
  
  if(n = ref - pos){
    XtTextSetInsertionPoint((Widget)w, (XtTextPosition) pos);
    xwleftXbvl(w , n);
  }
}

/*
 * efface le caractere a droite du curseur.
 */

/* ARGSUSED */
void delete_previous_characterXbvl( w, event, params, num_params )
     XbvlispWidget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  Int n;
  if(*num_params == 1)
    n = atoi(params[0]);
  else n = 1;
  xwbackXbvl(w, n);
}

/* ARGSUSED */
void shift_car_rightXbvl( w, event, params, num_params )
     XbvlispWidget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  Int n;
  if(*num_params == 1)
    n = atoi(params[0]);
  else n = 1;
  xwrightXbvl(w,  n);
}


/* ARGSUSED */
void kill_to_end_of_lineXbvl(w , event, params, num_params)
     XbvlispWidget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  
  if(w->xbvlisp.killbuf)XtFree(w->xbvlisp.killbuf); 
  w->xbvlisp.killbuf = NULL;
  make_cleolXbvl(w, &w->xbvlisp.killbuf);
}

/* a revoir modifier, le mecanisme de selections doit faire l'affaire */
/* ARGSUSED */
void unkill_der_killXbvl(wd, event, params, num_params)
     Widget wd;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  XbvlispWidget w = (XbvlispWidget)wd;
  if(w->xbvlisp.killbuf)
    InsertStringXbvlisp(wd, w->xbvlisp.killbuf);
}

/*
 * l'implantation de selections doit etre refaite !
 */
/* ARGSUSED */
void insert_selection(w, event, params, num_params)
     Widget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  CHAR *cut_buffer = NULL;
  int ncutbf, nbytes;
  
  if(*num_params == 0) ncutbf = 0;
  else ncutbf = **params - 48;
  cut_buffer = XFetchBuffer(XtDisplay(w), &nbytes, ncutbf);
  if(nbytes > 0){
    wst(cut_buffer);
    XtFree(cut_buffer);
  }
}

/*
 * trouve le dernier caractere different de blanc
 * et positionne le curseur sur le caractere suivant.
 */
/* ARGSUSED */
void end_of_lineXbvl(w, event, params, num_params)
     XbvlispWidget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  CHAR *editbuf, c;
  Int pos, ref;
  
  ref = pos =   (Int)  XtTextGetInsertionPoint((Widget)w);
  GetXbvlispString(w, editbuf);
  while((c = *(editbuf + pos)) != '\n' &&  c ) pos += 1;
  pos -= 1;
  while(pos > ref && *(editbuf + pos) == ' ') pos -= 1;
  if(pos >= ref) 
    XtTextSetInsertionPoint((Widget)w, (XtTextPosition) pos + 1);
}

/* ARGSUSED */
void toggle_insert_mode( w, event , params, num_params)
     Widget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  XbvlispWidget wxb = (XbvlispWidget) w;
  if (wxb->xbvlisp.toggle == True) 
    wxb->xbvlisp.toggle = False;
  else wxb->xbvlisp.toggle = True;
}

/*----------------------------------------------------------------------
* Date: Fri Jun  5 15:05:57 1992, dario@thor
*-----------------------------------------------------------------------
* Description: on coupe les spaces avant le retour charriot 
* Input: une chaine de caracteres a etre evalue
* Output: une chaine de caracteres avec les blancs de fin de ligne tronques
*----------------------------------------------------------------------*/
static CHAR * cleanXbvlBuffer(cut_buf)
     CHAR *cut_buf;
{
  CHAR *buf_ret , *ptr ;
  Int nblancs = 0;
  
  if(!cut_buf) return NULL;
  
  buf_ret = ptr = cut_buf;
  while(1){ 
    CHAR c = *ptr++;
    *buf_ret++ = c;
    if(c == ' ') 
      nblancs += 1;
    else if(c == '\n'&& nblancs){
      buf_ret -= (nblancs + 1);
      *buf_ret++ = '\n';
      nblancs = 0;
    }
    else if(c == 0) {
      if(nblancs){
	buf_ret -= (nblancs + 1);
	*buf_ret = 0;
      }
      return cut_buf;
    }
    else nblancs = 0;
  }
}
  
/* 
 * Communique a xbVLISP le contenu d'une selection.
 * Il y a deux modes de communiquer le selection a l'interprete :
 * En evaluant la selection comme si elle etait du texte entree par l'utilisateur
 * En insertion du texte simple.
 * Le texte est reformate dans les deux cas.
 *ARGSUSED */
void EvalSelection(w, event, params, num_params)
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num_params;
{
  int nbytes;
  CHAR *cut_buffer;
  if(event->type != ButtonPress) return;
  cut_buffer = XFetchBuffer(XtDisplay(w), &nbytes, 0);
  if(nbytes > 0 )
    external_ptr = external_buffer = cleanXbvlBuffer(cut_buffer);
}
