
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* les fonctions de base pour la creation des fenetres X
 *               xcrewin.c
 *		Ernesto Sendoya
 *		  janvier  91		
 */

#include <Objets.h>
#include <Xbvlisp.h>


/*
 * retourne un nouveau Widget D'interaction avec xbVLISP .
 */
CHAR *xbvl_string;
Widget CreateManagedXbVlispWidget(name, w_pere, wargs, n, ret_at)
     CHAR *name;
     Widget w_pere;
     ArgList wargs;
     Int n;
     struct atome ** ret_at;
{
  Int 
    colonnes = xvl_data.colonnes ,
    lignes =   xvl_data.lignes ,
    pages =  xvl_data.pages ;
  Arg xbvl_arg[10];
  ArgList merge_args;
  Widget xbvl_widget;
  CHAR *ptr = NULL;
  Int i;

  for(i = 0; i < n; i++){
    if(strcmp(XtNpages, wargs[i].name) == 0)
      pages = (Int)wargs[i].value ;
    else if(strcmp(XtNlignes, wargs[i].name) == 0)
      lignes = (Int)wargs[i].value;
    else if(strcmp(XtNcolonnes, wargs[i].name) == 0)
      colonnes = (Int)wargs[i].value ;
    else if(strcmp(XtNstring, wargs[i].name) == 0)
      ptr = (CHAR *)wargs[i].value;
  }

  if(pages < MIN_PAGES || lignes < MIN_LIGNES || colonnes < MIN_COLONNES) 
    return NULL;

  xbvl_string = calculXbvlEditBuffer(lignes, colonnes, pages, ptr);
  i = 0;
  XtSetArg(xbvl_arg[i], XtNstring , xbvl_string);   i++;
  XtSetArg(xbvl_arg[i], XtNlength, lignes*colonnes*pages ); i++;
#ifndef XtSpecificationRelease
  XtSetArg(xbvl_arg[i], XtNeditType, XttextEdit);i++;
  if(pages > 1){ 
    XtSetArg(xbvl_arg[i],
	     XtNtextOptions , 
	     editable  | scrollOnOverflow | scrollVertical | scrollHorizontal);
    i++;
  }
  else {
    XtSetArg(xbvl_arg[i], XtNtextOptions, editable  | scrollOnOverflow );
    i++; 
  }
#else
  XtSetArg(xbvl_arg[i], XtNtype, XawAsciiString);i++;
  XtSetArg(xbvl_arg[i], XtNeditType, XawtextEdit);i++;
  /*  utiliser usrStringInPlace */
  XtSetArg(xbvl_arg[i], XtNuseStringInPlace, True);i++;
  if(pages > 1){
    XtSetArg(xbvl_arg[i], XtNscrollVertical, XawtextScrollWhenNeeded); i++;
    XtSetArg(xbvl_arg[i], XtNscrollHorizontal, XawtextScrollWhenNeeded);i++;
  }
#endif
  
  if(n)merge_args = XtMergeArgLists(wargs, n, xbvl_arg, i);
  else merge_args = xbvl_arg;
  if((xbvl_widget = XbvlCreateWidget(name, xbvlispWidgetClass, w_pere,
				     merge_args, i + n,  ret_at)) == NULL){
    /* DEBUG */
    return NULL;
  } 
  if(n)XtFree((String)merge_args);
  XtManageChild(xbvl_widget);
  return xbvl_widget;
}


/*
 * Creation d'un shell et d'un Widget d'interaction envelope par un conteneur Form.
 * Le changement de taille du widget d'interaction est possible a partir des 
 * ressources du widget d'interaction lui-meme. 
 * Retourne le nouveau widget d'interaction.
 */

Widget xCreateXbVlispWidget(root_name, xbvl_name,  lignes, colonnes, pages, x, y)
     CHAR *root_name;
     CHAR *xbvl_name;
     Int lignes, colonnes, pages, x, y;
{
  Arg wargs[5];
  Widget newroot, canvas, interface_xbvl ;
  Int i;
  CHAR canvas_name[128];
  
  i = 0;
  XtSetArg(wargs[i], XtNallowShellResize, TRUE);i++;  
  /* possitionnement du Shell */
  if(x >=0  && y >= 0 ){
    static CHAR *position; CHAR tab[50];

    sprintf(tab,"%+d%+d",x, y ); 
    position = XtNewString(tab);
    XtSetArg(wargs[i], XtNgeometry, position);i++; 
  }
  
  if(newroot = XbvlCreateApplicationShell(root_name, XBVL_APP_CLASSE_NAME,
					  wargs, i, (struct atome **)NULL)){
    /* 
      Un widget Form pour avoir la possibilite d'introduire des hierarchies
      complexes a partier de celle-ci.
      */
    sprintf(canvas_name,"form-%s", xbvl_name);
    canvas = XbvlCreateManagedWidget(canvas_name, formWidgetClass, newroot,
					(ArgList)NULL, 0, NULL );
    interface_xbvl =  xCreateManagedXbvl(xbvl_name, canvas,  lignes, colonnes, pages, xvl_data.font);
    XtRealizeWidget(newroot);
    return interface_xbvl;
  }
  return NULL;
}


