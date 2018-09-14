/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*   Ici on construit l'homogeneite des objets graphiques.
 *   avec un widget pour communiquer avec xbVLISP.
 *  	         Xbvlisp.h
 *     Widget D'interaction avec xbVLISP
 *        Ernesto Sendoya, avril 92
 */

#include <vlisp.h> 

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <XbvlispP.h>

#ifndef XtSpecificationRelease
#include <X11/Form.h>
#else
#include <X11/Xaw/AsciiSrc.h>
#include <X11/Xaw/AsciiSink.h>
#include <X11/Xaw/Form.h>
#endif
#include <stdio.h>
#include <string.h>

/* comme c'est un widget dedie a la communication avec xbVLISP */

extern void handleFocusChange();



/*---------------------------------------*/
/* pour recuperer les options utilisateur */
	/* et defauts */
/*---------------------------------------*/

/*Provisoire. 
  Ces ressources sont en lecture seulement, donc pas besoin de les declarer,
  dans la liste de ressources.
  Il est suffisant de les calculer dans GetValues. Pour l'instant on 
  utilise ces ressources pour Debugage.
   */
static XtResource resources[] = {
#define offset(field) XtOffset(XbvlispWidget, xbvlisp.field)
  { XtNlignes, XtCLignes,  XtRInt,  sizeof(Int),
      offset(lignes),  XtRImmediate, (caddr_t) DEFAULT_LIGNES,},	
  { XtNcolonnes, XtCColonnes, XtRInt, sizeof(Int),
      offset(colonnes), XtRImmediate, (caddr_t) DEFAULT_COLONNES, },
  {  XtNpages, XtCPages, XtRInt, sizeof(Int),
       offset(pages), XtRImmediate, (caddr_t) DEFAULT_PAGES,  },
#undef offset
};


/*    des actions generales de l'application 
  {  "toggle_border_color", ( XtActionProc) toggle_border_color },
  {  "toggle_background_color", ( XtActionProc) toggle_background_color },
  {  "toggle_foreground_color", ( XtActionProc) toggle_foreground_color },
  {  "placeMenu", ( XtActionProc)PlaceMenu},
  {  "liste_aide", ( XtActionProc)liste_aide},
*/

static XtActionsRec _XbvlispActions[] = {
  {  "cree_bVLISP_window",  ( XtActionProc) cree_bVLISP_window },
  {  "stop", ( XtActionProc)stop_x},
  {  "arret", ( XtActionProc)jump_start},
  { "TITRE", ( XtActionProc) TITRE },
  {  "delete_next_character", ( XtActionProc) delete_next_characterXbvl }, 
  {  "delete_previous_character", ( XtActionProc) delete_previous_characterXbvl },
  {  "shift_car_right", ( XtActionProc) shift_car_rightXbvl },
  {  "kill_to_end_of_line", ( XtActionProc)kill_to_end_of_lineXbvl},
  {  "insert_selection", ( XtActionProc) insert_selection},
  {  "toggle_insert_mode", ( XtActionProc)toggle_insert_mode},
  {  "end_of_line", ( XtActionProc)end_of_lineXbvl},
  {  "faire_beep", ( XtActionProc)faire_beep },
  {  "select_aide", ( XtActionProc)select_aide},
  {  "liste_aide", ( XtActionProc)liste_aide},
  {  "arret", ( XtActionProc)jump_start},
  {  "delete_next_atome", ( XtActionProc)delete_next_atomeXbvl},
  {  "delete_prev_atome", ( XtActionProc)delete_prev_atomeXbvl},
  {  "unkill_der_kill", ( XtActionProc) unkill_der_killXbvl},
  {  "EvalSelection", (XtActionProc)EvalSelection},
  {  "communiqCharToVlisp", ( XtActionProc) communiqCharToVlisp},
};


/*
*/

