/* %I%, %R%, %B%, %D%,  %T%,  %M% */

/*
 *             xmenu.c
 * Creation du menu pop-up. attachee 
 * a la premiere interface du systeme
 *                Ernesto Sendoya janvier 1992
 */


#include <stdio.h>
#include <vlisp.h>



/* le  shell popup est global */

static Widget pshell;


/*
 * action,  dialog button
ARGUSED */
void PlaceMenu(w, event, params, num_params)
     Widget w;
     XButtonEvent *event;
     String *params;
     Cardinal *num_params;
{
  Arg arg[2];
  Int i;

  /* situe le popup shell dix pixels au-dessus et a gauche de la position
   * du pointeur de la souris, le widget n'est pas encore visible  */
  i = 0;
  XtSetArg(arg[i], XtNx, event->x_root - 10);  i++;
  XtSetArg(arg[i], XtNy, event->y_root - 10);  i++;
  XtSetValues(pshell, arg, i);
}

/*
 * Cree le pop-up attache a la premiere interface de dialogue 
 * avec l'interprete. Ce pop-up est configurable dynamiquement
 * puisque tous ses elements sont accessibles par le programmeur.
 */
void init_pop_up_menu(top_widget)
     Widget top_widget;
{
  Widget  menuform,  menubox, menulabel, menupane[10];
  Int i;
  Dimension label_width;
  Arg maval[3];

  /* pour fixer les tables de traductions */
  XtTranslations translatCommands ;
  static char pshellTranslat[] = "<BtnUp>:MenuPopdown(std-pshell)";
  static char CommandTranslat[]="<EnterWindow>:highlight()\n <LeaveWindow>:reset()\n <BtnUp>:set() notify() unset() ";

  i = 0;
  XtSetArg(maval[i], XtNallowShellResize, True); i ++;
  pshell = XbvlCreateWidget("std-pshell",
			    getWidgetClassFromString( XtNOverrideShell),
			    top_widget,
			    maval, i, 
			    (struct atome **)NULL);
  XtOverrideTranslations(pshell,  XtParseTranslationTable(pshellTranslat));
  
  menuform = XbvlCreateManagedWidget("std-menu-form",     /* widget name   */
				     getWidgetClassFromString(XtNawForm),     /* widget class */
				     pshell,              /* parent widget*/
				     (ArgList)NULL,0,     /* arglist size */
				     (struct atome **)NULL/* pas besoin de connaitre l'atome */
				     );
  
  menubox = XbvlCreateManagedWidget("std-menu-box",      /* widget name   */
				    getWidgetClassFromString(XtNawBox) ,      /* widget class */
				    menuform,            /* parent widget*/
				    (ArgList)NULL,0,     /* arglist size */
				    (struct atome **)NULL/* pas besoin de connaitre l'atome */
				    );
  
  menulabel = XbvlCreateManagedWidget("std-menu-label",      /* widget name   */
				      getWidgetClassFromString(XtNawLabel),      /* widget class */
				      menubox,               /* parent widget*/
				      (ArgList)NULL, 0,      /* arglist size */
				      (struct atome **)NULL  /* pas besoin de connaitre l'atome */
				      );
  
  XteSetValue(menulabel, XtNlabel, (XtArgVal)" Menu Principal ");

  /* meme largeur pour les boutons de commandes et le label */
  XteGetValue(menulabel, XtNwidth, (XtArgVal *)&label_width);
  XtSetArg(maval[0], XtNwidth, label_width);
  translatCommands = XtParseTranslationTable(CommandTranslat);

  for (i = 0; i < 5; i++) {
    char *buf, name[50];
    switch(i){
                  
    case 0: buf = "Installe l'aide "; break;
    case 1: buf = " Enlève l'aide  "; break;
    case 2: buf = "Nouvelle fenêtre"; break;
    case 3: buf = "     Arret      "; break;
    case 4: buf = " Sortir de Xbvl "; break;
    }
    sprintf(name, "std-command%d", i);
    XtSetArg(maval[1], XtNlabel, (XtArgVal)buf);
    menupane[i] = XbvlCreateManagedWidget(name,  getWidgetClassFromString(XtNawCommand),
					  menubox, maval, 2, (struct atome **)NULL);
    XtOverrideTranslations(menupane[i], translatCommands );
  }
  XtAddCallback(menupane[0], XtNcallback,(XtCallbackProc) liste_aide, NULL);
  XtAddCallback(menupane[1], XtNcallback,(XtCallbackProc) enleve_aide, NULL); 
  XtAddCallback(menupane[2], XtNcallback,(XtCallbackProc) cree_new_window, NULL);
  XtAddCallback(menupane[3], XtNcallback,(XtCallbackProc) jump_start , NULL);
  XtAddCallback(menupane[4], XtNcallback,(XtCallbackProc) stop_xbvl , NULL);
}




