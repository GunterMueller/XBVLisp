/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *               xSetValues.c
 * fonctions de base VLISP pour changer les ressources des fenetres
 * des interfaces.
 *
 *               Ernesto Sendoya
 *                avril  91		
 */


#include <vlisp.h>
#include <Objets.h>

/*  manipulation de Widgets */
/* possitionne la ressource du widget contenu dans l'atome passe en argument */
Int xSetValues()      /* (xSetValuesw atwidget "nomressouce" "type_ressource" val_ressorce ) */
{
  Widget widget = getWidgetFromAtome();
  
  if(widget){
    ArgList argg = NULL;
    Int n, is_resize ;
    
    is_resize = getArgListForWidget(widget, &argg, &n);
    if(n){
      XtSetValues(widget, argg, n);
      XtFree((String)argg);
      a1 = (Int *) valnb((Int *) n);
      if(is_resize && XtIsSubclass(widget, drawxbvlWidgetClass)) 
	DrawxbvlResize((DrawxbvlWidget)widget);
    }else a1 = nil;
  }  else a1 = nil;
  
  derec;
}

/*
 * change la ressource 'list' d'un widget awList a partir
 * d'une chaine de caracteres. La chaine est convertie en
 * un tableau de chaines, une chaine par mot. La ressource
 * est changee apres la conversion.
 */
Boolean changeWidgetListOfString(widget, str)
     Widget widget;
     String str;
{
  String * string_list;
  Int free;

  if(! (string_list = (String *)  getValueString(str, XtRStringList, &free)))
    return False;
  return changeWidgetList(widget, string_list);
}

  

/*
 * change la ressource 'list' d'un widget list si la nouvelle
 * liste est diferente. Retourne True si la liste est changee, False sinon.
 */
Boolean changeWidgetList(lwidget, list_o)
     Widget lwidget;
     String *list_o;
{
  
  String * vielle_liste = NULL;
  Arg wargs[5];

  Int i = 0;
  XtSetArg(wargs[i], XtNlist, &vielle_liste );i++;
  XtGetValues(lwidget , wargs, i);
  
  /* comparer la vielle et nouvelle liste */
  i = 0;
  while(vielle_liste[i] && list_o[i] 
	&& ! strcmp(vielle_liste[i], list_o[i]) )
    i += 1;
  if(!vielle_liste[i] && ! list_o[i]){
    /* libere la nouvelle liste */
    FreeStringTable(list_o);
    return False;
  }
  i=0;
  XtSetArg(wargs[i], XtNlist, (XtArgVal)list_o );i++;
  XtSetArg(wargs[i], XtNnumberStrings, 0); i++;
  XtSetArg(wargs[i], XtNlongest, 0); i++;
  XtSetValues(lwidget , wargs, i);
  /* et liberer l'ancienne liste */
  FreeStringTable(vielle_liste);

  return True;
}


/*----------------------------------------------------------
 * modifi une ressource d'un widget.
 ----------------------------------------------------------*/

void XteSetValue(widget, name, lvalue)
     Widget widget;
     String name;
     XtArgVal lvalue;
{
  Arg arg[1];
  XtSetArg(arg[0], name, lvalue);
  XtSetValues(widget, arg, 1);
}