/* Les Traslations */
#ifndef XtSpecificationRelease
/* dificultes pour recuprere le caractere 'S' et 's' ?*/
static char  _XbvlispTranslations[] =
"\
<Key>s: communiqCharToVlisp()\n\
Ctrl<Key>C:	forward-word()\n\
Ctrl<Key>D:	delete_next_character()\n\
Ctrl<Key>E:	end_of_line()\n\
Ctrl<Key>G:	delete_next_atome()\n\
Ctrl<Key>H:	delete_previous_character()\n\
Ctrl<Key>K:	kill_to_end_of_line()\n\
Ctrl<Key>M:	insert-file()\n\
Ctrl<Key>O:	faire_beep() \n\
Ctrl<Key>R:	previous-line()\n\
Ctrl<Key>X:	backward-word()\n\
Ctrl<Key>I:	delete_prev_atome()\n\
Ctrl<Key>J:	backward-word()\n\
Ctrl<Key>W:	TITRE()\n\
Meta<Key>B:	backward-word()\n\
Meta<Key>D:	shift_car_right()\n\
Meta<Key>E:	end-of-line()\n\
Ctrl<Key>Y:	unkill_der_kill()\n\
:Meta<Key>h:	faire_beep() \n\
:Meta<Key>H:	faire_beep() \n\
Meta<Key>K:	faire_beep()\n\
~Shift Meta<Key>Delete:		faire_beep() \n\
 Shift Meta<Key>Delete:		faire_beep() \n\
~Shift Meta<Key>BackSpace:	faire_beep() \n\
 Shift Meta<Key>BackSpace:	faire_beep() \n\
<Key>Shift:	 \n\
<Key>Shift_L:    \n\
<Key>Shift_R:    \n\
<Key>Control_L:  \n\
<Key>Control_R:  \n\
<Key>Caps_Lock:  \n\
<Key>Shift_Lock: \n\
<Key>Right:	forward-character()\n\
<Key>Left:	backward-character()\n\
<Key>Down:	next-line()\n\
<Key>Up:	previous-line()\n\
<Key>Delete:	delete_previous_character()\n\
<Key>Escape:	arret()\n\
<Key>BackSpace:	delete_previous_character()\n\
<Key>F1:	select_aide()\n\
<Key>F2:	liste_aide()\n\
<Key>F3:	TITRE()\n\
<Key>F4:	toggle_insert_mode()\n\
<Key>F5:	faire_beep(1)\n\
<Key>F7:	faire_beep(1)\n\
<Key>Insert:	toggle_insert_mode()\n\
Ctrl<Key>0:	insert_selection(0)\n\
Ctrl<Key>1:	insert_selection(1)\n\
Ctrl<Key>2:	insert_selection(2)\n\
Ctrl<Key>3:	insert_selection(3)\n\
Ctrl<Key>4:	insert_selection(4)\n\
Ctrl<Key>5:	insert_selection(5)\n\
Ctrl<Key>6:	insert_selection(6)\n\
~Ctrl ~Meta <Key>:	communiqCharToVlisp()\n\
Button2<Key>7:	insert_selection(7)\n\
Ctrl <Btn2Down>: EvalSelection()\n\
<Btn2Down>:	insert_selection(0)";
#else
/* pour la Relaise 4, 5 .. */
static char  _XbvlispTranslations[] =
"\
Ctrl<Key>C:	forward-word()\n\
Ctrl<Key>D:	delete_next_character()\n\
Ctrl<Key>E:	end_of_line()\n\
Ctrl<Key>F:	forward-character()\n\
Ctrl<Key>G:	delete_next_atome()\n\
Ctrl<Key>H:	delete_previous_character()\n\
Ctrl<Key>I:	delete_prev_atome()\n\
Ctrl<Key>J:	backward-word()\n\
Ctrl<Key>K:	kill_to_end_of_line()\n\
Ctrl<Key>M:	insert-file()\n\
Ctrl<Key>O:	form-paragraph()\n\
Ctrl<Key>V:	next-page()\n\
Ctrl<Key>W:	TITRE()\n\
Ctrl<Key>Y:	unkill_der_kill()\n\
Ctrl<Key>Z:	scroll-one-line-up()\n\
Meta<Key>D:	shift_car_right()\n\
Meta<Key>E:	end-of-line()\n\
Meta<Key>Z:	scroll-one-line-down()\n\
:Meta<Key>h:	faire_beep() \n\
:Meta<Key>H:	faire_beep() \n\
Meta<Key>K:	faire_beep()\n\
~Shift Meta<Key>Delete:		faire_beep() \n\
 Shift Meta<Key>Delete:		faire_beep() \n\
~Shift Meta<Key>BackSpace:	faire_beep() \n\
 Shift Meta<Key>BackSpace:	faire_beep() \n\
<Key>Shift_L:    \n\
<Key>Shift_R:    \n\
<Key>Control_L:  \n\
<Key>Control_R:  \n\
<Key>Caps_Lock:  \n\
<Key>Shift_Lock: \n\
<Key>Right:	forward-character()\n\
<Key>Left:	backward-character()\n\
<Key>Down:	next-line()\n\
<Key>Up:	previous-line()\n\
<Key>Delete:	delete_previous_character()\n\
<Key>Escape:	arret()\n\
<Key>BackSpace:	delete_previous_character()\n\
<Key>F1:	select_aide()\n\
<Key>F2:	liste_aide()\n\
<Key>F3:	TITRE()\n\
<Key>F4:	toggle_insert_mode()\n\
<Key>F5:	no-op(RingBell)\n\
<Key>F7:	no-op(RingBell)\n\
<Key>Insert:	toggle_insert_mode()\n\
Ctrl<Key>0:	insert_selection(0)\n\
Ctrl<Key>1:	insert_selection(1)\n\
Ctrl<Key>2:	insert_selection(2)\n\
Ctrl<Key>3:	insert_selection(3)\n\
Ctrl<Key>4:	insert_selection(4)\n\
Ctrl<Key>5:	insert_selection(5)\n\
Ctrl<Key>6:	insert_selection(6)\n\
Ctrl<Key>7:	insert_selection(7)\n\
~Ctrl ~Meta <Key>:	communiqCharToVlisp()\n\
<FocusIn>:	display-caret(on)\n\
<FocusOut>:	display-caret(off)\n\
<Btn1Down>,<Btn1Down>:	select-word()\n\
Ctrl <Btn2Down>: EvalSelection()\n\
<Btn2Down>:	insert_selection(0)";

#endif
static void  ClassInitialize();
static void Initialize();
static XtGeometryResult XbvlispQueryGeometry();
static void  XbvResize();
static void Destroy();
static Boolean SetValues(); 
static void xbvlisp_hook();

#if XtSpecificationRelease
#define superClase   (&asciiTextClassRec)
#else
#define superClase   (&asciiStringClassRec)
#endif

