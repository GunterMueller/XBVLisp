/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 
 *            xkill.c
 * les fonctions de base pour tuer des fenetres X
 */

#include <vlisp.h>








Int my_Exec_Flag = False;

/* fonctions */

Int xkill();        /* (xRemoveWidget  nomtop {win})  */
Int xkillInterf();  /* (xdelsub  nomtop {wind}) */


/*---------------------------------------------------------------
 *	Tue la fenetre passee en parametre
 *      il est interdit de tuer la fenetre active
 *	NSBR0
 ---------------------------------------------------------------*/

Int xRemoveWidget()/* (xRemoveWidget  nomtop {win})  */
{
  Int n;
  Widget aeliminer =  get_widget(&n);
  
  a1 = nil;
  if(aeliminer && aeliminer != top_widget){
#ifdef XtSpecificationRelease
    if(XtIsSubclass(aeliminer, tortueObjectClass))
      destroyTortue(aeliminer);
    else 
#endif
      XteDestroyWidget(aeliminer);
    a1 = t;
  }
  derec;
}


/* 
 * detruit une Widgette
 * et traite tous les evenements generes,
 * ou qui sont dans la queue.
 */

void  XteDestroyWidget( wid )
     Widget wid;
{
  
  XtDestroyWidget(wid);
#if 0
  /* pas vraiment necessaire */
  XSync(display, False);
  XteLoop();
#endif
  return ;
}


