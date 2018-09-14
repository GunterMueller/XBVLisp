/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 	xcallbks.c
 * 	Ernesto Sendoya		 Avril 1991
 */

#include <vlisp.h>

#ifdef STDC_HEADERS
static CHAR * expandExpression(Widget w, CHAR *xbvl_mess, caddr_t data);
#else
static CHAR * expandExpression();
#endif

/*
 * nouvelle interface
 ARGSUSED */
void cree_new_window(w, app_data, call_data)
     Widget w;
     caddr_t app_data,  call_data;
{
  static Int n = 0;
  CHAR name[72], root_name[128];
  
  sprintf(name,"%s%02d", WINS_NAMES, n++);
  sprintf(root_name, "root-%s", name);

  xCreateXbVlispWidget(root_name, name,
		       xvl_data.lignes,
		       xvl_data.colonnes + 1,
		       xvl_data.pages,
		       -1, -1);
}


/*
 * nouvelle interface
ARGSUSED */
void stop_xbvl(w, app_data, call_data)
     Widget w;
     caddr_t app_data,  call_data;
{
  stop();
}

/*
 * nouvelle interface
ARGSUSED */
void break_xbvl(w, app_data, call_data)
     Widget w;
     caddr_t app_data,  call_data;
{
  a1=nil;  Break(); 
}
  

/* D.P. Fixe la valeur de pbind (si possible) pour que la fonction */
/* appelante soit la fonction creatrice du widget */
void fixeBind(w)
Widget w;
{
  CHAR *u_ptr = XeWidgetName(w); /* recuperation du nom du widget */
  Int  *atw = (Int*)lookfor_atome(u_ptr), *at_creation;

  if(!atw) return; /* l'atome n'a pas ete trouve */
  at_creation=find((Int)atcrewid, (Int *)((struct atome *)atw)->ival);
  if(!at_creation) return; /* la fonction creatrice n'est pas presente */
  *atcrewid=(Int)car(cdr(car(at_creation))); /* ((crewid <fonction>)) */
}

/*
 * ce callback est appele avec un message destine a XBVL comme
 * Argument.
 */
/* ARGSUSED */
void comToXbvl(wind, xbvl_mess, call_data)
     Widget wind;           /* la widgette associee a l'appel */
     String xbvl_mess;       /* argument de la fonction */
     caddr_t call_data;       /* non utilisee */
{
#ifdef STDC_HEADERS
  extern Boolean getExprFromXSystem(CHAR **buf);
#else
  extern Boolean getExprFromXSystem();
#endif
  CHAR *expression;
  fixeBind(wind);
  expression = expandExpression(wind, xbvl_mess, call_data);
  communiqExprToVlisp(XtNewString(expression));
}


static CHAR * expandExpression(w, xbvl_mess, usr_data)
     Widget w;
     CHAR *xbvl_mess;
     caddr_t usr_data;
{

#ifdef XtSpecificationRelease
#undef index
#endif

  static CHAR callBuffer[MAX_COLS];
  
  CHAR *ptr, *u_ptr, tab[15];
  Int i = 0;
  /*
   * on insere les arguments remplacant le macros suivants :
   * $O par la  chaine selectionnee: le nom de l'objet pointe
   * $I par l'index de la chaine selectionnee dans un widget awList
   * $W par le widget
   * $# par l'indice de l'objet dans la liste.
   * $X par la positon en x
   * $Y par la positon en y
   * $L par la largeur du widget
   * $H par la hauteur du widget
   */
  
  while(*(callBuffer + i))  *(callBuffer + i++) = 0; /* netoyer le buffer */
  ptr = callBuffer;
  while( *xbvl_mess ){
    if(*xbvl_mess == '$' ){
      if(*(xbvl_mess + 1) == 'O'){
	if(XtIsSubclass(w, listWidgetClass) && usr_data)
	  u_ptr = ((XtListReturnStruct *)usr_data)->string;
	else u_ptr = XeWidgetName(w);
	while(*u_ptr) 
	  *ptr++ = *u_ptr++;
	xbvl_mess += 2;
      }
      if(*(xbvl_mess + 1) == 'I'){
	if(XtIsSubclass(w, listWidgetClass) && usr_data) {
	  static CHAR n_buf[10], *n_ptr;
	  sprintf((n_ptr = &n_buf[0]),"%d\0",((XtListReturnStruct *)usr_data)->list_index);
	  while(*n_ptr) *ptr++ = *n_ptr++;
	}
	xbvl_mess += 2;
      }
      if(*(xbvl_mess + 1) == 'W'){
	u_ptr = XeWidgetName(w);
	while(*u_ptr) 
	  *ptr++ = *u_ptr++;
	xbvl_mess += 2;
      }
      else if(*(xbvl_mess + 1 ) == '#'
	      && XtIsSubclass(w, listWidgetClass) && usr_data){
#ifdef XtSpecificationRelease
	sprintf(tab, "%d", ((XtListReturnStruct *)usr_data)->list_index);
#else
	sprintf(tab, "%d", ((XtListReturnStruct *)usr_data)->index);
#endif
	u_ptr = tab;
	while(*u_ptr) 
	  *ptr++ = *u_ptr++;
	xbvl_mess += 2;
      }
      else if(*(xbvl_mess + 1 ) == 'X'){
	Position x = 0;
	XteGetValue(w, XtNx, (XtArgVal * )&x);
	sprintf(ptr, "%d", (Int)x);
	ptr += strlen(ptr);
	xbvl_mess += 2;
      }
      else if(*(xbvl_mess + 1 ) == 'Y'){
	Position y = 0;
	XteGetValue(w, XtNy,(XtArgVal * ) &y);
	sprintf(ptr, "%d", (Int)y);
	ptr += strlen(ptr);
	xbvl_mess += 2;
      }
      else if(*(xbvl_mess + 1 ) == 'L'){
	Dimension width = 0;
	XteGetValue(w, XtNwidth,(XtArgVal * ) &width);
	sprintf(ptr, "%d", (Int)width);
	ptr += strlen(ptr);
	xbvl_mess += 2;
      }
      else if(*(xbvl_mess + 1 ) == 'H'){
	Dimension height = 0;
	XteGetValue(w, XtNheight,(XtArgVal * ) &height);
	sprintf(ptr, "%d", (Int)height);
	ptr += strlen(ptr);
	xbvl_mess += 2;
      }
      else 
	*ptr++ = *xbvl_mess++;
    }
    else 
      *ptr++ = *xbvl_mess++;
  }
  return callBuffer;
}

/* ----------------------------------------------------------------------*
 * nettoye la variable contenant le widget, utile si on detruit la fenetre
 * d'aide au cours d'une session interactive.
 *-----------------------------------------------------------------------*/
/* ARGSUSED */
void cleanVar(w, app_widget, call_data)
     Widget w;
     Widget * app_widget;
     caddr_t call_data;
{
  *app_widget = NULL;
}