XbvlispClassRec xbvlispClassRec = {
  {
    /* core fields */
    /* superclass       */      (WidgetClass) superClase,
    /* class_name       */      "Text",
    /* widget_size      */      sizeof(XbvlispRec),
    /* class_initialize */      (XtProc)ClassInitialize, /* ni converter ni grab action a enregistrer */
    /* class_part_init  */	NULL,       /* pas d'initialisation dynamique de la classe  */
    /* class_inited     */      FALSE,      /* toujours */
    /* initialize       */      (XtInitProc)Initialize,/* initialisation des champs de la sous-classe */
    /* initialize_hook  */	xbvlisp_hook,
    /* realize          */      XtInheritRealize,
    /* actions          */      _XbvlispActions, 
    /* num_actions      */      XtNumber(_XbvlispActions),
    /* resources        */      resources, /*NULL, */
    /* num_resource     */      XtNumber(resources),
    /* xrm_class        */      NULLQUARK,
    /* compress_motion  */      TRUE,
#ifdef XtSpecificationRelease
    /* compress_exposure*/      /* TRUE, */ XtExposeGraphicsExpose | XtExposeNoExpose,
#else
    /* compress_exposure*/      TRUE,
#endif
    /* compress_enterleave*/	TRUE,
    /* visible_interest */      FALSE,
    /* destroy          */      (XtWidgetProc)Destroy, 
    /* resize           */      XbvResize, 
    /* expose           */      XtInheritExpose,
    /* set_values       */      NULL,
    /* set_values_hook  */	SetValues, 
    /* set_values_almost*/	XtInheritSetValuesAlmost,
    /* get_values_hook  */	NULL,
    /* accept_focus     */      XtInheritAcceptFocus,
    /* version          */	XtVersion,
    /* callback_private */      NULL,
    /* tm_table         */      XtInheritTranslations,
    /* query_geometry	*/	NULL /*XbvlispQueryGeometry,*/
  },
  { /* Simple fields */
    /* change_sensitive	*/	XtInheritChangeSensitive,
  },
  { /* text fields */
    /* empty            */      0,
  },
  { /* ascii fields */
    /* empty            */      0,
  },
 {/*  xbvlisp fields */
   /* empty */                  0,
  }
};

WidgetClass xbvlispWidgetClass = (WidgetClass)&xbvlispClassRec;

/*
 * les translations pour remplacer celle
 * qui ne nous conviennent pas de la superclasse.
 * Une facon general de profiter incrementalement des
 * nouvelles fonctionalites des widgets Text.
 */

XtTranslations xbvl_Translations ;

static void  ClassInitialize()
{
  
  /*
   * les translations pour remplacer celle
   * qui ne nous conviennent pas de la superclasse.
   * Une facon general de profiter incrementalement des
   * nouvelles fonctionalites des widgets Text.
   */

  xbvl_Translations = XtParseTranslationTable(_XbvlispTranslations);

#if 0  
  /* 
    recuperation des defaults: on traite les ressources 
    specifiques a Xbvlisp commo ressources de l'application.
    */
  /* le probleme est qu'on a besoin d'initialiser la classe avant le 
     premier appel de XtCreateWidget(...,xbvlispWidgetClass, ..).
     XtClassInitialize(WidgetClass) ...
     */
  XtGetApplicationResources(top_widget,   /* doit etre deja realize */
			    (caddr_t)&xbvl_Defaults_data,
			    (XtResourceList)resources,
			    (Cardinal)XtNumber(resources),
			    (ArgList)NULL, (Cardinal)0);
#endif  

}

/* ARGSUSED */
#ifdef XtSpecificationRelease
static void  Initialize(request, new,  args, num_args)
     XbvlispWidget request, new;
     ArgList args; 
     Cardinal * num_args ;
#else
static void  Initialize(request, new)
     XbvlispWidget request, new;
#endif
{
  Arg wargs[10]; Int i;
  XFontStruct *font = NULL;
  Dimension border_width, left_margin;

  new->xbvlisp.n_bytes = 0; /* pour l'instant */
  new->xbvlisp.dessin_gc = NULL;
  new->xbvlisp.pixmap =  (Int)NULL;
  new->xbvlisp.tortues = NULL;
  new->xbvlisp.killbuf = NULL;

  XtAddEventHandler((Widget)new,/* ou les evenements arrivent */
		    FocusChangeMask|EnterWindowMask|LeaveWindowMask,
		    False,
		    (XtEventHandler)handleFocusChange,	/* la fonction */
		    (caddr_t)NULL);		/* l'argument  */


  i = 0;
  XtSetArg(wargs[i], XtNleftMargin, &left_margin); i++;
  XtSetArg(wargs[i], XtNborderWidth, &border_width); i++;
  XtSetArg(wargs[i], XtNfont, &font); i++;
  XtGetValues((Widget) new, wargs, i);

  if(font || getWidgetFont((Widget)new, &font)){
    /* calcul de la taille du widget, (lignes/colonnes) */
    new->core.width =  (Dimension) (new->xbvlisp.colonnes+CARS_MARGE_DROITE)*FONTWIDTH(font) + border_width + left_margin;
    new->core.height = (Dimension) FONTHEIGHT(font) * new->xbvlisp.lignes + border_width ; /*+0.5*/
    XbvResize(new);
  }
  XtOverrideTranslations((Widget)new, xbvl_Translations );
}

/* ARGSUSED */
static void  Destroy(w)
     XbvlispWidget w;
{
  XtFree(w->xbvlisp.string);
}

