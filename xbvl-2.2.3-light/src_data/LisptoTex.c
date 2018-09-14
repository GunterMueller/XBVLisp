/* Conversion de source Lisp en Latex */

#include <stdio.h>

#include "lisp_names.h"

FILE *in_fp, *out_fp;

static int in_lisp = 0, is_lisp_index = 0;

#define is_sep(c)       ((c) == '(' || (c) == ')' || (c) == ';' || (c) == ' ' || (c) == '\t' || (c) == '\n')
#define is_alpha(c)     (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= '0' && (c) <= '9'))

int
is_lisp_name(str)
char *str;
{
  register int a = 0, b = NUM_LISP_NAMES - 1, 
    i = NUM_LISP_NAMES / 2, val, k = 0, l = strlen(str), ln;
  char m_str[80];
  memset(m_str, '\0', 80);
  while(*str && is_alpha(*str)) m_str[k++] = *(str++);
  if(!k) return -1;
  while(1) {
    if(!(val = strcmp(m_str,lisp_names[i]))) return i;
    else if(val < 0) {
      b = i;
      i -= (i - a) / 2;
      if(b == i) return -1;
    }
    else {
      a = i;
      i += (b - i) / 2;
      if(a == i) return -1 ;
    }
  }
}  

void
check_char(str)
char *str;
{
  static char ret[BUFSIZ];
  char *tmp, *tmp1, c, prev_c;
  int b_prev, i, j, k, is_lisp = 0, lisp_ind, in_comm = 0;

  memset(ret, (int) '\0', BUFSIZ);
  /* calcul de l'espace a ajouter */
  if(in_lisp) {
    sprintf(ret, "\\> ", in_lisp);
    for(i = 1; i < in_lisp; i++) sprintf(ret, "%s \\> ", ret);
  }
  /* calcul du parenthèsage i final = nombre de parenthèses ouvertes et non fermées de la ligne */
  for(j = 1, i = 0, tmp = str; *tmp; tmp++) {
    if(*tmp == ';') j = (j == 0); /* in / out comment */
    if(j && *tmp == '(') ++i;
    if(j && *tmp == ')') --i;
  }
  if(i > 0) { 
    if(!in_lisp) fputs("\\begin{tabbing}\n", out_fp); 
    else i += in_lisp;
  }
  else if(in_lisp + i >= 0) in_lisp += i;
  else in_lisp = 0;

  /* Parse des caracteres speciaux (pour latex...) */
  prev_c = ' '; is_lisp = 0;
  for( ; (c = *str) ; str++) {
    {
      if(is_lisp_index && !in_comm){
	if(is_sep(prev_c) && !is_sep(c) && (lisp_ind = is_lisp_name(str)) >= 0) {
	  sprintf(ret, "%s{\\bf \0", ret);
	  b_prev = strlen(ret); is_lisp = 1;
	}
	if(is_sep(c) && is_lisp) {
	  sprintf(ret, "%s}\\index{%s}", ret, lisp_names[lisp_ind]);
	  is_lisp = 0;
	}
      }
      switch(c) 
	{
	case ';': 
	  if(in_comm) strcat(ret, ";} \0");
	  else strcat(ret, "{\\it ;");
	  in_comm = (in_comm == 0);
	  break;
	case '\\': strcat(ret, "\\backslash \0"); break;
	case '$': strcat(ret,  "\\$\0"); break;
	case '%': strcat(ret,  "\\%\0"); break;
	case '&': strcat(ret,  "\\&\0"); break;
	case '^': strcat(ret,  "\\^{ }\0"); break;
	case '_': strcat(ret,  "\\_\0"); break;
	case '{': strcat(ret,  "\\{\0"); break;
	case '}': strcat(ret,  "\\}\0"); break;
	case '#': strcat(ret,  "\\#\0"); break;
	case '[': strcat(ret,  "$[$\0"); break;
	case ']': strcat(ret,  "$]$\0"); break;
	case '<': strcat(ret,  "$<$\0"); break;
	case '>': strcat(ret,  "$>$\0"); break;
	case 'É': strcat(ret, "\\'{E}"); break;
	case 'È': strcat(ret, "\\`{E}"); break;
	case 'À': strcat(ret, "\\`{A}"); break;
	case 'á': strcat(ret, "\\'{a}"); break;
	case 'â': strcat(ret, "\\^{a}"); break;
	case 'é': strcat(ret, "\\'{e}"); break;
	case 'è': strcat(ret, "\\`{e}"); break;
	case 'ê': strcat(ret, "\\^{e}"); break;
	case 'ç': strcat(ret, "\\c{c}"); break;
	case 'î': strcat(ret, "\\^{i}"); break;
	case 'ú': strcat(ret, "\\'{u}"); break;
	case 'ô': strcat(ret, "\\^{o}"); break;
	default: 
	  if(ret[0]) sprintf(ret, "%s%c", ret, c);
	  else ret[0] = c, ret[1]= '\0';
	}
      prev_c = c;
    }
  }
  if(is_lisp)  sprintf(ret, "%s}\\index{%s}", ret, lisp_names[lisp_ind]);
  if(in_comm)  { strcat(ret, "}"); in_comm = 0; }

  /* ajout ou retrait de tabulation */
  if(i > in_lisp) {
    tmp = &ret[0];
    while(*tmp == ' ' || *tmp == '\t' || *tmp == '\\' || *tmp == '>') 
      fputc(*(tmp++), out_fp);
    for(j = 1; i > in_lisp && *tmp; tmp++) {
      fputc(*tmp, out_fp);
      switch(*tmp) {
      case ';' : j = (j == 0); break;
      case '(' : ++k; break;
      case ')' : --k; break;
      case ' ': case '\t':
	if(j & k) {
	  k = 0;
	  fputs("\\= \0", out_fp);
	  if(++in_lisp == i) fputs(tmp, out_fp);
	  else
	    while(*(tmp+1) && (*(tmp+1) == ' ' || *(tmp+1) == '\t')) ++tmp;
	}
	break;
      }
    }
    while(i > in_lisp) {
      fputs(" \\= \0", out_fp);
      ++in_lisp;
    }
    fputs("\\\\\n\0", out_fp);
  }
  else {
    fputs(ret, out_fp);
    fputs("\\\\\n\0", out_fp);
    if(i < 0 && !in_lisp) 
      fputs("\\end{tabbing}\n\0", out_fp);
  }
}

make_lisp()
{
  char buf[BUFSIZ], *tmp;

  while(fgets(buf,120,in_fp) != NULL) {
    buf[strlen(buf)-1] = '\0';
    if(!buf[0]) continue;
    tmp = &buf[0];
    check_char(tmp);
  }
}
    
main(ac, av)
int ac;
char **av;
{
  char i_fname[80], *prgName = av[0], *f_name = NULL,
       ind_name[80];

  in_fp = stdin;
  out_fp = stdout;

  ++av;
  while(*av) {
    switch(**av) {
    case 'L' : is_lisp_index = 1; break;
    }
    ++av;
  }
  make_lisp();
}

