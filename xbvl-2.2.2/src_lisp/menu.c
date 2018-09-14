
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	menu.c		fonctions speciales
 *			Patrick Greussay  27 Oct 82
 *			Harald Wertz 7. Feb. 83

 *                      Ernesto Sendoya Juil. 92
 */

#include <vlisp.h>


CHAR titre1[]=
  "^w menu, ^f -->, ^b <--, ^d delete -->, delete <--, <esc> arret bvlisp ";
CHAR titre2[]=
  "^g delete next atome, ^i delete prev. atome, ^k kill, F1 select aide, F2 aide";
CHAR titre3[]=
  "^y yank, ^p haut ligne, ^n down ligne, ^a debut de ligne, ^e fin de ligne";
CHAR titre4[]=
  "^v next page, ~v prev-page, ~< home, ~> end, ^z scroll up, ~z scroll down";
CHAR titre5[]=
  "~] forward paragraph, ~[ backward-paragraph, <inser> Insert, F4 Insert";


#ifdef STDC_HEADERS
static Boolean afficheMenu( Widget w,  CHAR *m);
#else
static Boolean afficheMenu();
#endif

/*
      Le nouveau environnement graphique de bVLISP 
      le redessin du systeme de menus pour 
      en integrer de menus pop-up. Pour l'instant on 
      se limite a l'implementation de la cohexistence de 
      plusieurs menus affiches sur plusieurs objets d'interface.
*/

/*
 * affiche le contenu du menu dans le widget passe en parametre,
 * la foction fait des affichages sur des widgets appartenant
 * a classes diferentes.
               a faire: realiser l'affichage directement sur un
	          widget AscciiText au lieu d'un widget Xbvlisp.
		  Augement les objets d'affichage de menu.
 */
static Boolean afficheMenu(widget, menu)
     Widget widget;
     CHAR *menu;
{
  if(!widget)return False;
  if(XtIsSubclass(widget, labelWidgetClass))
    XteSetValue(widget, XtNlabel, (XtArgVal)menu);
  else if(XtIsSubclass(widget, xbvlispWidgetClass))
    InsertStringXbvlisp(widget, menu);
  else if(XtIsSubclass(widget, drawxbvlWidgetClass))
    afficheMenuToDraw(widget, menu);
  else if(XtIsSubclass(widget, listWidgetClass)){
    if(changeWidgetListOfString(widget, menu) == True){
      /* une seule ligne */
      Int numberStrings;
      XteGetValue(widget, XtNnumberStrings, (XtArgVal *) &numberStrings);
      XteSetValue(widget, XtNdefaultColumns, numberStrings);
    }
  }
  else return False;
  return True;
}

Int titr1()
{
  TITRE(NULL, NULL, NULL, NULL);
  derec; 
}
/* ARGSUSE */
XtActionProc TITRE(w, e,params, num_params)
     Widget w;
     XEvent *e;
     String *params;
     Cardinal * num_params;
{
  if(titel== titre1) titel= titre2;
  else if(titel== titre2) titel= titre3;
  else if(titel== titre3) titel= titre4;
  else if(titel== titre4) titel= titre5;
  else titel= titre1;
  afficheMenu(menuVlisp, titel);
}

Int titre(){
  pc=titr1;
}

Int stdmenu() /* (stdmenu)SUBR1 */
{
  Widget menu_o;

  if(isWidget(a1))
    menu_o = GET_WIDGET_FROM_ATOME(a1);
  else menu_o = menuVlisp;
  afficheMenu(menu_o, titre1);
  derec; 
}

Int makem1()
{
  *atmenu = (Int)a1;a1=t;
  derec;
}

Int makem2()
{
  a2=a1;rec(nconc,makem1);
}

Int makemenu(){/* FSUBR */
  rec(copy,makem2);
}

Int menu()/* (menu {-widget-} nil/menu )  SUBR2 */
{
  Widget menu_output;
  
  if(isWidget(a1)){
    menu_output = GET_WIDGET_FROM_ATOME(a1);
    if(isnil(a2)){
      if(isnil((Int*)*atmenu)){ a1 = nil;derec; }
      a1 = car((Int *)*atmenu);
      *atmenu=(Int)cdr((Int *)*atmenu);
    }
    else a1 = a2;
  }
  else{
    menu_output = menuVlisp;
    if(isnil(a1)){
      if(isnil((Int*)*atmenu)) {a1 = nil; derec;}
      a1 = car((Int *)*atmenu);
      *atmenu=(Int)cdr((Int *)*atmenu);
    }
  }
  afficheMenu(menu_output, STR_FROM_STRLSP(a1));
  derec; 
}
  