/*
 * On met a jour lignes/colonnes
 */
static void  XbvResize(w)
    XbvlispWidget w;
{
  Arg wargs[5];
  XFontStruct *font = NULL;
  Int i = 0, cursor_x, cursor_y;
  Dimension left_margin;
  Int lignes, colonnes;
  Dimension border_width;

  XtSetArg(wargs[i], XtNleftMargin, &left_margin); i++;
  XtSetArg(wargs[i], XtNborderWidth, &border_width); i++;
  XtSetArg(wargs[i], XtNfont, &font); i++;
  XtGetValues((Widget) w, wargs, i);
  
  getCursorPosXbvl(w, &cursor_y, &cursor_x);

  if(!font)  getWidgetFont((Widget)w, &font); /* prevoir X11-R3 */
  /* calcul de nouvelles dimentions de l'interactin, (lignes/colonnes) */
  colonnes = (w->core.width - border_width - left_margin)/FONTWIDTH(font)-CARS_MARGE_DROITE;
  lignes = (w->core.height - border_width)/FONTHEIGHT(font) ;

  (*xbvlispClassRec.core_class.superclass->core_class.resize)((Widget)w);
  
  if(colonnes >= MIN_COLONNES && lignes >= MIN_LIGNES 
     && (colonnes > w->xbvlisp.colonnes || lignes > w->xbvlisp.lignes) ){

    CHAR *old_string = w->xbvlisp.string;
    CHAR *new_string = calculXbvlEditBuffer(lignes, colonnes, w->xbvlisp.pages, old_string);
    XtTextSource new_src = XtTextGetSource((Widget) w);

    w->xbvlisp.lignes = lignes;
    w->xbvlisp.colonnes = colonnes;

    i = 0; 
    XtSetArg(wargs[i], XtNstring, new_string); i++;
    XtSetArg(wargs[i], XtNlength, (w->xbvlisp.pages * lignes * colonnes + 1)); i++;
    XtSetValues((Widget) new_src, wargs, i);
    
    w->xbvlisp.string = new_string;
  }

  setCursorPosition(w, cursor_y, cursor_x);
}

static Boolean SetValues(current, request, new)
     XbvlispWidget current, request , new;
{
  return False;
  if(new->xbvlisp.lignes != current->xbvlisp.lignes
     ||new->xbvlisp.colonnes != current->xbvlisp.colonnes
     ||new->xbvlisp.pages != current->xbvlisp.pages){
    if(new->xbvlisp.lignes < MIN_LIGNES
       || new->xbvlisp.colonnes < MIN_COLONNES
       || new->xbvlisp.pages < MIN_PAGES){
      /* erreur de positionement de ressources */
      new->xbvlisp.lignes = current->xbvlisp.lignes;
      new->xbvlisp.colonnes = current->xbvlisp.colonnes;
      new->xbvlisp.pages = current->xbvlisp.pages;
      return False;
    }
    else{
      /* on calcule une nouvelle source */
      XtTextSource old_src;
      CHAR *string = new->xbvlisp.string;
      new->xbvlisp.string = calculXbvlEditBuffer(new->xbvlisp.lignes, 
						 new->xbvlisp.colonnes, 
						 new->xbvlisp.pages, 
						 string);
      old_src = (XtTextSource) XtTextGetSource((Widget)new);
      return True;
    }
  }  else return False;
}
static XtGeometryResult XbvlispQueryGeometry(w, proposed, answer)
     XbvlispWidget w;
     XtWidgetGeometry *proposed, *answer;
{
  puts ("QueryGeometry appele\n");
  
  answer->request_mode = CWWidth | CWHeight;
  answer->width = (w->core.width < 100) ? 100 :  w->core.width;
  answer->height = (w->core.height < 19) ? 19 :  w->core.height;
  
  if ( ((proposed->request_mode &(CWWidth | CWHeight)) == (CWWidth | CWHeight))
	&& proposed->width == answer->width && proposed->height == answer->height )
    return XtGeometryYes;
  else if (answer->width == w->core.width
	   && answer->height == w->core.height)
    return XtGeometryNo;
  else 
    return XtGeometryAlmost;
#if   0
  if(w->core.width < 100 || w->core.height < 19 ){
    if(w->core.width < 100) w->core.width = 100;
    if(w->core.height < 19) w->core.height = 19;
    return XtGeometryAlmost;
  }
  return XtGeometryYes;
#endif
}


/*----------------------------------------------------------------------
* Date: Wed Jun 10 17:28:30 1992, dario@thor
*-----------------------------------------------------------------------
* Description:  copie src dans dest
* Inputs: deux buffers d'interaction
*  dest: le buffer a modifier
*  src: 
* Function copyOldString return nothing
*----------------------------------------------------------------------*/
void copyOldString( dest, src )
     CHAR *dest, *src;
{
  Int cars = 0;
  while(*dest && *src){
    if(*dest == '\n'){
      while(*src && *src != '\n') ++src;
      if(*src) ++src;
      ++dest;
      cars = 0;
    } else if(*src == '\n'){
      while(*dest && *dest != '\n') ++dest;
      if(*dest) ++dest;
      ++src;
      cars = 0;
    }
    else {
      if(*src == '\t'){ /* expansion de tabulations */
	cars = (cars >= TABSTOPS ) ? cars%TABSTOPS : TABSTOPS - cars ;
	while(cars && cars-- > 0 && *dest != '\n')
	  *dest++ = ' ';
	++src;
	cars = 0;
      } else {
	if (*src == ' ') cars = 0;
	else ++cars;
	*dest++ = *src++;
      }
    }
  }
}

