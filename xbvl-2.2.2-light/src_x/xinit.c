/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *                xinit.c
 * Initialisation des Toolkits et creation de la
 *     premiere interface avec bVLISP
 *
 */

#include <vlisp.h>
#include <Objets.h>
#include <Xbvlisp.h>


Int LINES, COLS, WIDTH, HEIGHT; 


static  Widget formVlisp;

Display *display;


Widget top_widget, /* la widgette principale */
  menuVlisp; 	   /* pour l'affichage des menu bVLISP */ 	      

/* A:ELIMINER */

xbVlispRess  xvl_data;

  /*
   * on fait connaitre a l'application
   * des actions generales a toutes les widgets
   */

static XtActionsRec xbvl_actions[] = {
  {  "cree_bVLISP_window",  (XtActionProc) cree_bVLISP_window },
  {  "toggle_border_color", ( XtActionProc)  toggle_border_color },
  {  "toggle_background_color",  ( XtActionProc) toggle_background_color },
  {  "toggle_foreground_color",  ( XtActionProc) toggle_foreground_color },
  {  "placeMenu", ( XtActionProc) PlaceMenu},
  {  "stop", ( XtActionProc) stop_x},
  {  "enleve_aide", ( XtActionProc) enleve_aide},
  {  "liste_aide", ( XtActionProc) liste_aide},
  { "TITRE",  ( XtActionProc) TITRE },
  { "faire_beep", ( XtActionProc) faire_beep },
  { "select_aide", ( XtActionProc) select_aide},
  { "liste_aide", ( XtActionProc) liste_aide},
  { "enleve_aide", ( XtActionProc) enleve_aide},
  { "eval-expressions", ( XtActionProc) communiqExpressions},
};


/* options pour parser la ligne de commande */
/* et positionner les ressources specifiques a xbvl */

static XrmOptionDescRec xbvl_options[] = {
  { "-pages",    "*Pages",     XrmoptionSepArg, NULL},
  { "-pgs",       "*Pages",    XrmoptionSepArg, NULL},
  { "-colonnes", "*Colonnes",  XrmoptionSepArg, NULL},
  { "-cols",     "*Colonnes",  XrmoptionSepArg, NULL},
  { "-lignes",   "*Lignes",    XrmoptionSepArg, NULL},
  { "-lgs",      "*Lignes",    XrmoptionSepArg, NULL},
};


XtTranslations translations ;

/* -----------------------------------------------------*
 *	cree le widget pour Menu d'aide VLISP		*
 * -----------------------------------------------------*/
void cree_menuVlisp()
{
  Arg margs[10];     /* arguments pour changer les defauts des widgettes */
  XFontStruct *menufont;
  Int i ; 

  static char menu_translations[] =
    "<BtnDown>:placeMenu() MenuPopup(std-pshell) \n\
<Leave>:toggle_border_color() \n\
<Enter>:toggle_border_color() \n\
<Key>F1:cree_bVLISP_window()  \n\
<Key>F2:liste_aide() \n\
<Key>F3:enleve_aide()  \n\
<Key>F4:stop()";

  formVlisp = XbvlCreateManagedWidget("xbvl-form",
				      getWidgetClassFromString(XtNawForm)  ,
				      top_widget ,
				      (ArgList)NULL, 0,
				      (struct atome **)NULL );
  init_pop_up_menu(top_widget);

  i = 0;
  XtSetArg(margs[i], XtNtop, XtChainTop); i ++;
  XtSetArg(margs[i], XtNbottom, XtChainTop); i ++;
  menuVlisp = XbvlCreateWidget("xbvl-label",
			       getWidgetClassFromString(XtNawLabel)  ,
			       formVlisp ,
			       margs, i,
			       (struct atome **)NULL);

  /* la variable est mise a NULL si le widget est detruit, pour garder la
     coherence de l'interface ce lien doit etre cree */
  XtAddCallback(menuVlisp, XtNdestroyCallback,(XtCallbackProc) cleanVar, (caddr_t)&menuVlisp ); 
  
  XtOverrideTranslations(menuVlisp,  XtParseTranslationTable(menu_translations));
  XteGetValue(menuVlisp, XtNfont, (XtArgVal *)&menufont);
  i = 0;
  XtSetArg(margs[i], XtNwidth, (Dimension) FONTWIDTH( menufont) * 80);i++;
  XtSetArg(margs[i], XtNmaxHeight , (Dimension)FONTHEIGHT(menufont) );i++;
  XtSetValues(menuVlisp,margs, i);
  
  XtManageChild(menuVlisp);
}
/* -----------------------------------------------------*
 *	cree les premiers widgettes			*
 * -----------------------------------------------------*/

