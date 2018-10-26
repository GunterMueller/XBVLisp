/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *	misc.c		miscelaneus
 *                      Ernesto Sendoya
 */

#include <vlisp.h>


extern char *getenv();

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
  }
  return fild;
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
    new = NewString(buffer);
    *abuf = new;
  }
  return n;
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