static CHAR * calculXbvlBuffer(new, ptr)
     XbvlispWidget new;
     CHAR *ptr;
{
  return calculXbvlEditBuffer(new->xbvlisp.lignes,  new->xbvlisp.colonnes, new->xbvlisp.pages, ptr);
}


CHAR * calculXbvlEditBuffer(lignes, colonnes, pages, ptr)
     Int lignes;
     Int colonnes;
     Int pages;
     CHAR *ptr;
{
  Int t_lignes = lignes * pages;
  CHAR *s = XtMalloc(t_lignes * colonnes + 1);
  
  formatBuffer(s, t_lignes, colonnes );

  if(ptr){ /* le copier sur s */
    copyOldString(s, ptr);
  }
  return s;
}

/*
 * bidon
 */
static void xbvlisp_hook(new, wargs, num_args)
     XbvlispWidget new;
     ArgList wargs;
     Cardinal *num_args;
{
  extern CHAR *xbvl_string;
  Int i;
  for(i = 0; i < *num_args; i++){
    if(strcmp(XtNstring, wargs[i].name) == 0)
      break;
  }
  new->xbvlisp.string = xbvl_string;
}

/*------------------------------------------------------------------
 * extrait la police de caracteres comme ressourse
 * pour essayer de palier aux deficiences du Relais 3 ou XtGetValues 
 * ne fonctionne pas pour ce cas precis.
 ------------------------------------------------------------------*/
Boolean  getWidgetFont(widget, ret_font)
     Widget widget;
     XFontStruct **ret_font;

{

  /*            RESOURCE
   * Table Resources, decrivant les ressources
   *    la police de caracteres a recuperer
   */

  static XtResource f_resource[] = {
    {
      XtNfont, XtCFont,
      XtRFontStruct,
      sizeof(XFontStruct *),
      0,     /* XtOffset(vlisp_data_ptr, font),*/
      XtRString,
      "9x15",
    },
  };
  
  XtGetApplicationResources(widget,
			    (caddr_t)ret_font,
			    (XtResourceList)f_resource,
			    (Cardinal)XtNumber(f_resource),
			    (ArgList)NULL, (Cardinal)0);
  
  return True;
}

/***************************************************************************/
/*              fonctions pour la communication avec xbVLISP               */
/***************************************************************************/
Int InsertCharXbvlisp(w, e, ss, cc)
     Widget w;
     XEvent * e;
     CHAR **ss;
     Int cc;
{

  XbvlispWidget xbw = (XbvlispWidget) w; 
  Int col_cour, cols = xbw->xbvlisp.colonnes;
  CHAR *editbuf = xbw->xbvlisp.string;
  Int pos, ss_pos ;
  Int length ;
  CHAR cpy_buf[128], obuf[128];
  
  ss_pos = pos =  XtTextGetInsertionPoint(w);

  if(is_imprimable(cc) || cc == '\n'){
    obuf[0] = (CHAR) cc;
    obuf[1] = 0;
  } else if(cc == '\b') { /* insertion d'un backspace */
    if(pos>0) XtTextSetInsertionPoint(w, pos-1);
    return INSERT_DONE;
  } else {
    /* DEBUG */
    printf(" caractere non imprimable <%u> en InsertCharXbvlisp\n", cc);
    return -1;
  }
  

  col_cour = pos % cols;
  *cpy_buf = 0;

  /*
    l'idee est d'eviter la coupure du dernier mot ou cas
    ou on est arrive a la fin de ligne a l'entree d'un caractere,
    Dans ce cas on retourne le caractere ou le mot dont il fait
    partie et on passe le reste du buffer a l'evaluateur.
    */
  
  if(cc != '\n'  && col_cour == cols - 2){
    /* si le caractere fait partie d'un mot, on sauve le mot pour */
    /* l'affichage suivante                                       */
    CHAR c;
    
    while((c = *(editbuf + ss_pos - 1)) && is_lettre(c))  --ss_pos;
    if(length = pos - ss_pos){
      *(obuf + length) = cc ;
      strncpy(obuf, editbuf + ss_pos, length);
      *(obuf + length + 1) = 0;
    }
    XtTextSetInsertionPoint(w, ss_pos);
    if(ss)*ss = XtNewString(obuf);
    /* ligne passee a l'evaluateur automatiquement */
    return RETOUR_AUTO;
  }
  if(xbw->xbvlisp.toggle == True) xwrightXbvl(xbw, 1);
  InsertStringXbvlisp(w, obuf);
  return INSERT_DONE;
}
/*
 * enleve les lignes a scroller
 * ajoute des lignes blanches a la fin du texte.
 * le widget s'occupe de gestionner l'affichage.
 */