/*
 * Cree un widget d'application avec son atome associe.
 * Retourne le widget cree.
 */

Widget XbvlCreateApplicationShell(root_name,Classe_Name,  wargs,  i, ret_atome)
     CHAR * root_name, *Classe_Name;
     ArgList wargs;
     Int i;
     struct atome ** ret_atome; /* pour retourner l'atome image du widget */
     
{
  Int n;
  Widget newroot;
  ArgList merged_args = NULL;
  Display *dpy = NULL;
  Int *at = (Int *)genUniqueAtome(root_name);
  
  /* 
    experimentation avec des objets Xt: 
    l'integration du "display" 
    comme une VRAIE ressource de chaque racine d'interface.
    */
  for(n = 0; n < i; n++){
    if(strcmp(wargs[n].name, XtNdisplay) == 0){
      Arg m_args[5];
      Int num;
      dpy = ((Int)wargs[n].value) ? (Display *)wargs[n].value : display;
      XtSetArg(m_args[num], XtNscreen, DefaultScreenOfDisplay(dpy)); num++;
      XtSetArg(m_args[num], XtNargc, vlargc);                     num++;
      XtSetArg(m_args[num], XtNargv, vlargv);	                 num++;
      wargs = merged_args = XtMergeArgLists(wargs, i, m_args, num);
      i += num;
      break;
    }
  }
  if(! dpy ) dpy = display;
  if(newroot = XtAppCreateShell(STRPNAME(at), Classe_Name,
				applicationShellWidgetClass,
				dpy, wargs, i)){
    if(ret_atome) *ret_atome = (struct atome *)at;
    interneObjetWidget((struct atome *)at, newroot);
  }
  if(merged_args)XtFree((String)merged_args);
  /* pour DEBUG */
  if(!newroot)
    printf("ERRor, Impossible de creer l'ApplicationShell <%s>\n", STRPNAME(at));
  return newroot;
}

/*
 * Cree un widget normal avec son atome associe.
 * Retourne le widget cree et situe sur l'autorite parental.
 */

Widget XbvlCreateManagedWidget(widget_name, widget_class, w_pere, args_list, n, ret_atome)
     CHAR *widget_name;
     WidgetClass widget_class;
     Widget w_pere;
     ArgList args_list;
     Int n;
     struct atome ** ret_atome;
{
  Widget newwidget = XbvlCreateWidget(widget_name, widget_class, w_pere, args_list, n, ret_atome);
  
  if(newwidget && !(IS_POPUP_CLASS(widget_class)))
    XtManageChild(newwidget);
  return newwidget;
}
     
/*
 * Cree un widget normal avec son l'atome associe.
 * Retourne le widget cree et situe sur l'autorite parental.
 */

Widget XbvlCreateWidget(widget_name, widget_class, w_pere, args_list, n, ret_atome)
     CHAR *widget_name;
     WidgetClass widget_class;
     Widget w_pere;
     ArgList args_list;
     Int n;
     struct atome ** ret_atome;
{
  Widget newwidget;
  Int *at = (Int *)genUniqueAtome(widget_name);
  
  if(IS_POPUP_CLASS(widget_class))
    newwidget =  XtCreatePopupShell(STRPNAME(at), widget_class, w_pere, args_list, n);
  else 
    newwidget =  XtCreateWidget(STRPNAME(at), widget_class, w_pere, args_list, n);
  if(newwidget){
    if(ret_atome) *ret_atome = (struct atome *)at;
    interneObjetWidget((struct atome *)at, newwidget);
  }  /* pour DEBUG */
  else  {
    printf("ERRor, Impossible de creer widget <%s>\n", STRPNAME(at));
    if(ret_atome)*ret_atome = (struct atome *)nil;
  }
  return newwidget;

}
     

/*
 * Pour faciliter la creation d'un widget d'interaction et son atome associe.
 */

Widget xCreateManagedXbvl(name, pere,  lignes, colonnes, pages, font)
     CHAR *name;
     Widget pere;
     Int lignes;
     Int colonnes;
     Int pages;
     XFontStruct *font;
{
  Int i;
  Arg argg[5];
  Widget xbvl_w;
  
  i = 0;
  XtSetArg(argg[i], XtNcolonnes, colonnes); i ++;
  XtSetArg(argg[i], XtNlignes, lignes); i ++;
  XtSetArg(argg[i], XtNpages, pages); i ++;
  XtSetArg(argg[i], XtNfont, font); i ++;
  XtSetArg(argg[i], XtNresizable, TRUE); i ++;
  xbvl_w = CreateManagedXbVlispWidget(name,  pere, argg, i, NULL);
  return xbvl_w;
}

