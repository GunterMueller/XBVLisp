/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 	     XtToLISP.c
 * 	   Ernesto Sendoya
 *     Ici on definit une fonction Important
 *   pour faire le couplage d'objet X et types
 *         de donnees LISP.
 *      
 */

#include <vlisp.h>
#include <X11/IntrinsicP.h>


/*
 * ces deux fonctions introduisent les ressources
 * "parent" et "children" qui permettent l'unicite de
 * repressentation d'objet LISP et Xt.
 */


/*--------------------------------------------------------- 
 * ramene l'atome correspondant au widget pere du widget
 * passe en parametre.
 *---------------------------------------------------------*/

Int xWidgetParent()  /* (xWidgetParent at) subr1) */
{
  if(isWidget(a1)){ 
    /* toujour de problemes avec la version 3 de X11 */
#ifdef XtSpecificationRelease
    CoreWidget w =  (CoreWidget)GET_WIDGET_FROM_ATOME(a1);
    CHAR * pere = ((CoreWidget)w->core.parent)->core.name;
#else
    WidgetRec * w = (WidgetRec *)GET_WIDGET_FROM_ATOME(a1);
    CHAR *  pere = ((WidgetRec *)w->core.parent)->core.name;
#endif
    if(XtIsSubclass (w, applicationShellWidgetClass)) 
      a1 = nil;
    else if((a1 = (Int *)lookfor_atome(pere)) == NULL) a1 = nil;
  } else a1 = nil;
  if( my_Exec_Flag == 1) return;
  derec;
}

/* 
 * ramene la liste d'enfants d'un widget composite
 * qui sont differents du widget en deuxieme argument.
 */

void GetWidgetChildren(w, children, num_children, cw)
     Widget w;
     WidgetList children;
     Int num_children;
     Widget cw;
{
  CompositeWidget to = (CompositeWidget)w;
  Int * ataux, * atret = nil;
  
  while( --num_children >= 0 ){
    WidgetRec * widget = *(children + num_children);  /* CoreWidget au lieu de WidgetRec */
    CHAR *name = widget->core.name;
    
    if(cw == widget) continue;
    if((ataux = (Int *)lookfor_atome(name)) == NULL){
      /* 
	Le fait widget partie d'un widget composite lui meme. bVLISP permet un acces par
	a ce widget. Cet acces doit etre fait avec beaucoup de precaution puisque
	le widget est entierement manipule par les methodes de la classe de 
	son conteneur. On pourra appliquer reccursivement ce procede sur les 
	widgets dependants.
	On cree un atome dont le nom  exprime clairement cette relation de dependance.
	Le pere est le package du widget subordonne.
	*/
      extern Int DEJA;
      Int pack_courr = *atpackage;

      ataux = nil;
      *atpackage = (Int)lookfor_atome(to->core.name);
      if(*atpackage == (Int) NULL){/* DEBUG */
	printf("Error:atome associe au widget %s inexistant\n", to->core.name);
	*atpackage = (Int)nil;
      }
      genat(name);
      if(DEJA && isWidget(a5) 
	 && (Widget)widget != GET_WIDGET_FROM_ATOME(a5)){/* DEBUG */
	printf("Error:atome a associer a  %s est un widget Existant\n", name);
	ataux = nil;
      }
      else ataux = a5;
      if(ataux != nil)
	interneObjetWidget((struct atome *)ataux, widget );
      *atpackage = pack_courr;
    }
    atret = cons(ataux, atret);
  }
  a1 = atret;
}


#ifndef XtSpecificationRelease

CHAR *XeWidgetName(w)
     Widget w;
{
  return w ? ((WidgetRec *)w)->core.name : NULL ;
}

#endif