Boolean scrollTextXbvlisp (w, dici, nlignes)
     Widget w;
     Int dici, nlignes;
{
  XtTextBlock text;
  XtTextPosition premiere;
  CHAR  *tmp_buf;
  Int lignes, cols, pages, t_lignes, text_length; 
  Int deb_scroll, cars_scrolles;

  if (!w) return False;

  lignes = ((XbvlispWidget) w)->xbvlisp.lignes;
  cols = ((XbvlispWidget) w)->xbvlisp.colonnes;
  pages = ((XbvlispWidget) w)->xbvlisp.pages;
  t_lignes = lignes * pages ;
  text_length = t_lignes * cols;
  deb_scroll = dici * cols;
  cars_scrolles = nlignes * cols;

  if(dici + nlignes > t_lignes || nlignes < 1) 
    return False;

  /* effacer les lignes a scroller */
  text.firstPos = 0; 
  text.format = FMT8BIT;
  
  text.ptr = " ";
  text.length = 0 ;
  
  /*  XtTextDisableRedisplay(w);*/
  if(eXtTextReplace(w ,
		 (XtTextPosition) deb_scroll, 
		 (XtTextPosition) deb_scroll + cars_scrolles  , &text) != XawEditDone) 
    return False;
  
  /* et completer avec des blancs  a la fin du text */
  tmp_buf = XtMalloc(cars_scrolles + 2);
  *tmp_buf = '\n'; /* le premier retour a la ligne */
  formatBuffer( tmp_buf + 1, nlignes  , cols);
  
  appendText(w , text_length - cars_scrolles - 1, tmp_buf);
  XtFree(tmp_buf);

  /* reafficher le widget */
  premiere = XtTextTopPosition(w);
  XtTextInvalidate(w, premiere, (XtTextPosition) premiere + lignes * cols);
  return True;
}

/*
 * insertion d'une chaine dans le widget d'interaction
 * passe un argument.
 * le curseur va a la fin de la chaine inseree.
 */

Boolean InsertStringXbvlisp(w, buf)
     Widget w;
     CHAR *buf;
{
  XbvlispWidget interface = (XbvlispWidget) w;
  CHAR cpy_buf[TAILLE_TAMPON*2], tmp_str[TAILLE_TAMPON], *ptr_str;
  Int i, new_pos, l_courr, l_total, str_lignes, correct ;
  Int text_lignes, cols, pages, total_lignes, text_length;
  Int ligne_cour, col_cour, l_buf;
  CHAR *editbuf;

  if(! w ) {
    /* DEBUG 
    printf("Err InsertTextXbvlisp [%s]\n", buf); 
    fflush(stdout); */
    return False;
  }
  
  text_lignes = interface->xbvlisp.lignes;
  cols = interface->xbvlisp.colonnes;
  pages = interface->xbvlisp.pages;
  total_lignes = text_lignes * pages;
  text_length = total_lignes * cols;
  editbuf  = interface->xbvlisp.string;
  l_total = l_courr = XtTextGetInsertionPoint(w);
  l_buf = strlen(buf);
  ligne_cour = l_courr / cols;
  col_cour = l_courr % cols;
  
  if(l_buf == 0) { return False; }

  cpy_buf[0] = 0;
  /*
    l'idee est d'eviter la coupure du dernier mot ou cas
    ou on est arrive a la fin de la ligne quand on tape,
    on coupe du buffer d'edition le mot incomplet et on
    le met en tete de la chaine a ajouter a ce meme buffer
    */
  if(l_buf == 1 && col_cour == 0 && ligne_cour > 0
     && is_lettre(*(editbuf + l_courr - 2))
     && is_lettre(*buf)){
    l_courr -= 1;
    correct = 0;
    while(is_lettre(*(editbuf + l_courr - 1)) && correct < cols-1 ){
      ++correct;
      l_courr -= 1;
    }
    if(correct == cols-1){ /* il fault accepter la coupure du mot */
      correct = 0;
      ptr_str = buf;
      l_courr = l_total;
    }
    else { /* mise a jour des variables, on se positionne avant le mot */
      strcpy(tmp_str, editbuf + l_courr );
      strcpy(tmp_str +  correct, buf);
      col_cour = cols - correct - 1 ;
      ptr_str = tmp_str;
      ligne_cour -= 1;
    }
  }
  else ptr_str = buf;
  
  str_lignes =  PreparAppend(cpy_buf, ptr_str, col_cour, cols);

  if(ligne_cour + str_lignes < total_lignes ) {
    /* on remplit le buffer */
    new_pos= appendText(w, l_courr, cpy_buf);
  }
  else if(str_lignes >= total_lignes ){ 
    l_buf = total_lignes * cols ;
    for(i = 0; i < text_lignes && l_buf > 0; l_buf-- ){
      if(*(cpy_buf + l_buf-1) == '\n'){
	ptr_str = cpy_buf + l_buf;
	++i;
      }
    }
    new_pos = l_buf = (*ptr_str == 0) ? 0 :  strlen(ptr_str);
    for(; l_buf < text_length - 1; l_buf++)
      *(ptr_str + l_buf) = ' ';
    *(ptr_str + l_buf) = 0;
  
    appendText(w, 0, ptr_str);
  } 
  else {
    /* scroll */
    Int scroll_lignes =  ligne_cour + str_lignes - text_lignes + 1;

    l_buf = strlen(cpy_buf);
    l_courr -= scroll_lignes*cols;
    
    scrollTextXbvlisp(w, 0, scroll_lignes);

    if(*(cpy_buf +l_buf-1) == '\n'){        /* la derniere ligne avec retour */
      new_pos = (interface->xbvlisp.lignes - 1)*cols; /* et le curseur va au debut de la ligne*/
    }
    else new_pos =  l_courr + l_buf; /* <-- a revoir */
    
    appendText(w, l_courr, cpy_buf);
  }

  XtTextSetInsertionPoint(w, new_pos); 
  return True;
}

