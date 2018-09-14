/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	misc.c		miscelaneus
 *                      Ernesto Sendoya
 */

#include <vlisp.h>


extern char *getenv();

/*
 * libere un tableau de chaines de caracteres,
 *   le tableau doit etre alloue avec malloc.
 */
void FreeStringTable( string_table)
     String * string_table;
{
  Int i;

  for(i = 0; string_table[i]; i ++)
    XtFree(string_table[i]);
  XtFree((char*)string_table);
}

/*
 * fait la conversion le nombre bvlisp en chaine
 * retourne la longeur de la chaine.
 */

Int numToString(buf, bvlnum)
     CHAR *buf;
     Int *bvlnum;
{
#ifndef NO_VFLO
  return isflo(bvlnum) ?  sprintf(buf, "%f", (float)valflo(bvlnum)):
    sprintf(buf , "%d", (Int)valnb(bvlnum));
#else
    return sprintf(buf , "%d", (Int)valnb(bvlnum));
#endif
}

/*
 * 
 */

FILE *fopenFile(var_name, default_name)
     CHAR * var_name, * default_name;
{
  CHAR *env, *name;
  FILE *ff ;

  if(var_name && (env = getenv(var_name)))
     name = env;
  else 
    name = default_name;

  if((ff = fopen(name, "r")) == NULL){
    CHAR mess[512];
    sprintf(mess,"File %s inexistante ", name);
    perror(name);
    wst(mess);
    XBell(display, 99);
  }
  return ff;
}

/*
 * 
 */

Int openFile(var_name, default_name)
     CHAR * var_name, * default_name;
{
  CHAR *env, *name;
  Int fild;

  if(var_name && (env = getenv(var_name)))
     name = env;
  else 
    name = default_name;

  if((fild = open(name, 0)) == 0){
    CHAR mess[512];
    sprintf(mess,"File %s inexistante ", name);
    perror(name);
    wst(mess);
    XBell(display, 100);
  }
  return fild;
}



/*---------------------------------------------------------------
 * vide la queue d'evenements du serveur
 * tres utile pour faire un changement immediat de la fenetre
 * courante depuis un programme lisp; si non les affichages sur
 * l'ancienne fenetre continuent le temps que les evenements 
 * generes soient dans la queue.
 ---------------------------------------------------------------*/

bVlispFonc xflush()
{
  XSync(display, False);
  XteLoop(); /* pour garantir que tous les evenements soient */
  XteLoop();          /* traites */
  derec;
}
/*---------------------------------------------------------------
 * Gele les evenements du clavier sur la fenetre passee en parametre,
 * si plusieurs interfaces recouvrent la fenetre sur laquelle le grab
 * a etet fait le repositionnement du curseur sur ces fenetres changera
 * la fenetre courante
 * NSUBR
 ---------------------------------------------------------------*/

bVlispFonc grab()/* (xGrab widget ) subr1*/
{
  if(isWidget (a1)){
  Widget widget =  GET_WIDGET_FROM_ATOME(a1);
    faireGrab(widget);
    a1 = t;
  } else a1 = nil;
  derec;
}
/*---------------------------------------------------------------
 * Gele les evenements du clavier sur la fenetre passee en parametre,
 * si plusieurs interfaces recouvrent la fenetre sur laquelle le grab
 * a etet fait le repositionnement du curseur sur ces fenetres changera
 * la fenetre courante
 * NSUBR
 ---------------------------------------------------------------*/

bVlispFonc xBeep()/* (xBeep widget ) */
{
  Int n ;
  
  if(isint(a1) &&(n = valnb(a1)) < 100 && n > -100)
    ;
  else n = 0;
  XBell(display, n);
  a1 = t;
  derec;
}

/*---------------------------------------------------------------
 * Libere le clavier et le pointeur d'un grab vient d'etre 
 * effectue SBRO
 ---------------------------------------------------------------*/

bVlispFonc ungrab()/* (xUngrab) */
{
  faireUnGrab();
  a1=t;
  derec;
}


/*---------------------------------------------------------------
 * endort bVLISP 
 ---------------------------------------------------------------*/ 
bVlispFonc xsleep()
{
  Int sec = 0;

  if(isnil(a1)) derec;
  sec = valnb(car(a1));
  sleep(sec);
  derec;
}


/*---------------------------------------------------------------
 *	Sauvegarde le contenu du buffer d'edition dans le fichier 
 *      passe en parametre.
 *	NSBR0
 ---------------------------------------------------------------*/

bVlispFonc xwsave()/* (xwsave nomtop wind fichero )*/
{/* A REFAIRE ? */
#if 0
  CHAR *fic;
  Int n;
  Widget widget;
  XINTERFACE *interfac = get_bvl_wind(&n);

  widget = interfac->interface;
  if(interfac && n == 2 && get_oneName(&fic) == True ){
    FILE *ffd = fopen(fic, "w");
    a1 = t;
    if(ffd == NULL){
      perror(fic);
      a1 = nil;
    }
    else {
      CHAR *editbuf;
      Int l; CHAR c;
      GET_STRING_VAL(editbuf, interfac);
      l =  strlen(editbuf);
      /* pour ne pas sauvegarder les blancs de fin fichier */
      while(l > 0 && isspace(c = *(editbuf + l)) ||  c =='\n')
	l -= 1;
      if(l)write(fileno(ffd), editbuf, l);
      write (ffd, "\n", 1);
      fclose(ffd);
    }
    XtFree((String)fic);
  }else a1 = nil;
#endif
  derec;
}
/* 
 *  Pour changer le mode d'interaction avec l'interprete.
 */


