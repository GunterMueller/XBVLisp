
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* les fonctions de base pour gestion des tables de translations
 *               xTranslation.c
 *		Damien Ploix
 *		  janvier  93		
 */

#include <vlisp.h>
#include <Objets.h>
#include <Xbvlisp.h>

#ifdef STDC_HEADERS
extern void XtFree(char *);
#else
extern void XtFree();
#endif
/*extern char *XtMalloc(Int) ;*/

/* Fabrique une table de translation a partir d'une Translation
   et d'une expression Lisp 
 */
XtTranslations MakeTranslation(widget, trans, expr)
Widget widget ;
char *trans, *expr;
{
  char *StrTransTable = (char *) XtMalloc(strlen(trans) + 1 +
					strlen(expr) + 1 +
					30) ;
  XtTranslations TransTable ;
  if(!StrTransTable) return NULL ;
  sprintf(StrTransTable, "%s:  eval-expressions(\"%s\")", trans, expr) ;
  TransTable = XtParseTranslationTable(StrTransTable);
  XtFree(StrTransTable) ;
  return TransTable ;
}

/*---------------------------------------------------------------
 *	xAugmentTranslations   sbr3
 *      Augmente la table de translation de la widget passee en
 *      argument.
 ---------------------------------------------------------------*/
bVlispFonc xAugmentTranslation() /* (xAugment widgetat "<TranslationExpr>"
				                                   "<EXPRESSION>") */
{
  Widget widget ;
  CHAR *translation, *expression;

  if(isWidget (a1)) {
    widget = GET_WIDGET_FROM_ATOME(a1);
    if(getNameFromArg(&translation, a2) > 0 && 
       getNameFromArg(&expression, a3) > 0) {
	XtTranslations TransTable = MakeTranslation(widget, translation, expression);
	if(TransTable) XtAugmentTranslations(widget, TransTable);
      }
  }
  derec ;
}

/*---------------------------------------------------------------
 *	xOverride   sbr3
 *      Override la table de translation de la widget passee en
 *      argument.
 ---------------------------------------------------------------*/
bVlispFonc xOverrideTranslation() /* (xOverrideTranslation widgetat "<TranslationExpr>"
				     "<EXPRESSION>") */
{
  Widget widget ;
  CHAR *translation, *expression;

  if(isWidget (a1)) {
    widget = GET_WIDGET_FROM_ATOME(a1);
    if(getNameFromArg(&translation, a2) > 0 && 
       getNameFromArg(&expression, a3) > 0) {
	XtTranslations TransTable = MakeTranslation(widget, translation, expression);
	if(TransTable) XtOverrideTranslations(widget, TransTable);
      }
  }
  derec ;
}