Boolean  xgetBufin(bfin)
     CHAR *bfin;
{
  Int n, ligne_courr, pos =   XtTextGetInsertionPoint(Xbvlisp_courr);
  XbvlispWidget bvlw = (XbvlispWidget) Xbvlisp_courr;
  CHAR *str = NULL, *ptr;
  
  n = 0;
  while(n < MAX_COLS) *(bfin + n++) = 0; /* netoyer le buffer */

  if (! Xbvlisp_courr ) return False;
  str = bvlw->xbvlisp.string;
  ligne_courr = pos/bvlw->xbvlisp.colonnes;
  ptr = str + ligne_courr * bvlw->xbvlisp.colonnes + *atlmarg;
  /* ptr pointe vers la ligne qui va a etre evaluee */
  return copyFiltre(bfin, ptr, str, pos);
}

Boolean copyFiltre(bfin, ptr, str,  pos)
     CHAR *bfin, *ptr, *str;
     Int pos;
{
  Int n;

  if ( ! ptr ) {
    printf("Erreur appel a coppy Filtre:\nbufin = %s\n, external_ptr == NULL\n", bfin);
    return False;
  }
  if (*ptr == '=' )
    ++ptr;
  else if( *ptr == '?' && (UInt) *atprompt == (UInt)atlst ) 
    ++ptr;
  else { /* pour commencer la lecture apres le prompt */
    CHAR *prompt, nprompt[64];
    Int lpmpt;
    int_p1 = (Int *)*atprompt; 
    if(isnum(int_p1)){
      lpmpt = numToString(nprompt, int_p1);
      prompt = nprompt;
    } else {
      if(isatom(int_p1))
	int_p1=((struct atome *)int_p1)->pn;
      prompt = (CHAR *)(int_p1 + 2/2);
      lpmpt = strlen(prompt);
    }
    if(strncmp (prompt, ptr, lpmpt) == 0) ptr += lpmpt;
  }
  n = 0;
  while( ptr < (str + pos) && *ptr != 0 && n < 127){
    *bfin = *ptr;/* copie */
    if(*ptr == '\n')  break;
    ++n;
    ++ptr;
    ++bfin;
  }
  *bfin = '\n';
  *++bfin = '\0';
  return True;
}




static control_change_focus = False;

/* event handler
 * change la couleur du bord de la fenetre d'interface
 * et de celle qui vient de perdre la main.
 * ***************************************************
 ARGSUSED */
#ifndef XtSpecificationRelease
void handleFocusChange( w, nouvelle, event)
     Widget  w;
     Widget nouvelle;
     register XFocusChangeEvent *event;
#else
     void handleFocusChange( w, nouvelle, event, bid)
     Widget  w;
     Widget nouvelle;
     register XFocusChangeEvent *event;
     Boolean * bid;
#endif
{
  
  extern Int lcou ;/* nb de places libres restantes ds ligne */
  
  if(control_change_focus == True) return;
  if(event->type == EnterNotify||event->type == FocusIn){
    /* nouvelle fenetre courante et nouvelle taille de
       lignes pour les sorties de bVLISP */
    if(nouvelle == NULL){ /* un widget Xbvlisp demande le focus */
      if(XtIsSubclass(w, xbvlispWidgetClass)){
	Xbvlisp_courr = w;
	lcou = ((XbvlispWidget)w)->xbvlisp.colonnes - CARS_MARGE_DROITE;
      }
    }
  }
}

/*
 * fonctions d'acces a la variable
 *   disable_change_focus
 */

enable_change_focus()
{
  control_change_focus = False;
}
disable_change_focus()
{
  control_change_focus = True;
}

/*******************************************************************/
/* determine la quantite de caracteres de la ligne cree par bvlisp */
/* doit faire partie du protocole de communication entre           */
/*               bVLISP et les I/O                                 */
/*******************************************************************/
/* pour iniott et debut1 */
Int getXbvlColonnes(w) 
     Widget w;
{
  if(w) return ((XbvlispWidget)w)->xbvlisp.colonnes ;
}
/* ******************  FONCTIONS D'INTERFACE AVEC xbVLISP **************** */

/*---------------------------------------------------------------
 * lchangewin
 *            Change la fenetre active a celle passe
 *            en parametre. Le pointeur de la souris
 *            reste dans la meme position.
 ---------------------------------------------------------------*/

bVlispFonc lchangewin() /* (xChangeWin win flag) nsubr */

{
  Int flag = 0;
  Widget w ;
  Int *at = car(a1);

  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    a1 = cdr(a1); /* avancons */
    if(init_oneINT(&flag) == ERROR) ;
    if(flag == 0)
      posPointerCentreWin(w);
    Xbvlisp_courr = w;
    lcou = ((XbvlispWidget)w)->xbvlisp.colonnes -  CARS_MARGE_DROITE;
    a1 = t;
  } else a1 = nil;
  derec;
}

  
/*---------------------------------------------------------------
 * retourne une liste avec les dimensions et la position,
 * en pixels, de la fenetre passee en argument.
 ---------------------------------------------------------------*/