Int xEnable_focus();
Int xDisable_focus();


bVlispFonc xEnable()
{
  enable_change_focus();
  a1 = t;
  derec;
}


Int xDisable()
{
  disable_change_focus();
  a1 = t;
  derec;
}

/*---------------------------------------------------------------------*
 * comparaison lexicographique de deux chaines.
 * Utilise pour ordonner des tableaux des pointeurs vers chaines.
 *---------------------------------------------------------------------*/
Int  cmpsujs(psujet1, psujet2)
     String *psujet1, *psujet2;
{
  return strcmp( psujet1[0], psujet2[0]);
}

/*
 * Realise un Grab ( un gel du curseur ) sur la fenetre passe en parametre
 */
Int faireGrab(w)
     Widget w;
{

  XGrabKeyboard(display, XtWindow(w), GrabModeAsync,True, GrabModeAsync, CurrentTime);
  XGrabPointer(display, XtWindow(w),True, ButtonPressMask|ButtonReleaseMask, 
	       GrabModeAsync, GrabModeAsync,XtWindow(w), None, CurrentTime);
  return 1;
}
/* 
 * Realise un ungrab ( un degel du curseur ) sur la fenetre passee en parametre
 */

Int faireUnGrab()
{
  XUngrabKeyboard(display , CurrentTime);
  XUngrabPointer(display , CurrentTime);
}

/*
 * enleve tous les caracteres differents du blanc et du retour.
 * si flag == True; les compte seulement si flag == False
 */
Int netoy_text(ptr, length, mode)
     CHAR *ptr;
     Int length, mode;
{
  Int n, cnt;
  for(cnt = n = 0; n < length; ptr++, n++)
    if(*ptr == '\n'|| *ptr == ' ') ; /* on ne touche pas */
    else {
      cnt += 1;
      if(mode == True)	*ptr = ' ';
    }
  return cnt;
}

/*
 *  Prepare le kill buffer.
 *  Une limite imposee de BUFSIZ (512-1024) caracteres.
 */
Int save_texte(abuf, ptr, length)
     CHAR **abuf;
     CHAR *ptr;
     Int length;
{
  Int n;
  CHAR buffer[BUFSIZ + 1], *new;

  if(length <= 0) return 0;
  if(length > BUFSIZ) length = BUFSIZ;
  strncpy(buffer, ptr, length);
  
  for(n = 0; n < length;  n++)
    if(buffer[n] == '\n') 
      buffer[n] = ' ' ; /* on ne sauve pas les retours */
  while(n > 0)
    if(buffer[n - 1] == ' ') 
      n -= 1;
    else break;
  buffer[n] = 0;
  if(n){ /* qqc. a sauver */
    new = XtNewString(buffer);
    *abuf = new;
  }
  return n;
}



/* 
 * calcule le nbr. d'elements d'un tablau de chaines,
 * le tableau doit etre termine par la chaine nulle.
 */

Int GetStringsInfos(s_list, longest)
     String * s_list;
     Int *longest;
{
  Int numberStrings = 0;
  CHAR *str;

  *longest = 0;
  while(str = s_list[numberStrings]){
    Int l = strlen(str);
    if(*longest < l)
      *longest = l;
    numberStrings += 1;
  }
  return numberStrings;
}  

/*
 * regarde si la chaine est la representation
 * externe d'un entier.
 */

Boolean isInteger(str)
     CHAR *str;
{
  Int n = 0;

  if(*str == '-' || *str == '+')
    str += 1;
  while(*str && isdigit(*str)){
    str += 1;
    n += 1;
  }
  if (!*str && n) return True;
  return False;
}

/*
 * regarde si la chaine est la representation
 * externe d'un flottant.
 */
Boolean isFloat(str)
     CHAR *str;
{
  Int E = False, PoInt = False, n = 0;
  if(*str == '-' || *str == '+')
    str += 1;
  while(*str){
    if(*str == '.'){ /* un seule PoInt */
      if(PoInt == True) return False;
      PoInt = True;
    } 
    else if (*str == 'E' || *str == 'e'){
      if(E == True) return False;
      if(*str == '-' || *str == '+')
	str += 1;
      PoInt = E = True;
    }
    else if (!isdigit (*str)) 
      return False;
    n += 1;
    str += 1;
  }
  if (!*str && n) return True;
  return False;
}

/*
 * regarde si la chaine est la representation
 * externe d'un flottant d'une chaine
 */
Boolean isString(str)
     CHAR *str;
{
  return (*str == '"' && str[strlen(str)] == '"') ? True : False ;
}