/* #include "menu" * l'icon du menu */
void X_init()
{
  extern void RegisterConverters(), initObjets(), initWidgetClasses();
  Arg margs[10];     /* arguments pour changer les defauts des widgettes */
  Dimension interface_width, menu_width;
  int i, tmp_argc = vlargc; /* pour ne pas modifier vlargc */
  struct atome *rep_atome;
  static char my_name[20];

  if(top_widget) return; /* ou cas ou (config nombres chaines atomes listes piles) soit appele */

  /* initialisation des objets manipulés par l'interface */
  (void)initWidgetClasses();
  (void)initObjets();
  (void)init_quarks();
  (void)RegisterConverters();

  /* initialisation de l'interface par defaut */
  /* essai de création de l'interface via un fichier Lisp ... */

  rep_atome = genUniqueAtome(&main_top_name[0]); 
  top_widget = XtInitialize(getAtomName(rep_atome), TOP_APP_CLASS,
			    xbvl_options,          /* options pour parser */
			    XtNumber(xbvl_options),/* la ligne de commande */
			    &tmp_argc, vlargv);
  if(!top_widget){
    printf("Impossible d'initialiser des objets X-SYSTEME\n");
    exit(1);
  }
  /* permettre au top de changer de taille */
  XteSetValue(top_widget, XtNallowShellResize, True); 

  /* POUR INTERNER LE PREMIER WIDGET: */
  interneObjetWidget(rep_atome, top_widget);

  /* initialisations de constantes A:eliminer */
  display = XtDisplay(top_widget);
  /* configuration des ressources de l'application */
  /* initialisation des defauts */

  getXbvlRessources(top_widget,  &xvl_data);

  cree_menuVlisp();

  XtAddActions(xbvl_actions, XtNumber(xbvl_actions));

  /* le premier widget d'interaction */
  Xbvlisp_courr =  xCreateManagedXbvl("main-bVLISP",
				      formVlisp,
				      xvl_data.lignes, 
				      xvl_data.colonnes, 
				      xvl_data.pages,
				      xvl_data.font);
  i = 0;
  XtSetArg(margs[i], XtNtop, XtChainTop); i ++;
  XtSetArg(margs[i], XtNfromVert,  (XtArgVal)menuVlisp); i ++;
  XtSetValues(Xbvlisp_courr, margs, i);

  /* des dimensions correctes pour la premiere fenetre d'interaction */
  XteGetValue(Xbvlisp_courr, XtNwidth, (XtArgVal *)&interface_width);
  XteGetValue(menuVlisp, XtNwidth, (XtArgVal *)&menu_width);

  if(menu_width < interface_width)
    XteSetValue(menuVlisp, XtNwidth, interface_width);
  else if(menu_width > interface_width)
    XteSetValue(Xbvlisp_courr, XtNwidth, menu_width);

  XtManageChild(menuVlisp);
  XtRealizeWidget(top_widget);
  WIDTH = (Int)DisplayWidth(display, DefaultScreen(display)); 
  HEIGHT =(Int) DisplayHeight(display, DefaultScreen(display));
  COLS = (Int) WIDTH/FONTWIDTH(xvl_data.font);
  LINES =(Int) HEIGHT/FONTHEIGHT(xvl_data.font);
}