bVlispFonc ldim()   /* (xwdim win) nsubr */
{
  
  Widget w ;
  Int *at = car(a1);
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    XbvlispWidget wbvl = (XbvlispWidget)w;
    Position posx, posy;
    XteGetValue(w, XtNx, (XtArgVal *)&posx);
    XteGetValue(w, XtNy, (XtArgVal *)&posy);
    
    a1 = cons(cranb(wbvl->xbvlisp.colonnes),
	      cons(cranb(wbvl->xbvlisp.lignes),
		   cons(cranb((Int)posx),
			cons(cranb((Int)posy),
			     cons (cranb(wbvl->xbvlisp.pages), nil)))));
    } else a1 = nil;
  derec;
}


/*---------------------------------------------------------------
 * Ramene L'atome associe a l'interface d'interaction courante
 ---------------------------------------------------------------*/

bVlispFonc lwin(){ /* (xwinp) SUBR0 ramene l'atome image  de la fenetre courante */
  if(Xbvlisp_courr == NULL ||
     (a1 = (Int *)lookfor_atome(((XbvlispWidget)Xbvlisp_courr)->core.name)) == NULL) a1 = (Int *)nil;

  derec;
}

/*---------------------------------------------------------------
 * retourne t si le widget passee en parametre est une interface
 *              d'interaction, sinon ramene ni.
 ---------------------------------------------------------------*/

bVlispFonc lwindowp(){ /* (xwindowp win) nsubr */
  Int *at = car(a1);
  Widget w;
  
  if(isWidget(at)
     && (w = GET_WIDGET_FROM_ATOME(at))
     && (XtIsSubclass(w, xbvlispWidgetClass))) {
    a1 = t;
  }else a1 = nil;
  derec;
}

/*------------------------------------------------------------------
 * extrait quelques ressourses pour la widgette passe
 * en parametre.
 ------------------------------------------------------------------*/
void getXbvlRessources (interface,  win_data)
     Widget interface;
     xbVlispRess *win_data;
{
  
  /*            RESOURCES
   * Table Resources, decrivant les ressources
   * specifiques a bVLISP et ou les recuperer.
   */
  
  static XtResource resources[] = {
    {
      XtNfont, XtCFont,
      XtRFontStruct, sizeof(XFontStruct *),
      XtOffset(xbVlispRessPtr, font),
      XtRString,"9x15",
    },
    {
      XtNpages, XtCPages,
      XtRInt, sizeof(Int),
      XtOffset(xbVlispRessPtr, pages),
      XtRImmediate, (caddr_t) DEFAULT_PAGES,
    },
    {
      XtNcolonnes, XtCColonnes,
      XtRInt, sizeof(Int),
      XtOffset(xbVlispRessPtr, colonnes),
      XtRImmediate, (caddr_t) DEFAULT_COLONNES,
    },	
    {
      XtNlignes, XtCLignes,
      XtRInt, sizeof(Int),
      XtOffset(xbVlispRessPtr, lignes),
      XtRImmediate, (caddr_t) DEFAULT_LIGNES,
    },
  };
  
  win_data->font = NULL;
  XtGetApplicationResources(interface,
			    (caddr_t)win_data,
			    (XtResourceList)resources,
			    (Cardinal)XtNumber(resources),
			    (ArgList)NULL, (Cardinal)0);
  if(!win_data->font) 
    XtErrorMsg("Application Font non Trouvée","Ressources", "Initialisation",
	       "Impossible de calculer la taille du widget",
	       NULL,0);
}

/* 
  Change tout le contenue du buffer du widget d'interface par la chaine passe en parametre.
  */

Int changeText(widget, string)
     Widget widget;
     CHAR *string;
{
  XbvlispWidget w = (XbvlispWidget)widget;
  XtTextBlock text;
#if 0
  char *tmp_s = string;
  Int t_lines = 0, i = 0, t_col = 0, t_pages = 0;
#endif

  if(! string ) return -1;

#if 0
  /* calcul de la taille du nouveau texte */
  while(tmp_s) {
    if(tmp_s == '\n') {
      if(i > t_col) t_col = i;
      ++t_lines;
    }
    ++tmp_s;
  }
#endif

  text.firstPos = 0;
  text.format = FMT8BIT;
  if(XtIsSubclass(widget, xbvlispWidgetClass)) {
#if 0
    if(w->xbvlisp.colonnes < t_col ||
       w->xbvlisp.pages < t_lines / w->xbvlisp.lignes) { /* fenêtre trop petite... */
      /* A TRAITER... */  
	}
#endif
    text.length = w->xbvlisp.lignes * w->xbvlisp.colonnes * w->xbvlisp.pages ;
    text.ptr = calculXbvlEditBuffer(w->xbvlisp.lignes, w->xbvlisp.colonnes, w->xbvlisp.pages, string );
  }
  else if(XtIsSubclass(widget, asciiTextWidgetClass)) {
    Arg wargs[3];
    XFontStruct *font = NULL;
    Int i = 0, width, height, lines, colonnes; 
    XtSetArg(wargs[i], XtNfont, &font); i++;
    XtSetArg(wargs[i], XtNwidth, &width);   i++;
    XtSetArg(wargs[i], XtNheight, &height); i++;
    XtGetValues(widget, wargs, i);
    
    colonnes = width / FONTWIDTH(font);
    lines = height / FONTHEIGHT(font);
    text.length = strlen(string);
    text.ptr = calculXbvlEditBuffer(lines, colonnes, 1, string);
  }

  eXtTextReplace(widget, (XtTextPosition) 0, 
		 (XtTextPosition) text.length,
		 &text);
  XtTextInvalidate(widget, 0, (XtTextPosition) text.length);
  XtFree(text.ptr);
  return True;
}
