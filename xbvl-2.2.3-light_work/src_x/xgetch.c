/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *  pour les entrees de l'interprete
 *          xgetch.c
 *  Ernesto Sendoya janvier 1992
 */


#include <vlisp.h>
#include <X11/keysym.h>

#ifdef STDC_HEADERS
static Boolean getCharFromXSystem(Int *c);
static Boolean getExprFromXSystem(CHAR **buf);
#else
static Boolean getCharFromXSystem();
static Boolean getExprFromXSystem();
#endif


/*------------------------------------------------------------*
 * pour faire la lecture ou insersion d'expressions
 * communiquees par les widgets:
 *------------------------------------------------------------*/

CHAR *external_buffer, *external_ptr;
CHAR * w_to_vlisp_exprs[BUF_CIRCULAIRE_TAILLE];
Int tail_expr = -1, head_expr = 0;

static Boolean getExprFromXSystem(buf)
     CHAR * * buf;
{
  if(tail_expr == head_expr - 1 ||
     (tail_expr == BUF_CIRCULAIRE_TAILLE - 1 && head_expr == 0))
    return False; /* buffer vide */
  if(tail_expr == BUF_CIRCULAIRE_TAILLE - 1)
    tail_expr = 0;
  else tail_expr += 1;
  *buf =   w_to_vlisp_exprs[tail_expr];
  return True;
}


/*------------------------------------------------------------*
 * Pour les caracteres entres au clavier
 *------------------------------------------------------------*/

CHAR x_to_vlisp_chars[BUF_CIRCULAIRE_TAILLE];
Int tail_char = -1, head_char = 0;

static Boolean getCharFromXSystem(c)
     Int *c;
{
  if(tail_char == head_char - 1 ||
     (tail_char == BUF_CIRCULAIRE_TAILLE - 1 && head_char == 0))
    return False; /* buffer vide */
  if(tail_char == BUF_CIRCULAIRE_TAILLE - 1)
    tail_char = 0;
  else tail_char += 1;
  *c =  (Int) x_to_vlisp_chars[tail_char];
  return True;
}


/*
 *    retourne le prochain caractere du flux d'entre
 *    ----------------------------------------------
 * On fait connaitre a bVlisp que les caracteres communiques 
 * par les widgets,
 * retourne le prochain caractere en provenance du clavier
 * ou de l'expression communiquee par un widget.
 */

Int xgetch1(ret_event)
     XEvent *ret_event;
{
  Int c = 0;
  XEvent event;
  
  while(1){
    /* excrutation d'expressions en provenance des widgets
       avant l'apprehension d'evenements.                   */
    if(! external_buffer && getExprFromXSystem(& external_buffer) )
      external_ptr = external_buffer;
    if(external_buffer){
      /* a revoir ************************/
      if((Int)pc == (Int)tyi){ /* un seul caractere demande */
	c = (Int)*external_ptr++;
	if(! *external_ptr){
	  XtFree(external_buffer);
	  external_buffer = external_ptr = NULL;
	}
	/******************************** jusqu'ici*/
	return c;
      }
      else { /* une expression a communiquer */
	CHAR * repere = index(external_ptr, '\n');
	/* preparation du buffer a evaluer */
	copyFiltre(bufin, external_ptr, external_ptr, 
		   repere ? repere - external_ptr : strlen(external_ptr));
	if(repere) 
	  external_ptr = repere + 1;
	else {
	  XtFree(external_buffer);
	  external_buffer = external_ptr = NULL;
	}
	return  CALLBACK_INPUT;
      }
    }

    if(getCharFromXSystem(&c)) return c;
    XtNextEvent(&event);
    XtDispatchEvent(&event); 
    if(getCharFromXSystem(&c)) return c;
  }
}


/*  
 * vide et traite tous les evenements, exeption des messages,
 * qui sont remis dans la queue et arretent la boucle de traitement,
 * une absence d'evenements ne bloque pas la fonction.
 *
 */

void XteLoop()
{
  XEvent evnt; 
#if 0 /* essai de tous les masks */
  long mask =  ExposureMask|KeyPressMask|KeyReleaseMask|StructureNotifyMask;
#else
/*  static long mask = 0x00ffffff; */ /* 1-24 a 1 */
#endif
  while(XCheckMaskEvent(display, 0x00ffffff, &evnt) == True)
    XtDispatchEvent(&evnt); /* un evenement a traiter */
}

/*
 * Action qui ecrit des caracteres pour que VLISP les lise,
 * permet d'etablir les entrees pour VLISP a partir d'un
 * Widget d'interaction.
 ARGUSED */
void communiqCharToVlisp(w, event, params, num_params)
     Widget w;
     XKeyEvent *event;
     String *params;
     Cardinal *num_params;
{
  Int key;
  CHAR c;
  
  if (event->type != KeyPress) return;
#undef XtSpecificationRelease
#ifndef XtSpecificationRelease
  XLookupString((XKeyEvent *)event, (char *)&c, 1, (KeySym *)&key,  NULL);	
#else
  key = XtGetActionKeysym(event, NULL);
#endif

  /* on regarde si le buffer de communication est sature */
  if(head_char == tail_char || 
     (head_char == BUF_CIRCULAIRE_TAILLE - 1  && tail_char == 0))
    return; /* saturation, le caractere est perdu */
#ifdef X_USE_LATIN1
  if(key & 0xFF00 )
#else
  /* refuse les accents en entrée */
  if(key & 0xFF80 )
#endif
    { 
    if(key == XK_Return||key == XK_KP_Enter||key == XK_Return){
      x_to_vlisp_chars[head_char] = '\n';
      if(head_char ==  BUF_CIRCULAIRE_TAILLE - 1)
	head_char = 0;
      else head_char += 1;
      return;
    }
    /* key n'est pas un caractere ASCII, ni retour charriot */ 
    printf("polison a bord -> %d\n", key);
    return;
  }
  /* keysym egal au caractere, on le communique a bvlisp */
  x_to_vlisp_chars[head_char] = key; 
  if(head_char ==  BUF_CIRCULAIRE_TAILLE - 1)
    head_char = 0;
  else head_char += 1;
}

/*
 * Action qui ecrit des caracteres pour que VLISP les lisse,
 * permet d'etablir les entrees pour VLISP a partir d'un
 * Widget d'interaction.
 ARGUSED */
Boolean communiqExprToVlisp(expr)
     CHAR *expr;
{
  /* on regarde si le buffer de communication est sature */
  if(head_expr == tail_expr || 
     (head_expr == BUF_CIRCULAIRE_TAILLE - 1  && tail_expr == 0))
    return False; /* saturation, l'expression est perdue */
  w_to_vlisp_exprs[head_expr] = expr; /* expression enregistree */ 
  if(head_expr ==  BUF_CIRCULAIRE_TAILLE - 1)
    head_expr = 0;
  else head_expr += 1;
  return True;
}
