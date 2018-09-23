/*
 *	traaide
 *	traducteur de file-aide
 *	Patrick Greussay. 24 Janvier 1981
 *			  revise le 14 Septembre 1982
 *			  revise le 24 Septembre 1982
 *      Revision le 26 decembre 1995 : D.P. : ajout de la generation en Latex 
 *      et des commandes speciales
 */

/* les commandes speciales, presentes en debut de ligne, sont :
 * Pour les sections :
 * ^%H <titre>    : titre de la section (aussi le titre de la fenetre d'aide sous lisp).
 *                  Pour etre pris en compte par xbvl, cette commande doit etre en premiere
 *                  ligne du fichier d'aide.
 * ^<titre>       : titre de la section. Peut, contrairement a ^%H se trouve a n'importe
 *                  quel endroit du fichier d'aide et n'est utilise que par la generation
 *                  en Latex.
 * ^^<titre>      : titre d'une sous section
 * ^              : definition d'un nouvel item d'aide (le nom de l'item se trouvant sur la
 *                  ligne suivante et ne sera pas affichee lors de l'affichage de l'aide 
 *                  sous xbvl.
 * Pour l'habillage :
 * ^%T<n-col>     : definission d'un tableau de n-col colonnes. les colonnes sont seprarees 
 *                  par des tabulations (un caractere autre que tabulation definissant le
 *                  contenu d'une colonne, meme un espace). La fin d'un tableau est indique
 *                  par une autre commande speciale ou par une ligne vide.
 * ^%L            : affichage de code Lisp. Il impliquera une indentation speciale en Latex.
 *                  l'arret de ce mode est indique par une autre commande speciale ou par une
 *                  ligne vide.
 * ^%B <width> <height> <fichier>   : indication d'affichage d'un fichier bitmap par Xbvl. 
 * ^%P <fichier>   : indication d'inclusion d'un fichier poscript par Latex.
 * ^%I <fichier>   : indication d'inclusion d'une image par Html
 * ^%%             : mise en commentaire de la ligne
 */ 
   
#include <stdio.h>
#ifdef HAVE_INDEX
#include <strings.h>
#else
#include <string.h>
#define index strchr
#endif

#include "lisp_names.h"

typedef struct {
  long pos;
  char ligne[120];
} LigneAide;

#define DAT_MODE 0
#define TEX_MODE 1
#define HTML_MODE 2

static char mode = 0;
static int lev[3], is_subsub = 0, is_file = 0, is_lisp_index = 0;


static char o_fname[80], s_fname[80], toc_base_name[80], idx_name[80];
FILE *in_fp, *out_fp, *sec_fp, *toc_fp = NULL, *cur_toc_fp = NULL;

main(ac, av)
int ac;
char **av;
{
  char i_fname[80], *prgName = av[0], *f_name = NULL,
       ind_name[80];

  in_fp = stdin;
  out_fp = stdout;
  sec_fp = stderr;

  ++av;
  while(*av) {
    switch(**av) {
    case 'L' : is_lisp_index = 1; break;
    case 'l' : mode = TEX_MODE; break;
    case 'h' : mode = HTML_MODE;break;
    case 'f' : 
      sprintf(i_fname, "%s.dat", f_name = *(++av));
      if(mode & TEX_MODE) {
	sprintf(o_fname, "%s.tex", *av);
	sprintf(idx_name, "%s.ind", *av);
      }
      else if(mode & HTML_MODE){
	sprintf(o_fname, "%s.html", *av);
	sprintf(toc_base_name, "toc_%s", *av);
      }
      else { 
	sprintf(o_fname, "%s.:dat", *av);
	sprintf(s_fname, "%s.:sec", *av);
      }
      if((in_fp = fopen(i_fname, "r")) &&
	 (out_fp = fopen(o_fname, "w"))) is_file= 1;
      else exit(1);
      printf("%s --> %s\n", i_fname, o_fname);
      break;
    default :
      printf("Usage : %s [lhL] [f file_name] \n\tl = mode latex\n\tL = index Lisp\n\th = mode Html\n\tfile_name = fichier sans extension\n",prgName);
      exit(1);
    }
    ++av;
  }
  if(mode) {
    if(mode & HTML_MODE) {
      if(f_name) { 
	sprintf(ind_name, "%s.html", toc_base_name);
	toc_fp = fopen(ind_name, "w");
      }
      else {
	is_file = 2;
	strcpy(toc_base_name, "toc\0");
	toc_fp = fopen("toc.html", "w");
	out_fp = fopen(strcpy(o_fname, "doc.html"), "w");
      }
    }
    make_mode();
  }
  else make_aide();
  if(in_fp != stdin) fclose(in_fp);
  if(out_fp != stdout) fclose(out_fp);
  if(toc_fp) fclose(toc_fp);
  if(cur_toc_fp) fclose(cur_toc_fp);
  exit(0);
}

make_aide()
{
  LigneAide s[50] ;  /* sections */
  LigneAide t[2000]; /* éléments */
  int nlus,it,i, n_s;
  char buf[120],obuf[120];
  long nb;
  nb=0L; it = -1, n_s = 0;
  while(fgets(buf,120,in_fp) != NULL)
    {
      nlus=strlen(buf);
      
      if(buf[0] == '^' && buf[1] == '^') /* section */
	{
	  s[n_s].pos = nb; strcpy(s[n_s].ligne, &buf[2]);
	  s[n_s].ligne[strlen(s[n_s].ligne)-1] = '\0';
	  if(s[n_s].ligne[strlen(s[n_s].ligne)-2] < 32) 
	    s[n_s].ligne[strlen(s[n_s].ligne)-2] = '\0';
	  ++n_s;
	}
      if(obuf[0]=='^' && obuf[1] <= 27)
	{
	  t[++it].pos=nb; strcpy(t[it].ligne,buf);
	}
      nb+=(long)nlus;
      strcpy(obuf,buf);
    }
  it++;
  fwrite(&it,sizeof(int), 1, out_fp);
  --it;
  for(i=0; i<=it ; i++)fwrite(&t[i],sizeof(LigneAide), 1, out_fp);
  if(n_s) { /* des sections existes */
    if(s_fname[0]) {
      if(!(sec_fp = fopen(s_fname, "w"))) exit(1);
    }
    else sec_fp = stderr;
    fwrite(&n_s, sizeof(int), 1, sec_fp); /* nombre de sections */
    for(i = 0; i < n_s; i++) fwrite(&s[i],sizeof(LigneAide), 1, sec_fp);
    fclose(sec_fp);
  }
}

static int in_tableau = 0, in_enum = 0, in_item = 0, 
           in_lisp = 0, max_lisp = 0, max_html = 0;

void
fin_tableau() 
{ 
  in_tableau = 0;
  if(mode & TEX_MODE) fputs("\\end{tabular}\n", out_fp);    
  else fputs("</TABLE>\n", out_fp); 
}
void
fin_enum()    
{ 
  if(mode & TEX_MODE) fputs("\\end{enumerate}\n", out_fp);
  else fputs("</OL>\n", out_fp);
  in_enum = 0; 
}
void
fin_item()
{
  if(mode & TEX_MODE) fputs("\\end{itemize}\n", out_fp); 
  else fputs("</UL>\n", out_fp);
  in_item = 0; 
}
void
fin_lisp()    
{ 
  if(mode & TEX_MODE) fputs("\\end{tabbing}\n", out_fp);
  in_lisp = max_lisp = 0; 
}

static char *Item[2] = {"\\item \0", "<LI>\0"};

#define NOT_IN_ANY()    (!in_enum && !in_item && !in_tableau && !in_lisp)
#define is_sep(c)       ((c) == '(' || (c) == ')' || (c) == ';' || (c) == ' ' || (c) == '\t' || (c) == '\n')
#define is_punct(c)     ((c) == ',' || (c) == '.' || (c) == ';' || (c) == '(' || (c) == ')')
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

char *
check_char(str, is_section)
char *str;
int is_section;
{
  static char ret[BUFSIZ];
  char *tmp, *tmp1, c, prev_c;
  int b_prev, n_col = in_tableau-1, have_col = 1, i, j, is_lisp = 0, lisp_ind;

  memset(ret, (int) '\0', BUFSIZ);
  /* Dans une enumeration ... next item = "\t<n_item>)" */
  for(tmp = str ; in_enum && (c = *tmp); tmp++) {
    if(c == '\t' || c == ' ' || isdigit(c)) continue; 
    if(c == ')') { fprintf(out_fp, Item[(mode == HTML_MODE)]); str = ++tmp; }
    break;
  }

  /* Dans une liste : next item = "\t\t<item>", 
     continuation de l'item precedent = "\t\t\t<cont>" */
  if(in_item) {
    if(*(tmp = str) == '\t' && *(++tmp) == '\t') {
      if(*(tmp+1) != '\t') { fprintf(out_fp, Item[(mode == HTML_MODE)]); str = tmp; }
      else str = tmp+1;
    }
    else fin_item();
  }

  /* Dans un tableau HTML : debut de ligne = <TR> */
  if(in_tableau && mode & HTML_MODE) fputs("<TR><TH>\0", out_fp);

  /* En mode lisp : calcul de l'espace a ajouter */
  if(in_lisp) {
    if(in_lisp > 1) {
      if(mode & TEX_MODE) {
	sprintf(ret, "\\> ");
	for(i = 2; i < in_lisp; i++) sprintf(ret, "%s \\> ", ret);
      }
      else sprintf(ret, "<tab to=t%d>\0", in_lisp-1);
    }
    for(j = 1, i = 0, tmp = str; *tmp; tmp++) {
      if(*tmp == ';') j = (j == 0); /* in / out comment */
      if(j && *tmp == '(') ++i;
      if(j && *tmp == ')') --i;
    }
    if(i > 0) ++in_lisp; 
    if(i < 0 && in_lisp > 1) --in_lisp;
    tmp = &ret[strlen(ret)];
  }

  /* Parse des caracteres speciaux (pour latex...) */
  prev_c = ' '; is_lisp = 0;
  for( ; (c = *str) ; str++) {
    if(mode & TEX_MODE) {
      if(is_lisp_index && !is_section && !in_lisp){
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
    else
      switch(c) {
      case '<' : strcat(ret, "&lt;"); break;
      case '>' : strcat(ret, "&gt;"); break;
      case '"' : strcat(ret, "&quot;"); break;
      default: if(ret[0]) sprintf(ret, "%s%c", ret, c);
      else ret[0] = c, ret[1]= '\0';
      }
    /* parse du changement de colonne dans un tableau... */
    if(n_col > 0) {
      if(!have_col && c == '\t') n_col--, strcat(ret, (mode & TEX_MODE) ? "& \0" : "<TH>\0"), have_col = 1; 
      if( have_col && c != '\t') have_col = 0;
    }
  }
  if(is_lisp) {
    sprintf(ret, "%s}\\index{%s}", ret, lisp_names[lisp_ind]);
  }
    
  /* ajout ou retrait de tabulation */
  if(i > 0 && in_lisp > max_lisp) { 
    char stmp[80];
    max_lisp = in_lisp;
    while(*tmp == ' ' || *tmp == '\t') ++tmp;
    if(mode & TEX_MODE || max_lisp > max_html) {
      for(j = 0, tmp1 = tmp; *tmp1; tmp1++)
	if(*tmp1 == ' ') { 
	  strcpy(stmp, (tmp1+1));
	  *tmp1 = '\0';
	  if(mode & TEX_MODE) sprintf(ret, "%s \\= %s\\\\", ret, stmp);
	  else sprintf(ret, "%s<tab id=t%d> %s", ret, ((max_html = max_lisp) - 1), stmp);
	  break;
	}
      if(tmp1 && !*tmp1) {
	if(mode & TEX_MODE) strcat(ret, " \\= \\\\\0");
	else if(max_lisp > max_html) sprintf(ret, "%s<tab id=t%d> \0", ret, ((max_html = max_lisp) - 1));
      }
    }
  }
		
  /* Ajout des colonnes manquantes */
  while(n_col-- > 0) strcat(ret, (mode & TEX_MODE) ? "& \0" : "<TH>\0");
  /* et de la ligne horizontale */
  if(mode & TEX_MODE && in_tableau > 0) strcat(ret, "\\\\ \\hline");
  return ret;
}

void 
PutHeader()
{
  if(mode & TEX_MODE) {
    fputs("\\documentstyle[A4,french,psfig,here]{article}\n", out_fp);
    if(is_file) fputs("\\makeindex", out_fp);
    fputs("\\begin{document}\n\\title{TITRE A REMPLACER}\n", out_fp);
    fputs("\\author{LE OU LES AUTEURS}\n\\maketitle", out_fp);
    fputs("\\tableofcontents\n", out_fp);
  }
  else {
    lev[0] = lev[1] = lev[2] = 0;
    fputs("<HTML><HEAD><TITLE>TITRE A REMPLACER</TITLE>\n",out_fp);
    fputs("<BODY>\n", out_fp);
    fputs("<HTML><HEAD><TITLE>TITRE A REMPLACER : Sommaire</TITLE>\n",toc_fp);
    fputs("<BODY>\n", toc_fp);
  }
}    

void
PutEnd()
{
  if(mode & TEX_MODE) {
    if(is_file) fprintf(out_fp, "\\pagebreak\n\\input{%s}\n\n", idx_name);
    fputs("\\end{document}\n", out_fp);
  }
  else {
    fputs("</BODY></HTML>\n", out_fp);
    fputs("</BODY></HTML>\n", toc_fp);
  }
}

void
MakeSection(level, str)
int level;
char *str;
{
  static char *sec_name[3] = { "section\0", "subsection\0", "subsubsection\0" };
  static char *sec_mode[3] = { "H1\0", "H3\0", "B\0" };
  static char lev0_str[80];
  static int have_lev0 = 0;

  if(mode & TEX_MODE) {
    if(!level) fprintf(out_fp, "\\pagebreak \n");
    fprintf(out_fp, "\\%s{%s}", sec_name[level], str);
    if(level == 2) fprintf(out_fp, "\\index{%s}", str);
    fputs("\n", out_fp);
  }
  else {
    char cur_toc_name[80];
    ++lev[level];
    if(level == 1) {
      lev[2] = 0;
      if(!have_lev0) {
	if(lev0_str[0]) {
	  fprintf(toc_fp, "<%s> %d <A HREF=\"%s_%d_%d.html\"> %s </A> <%s>\n", sec_mode[0], lev[0], toc_base_name, 
		  lev[0], lev[1], lev0_str, sec_mode[0]);
	  lev0_str[0] = '\0';
	}
	have_lev0 = !is_subsub;
	if(cur_toc_fp) {
	  fprintf(cur_toc_fp, "\n</BODY></HTML>\n");
	  fclose(cur_toc_fp);
	}
	sprintf(cur_toc_name, "%s_%d_%d.html", toc_base_name, lev[0], lev[1]);
	cur_toc_fp = fopen(cur_toc_name, "w");
	fprintf(cur_toc_fp, "<HTML><HEAD><TITLE>Aide, Section %d ", lev[0]);
	if(is_subsub) fprintf(cur_toc_fp, "%d ", lev[1]);
	fprintf(cur_toc_fp, "</TITLE>\n<BODY>\n");
	fprintf(cur_toc_fp, "<%s> %d %s </%s>\n", sec_mode[0], lev[0], lev0_str, sec_mode[0]);
      }
      if(is_subsub)
	fprintf(toc_fp, "<%s> %d.%d <A HREF=\"%s_%d_%d.html\"> %s </a> </%s>", sec_mode[level], lev[0], lev[1], 
		toc_base_name, lev[0], lev[1], str, sec_mode[level]);
    }
    if(!level) {
      lev[1] = lev[2] = have_lev0 = 0;
      strcpy(lev0_str, str);
    }
    else {
      fprintf(cur_toc_fp, "<%s> %d.%d", sec_mode[level], lev[0], lev[1]);
      if(level > 1) fprintf(cur_toc_fp, ".%d", lev[2]);
      fprintf(cur_toc_fp, " <A HREF=\"%s#SEC%d%d%d\"> %s </A> </%s><P>\n", o_fname, lev[0], lev[1], lev[2], 
	      str, sec_mode[level]);
    }
    fputs("<P>\n", out_fp);
    fprintf(out_fp, "<%s><A NAME=\"SEC%d%d%d\"> %d", sec_mode[level], lev[0], lev[1], lev[2], lev[0]);
    if(level > 0) fprintf(out_fp, ".%d", lev[1]);
    if(level > 1) fprintf(out_fp, ".%d", lev[2]);
    fprintf(out_fp, " %s</A> </%s><P>\n", str, sec_mode[level]);
    fflush(out_fp);

    fflush(toc_fp);
    if(cur_toc_fp) fflush(cur_toc_fp);
  }
}

void
MakeOpenTableau()
{
  int i;
  if(mode & TEX_MODE) {
    fprintf(out_fp, "\\begin{tabular}{|");
    for(i = 0; i < in_tableau; i++) fprintf(out_fp, "l|");
    fprintf(out_fp, "}\\hline\n");
  }
  else {
    fputs("<TABLE BORDER>\n", out_fp);
  }
}

#define PutErr(str) fprintf(stderr, str), fflush(stderr)

make_mode()
{
  char buf[BUFSIZ],obuf[BUFSIZ], *tmp, *tmp_1;
  int nlus = 0,i;

  PutHeader();

  while(fgets(buf,120,in_fp) != NULL) {
    buf[strlen(buf)-1] = '\0';
    if(!buf[0] && nlus) continue;
    else if(obuf[0] == '^') {
      if(in_tableau) fin_tableau();
      if(in_enum) fin_enum();
      if(in_item) fin_item();
      switch(obuf[1]) {
      case '^': /* subsection */
	tmp = &obuf[2];
	is_subsub = 1;
	MakeSection(1, strcpy(obuf, check_char(tmp, 1)));
	break;
      case '%': /* commandes speciales : ^%T<n_col> = debut de tableau, ^%L = debut format lisp ^%H <Titre du fichier>*/
	switch(obuf[2]) {
	case 'T': /* Debut d'un tableau = ^%T<n_col> */
	  in_tableau = obuf[3]-'0'; /* nombre de colonnes */
	  MakeOpenTableau();
	  tmp = &buf[0];
	  strcpy(buf, check_char(tmp, 0));
	  strcat(buf, "\n");
	  fputs(buf, out_fp);
	  break;
	case 'L' : /* debut du mode lisp : in_lisp contient l'espace a ajouter */
	  in_lisp = 1; 
	  if(mode & TEX_MODE) fprintf(out_fp, "\\begin{tabbing}\n");	
	  else fputs("<P>", out_fp);
	  tmp = &buf[0];
	  strcpy(buf, check_char(tmp, 0));
	  strcat(buf, "\n");
	  fputs(buf, out_fp);
	  break;
	case 'H' :
	  tmp = &obuf[4];
	  MakeSection(0, strcpy(obuf, check_char(tmp, 1)));
	  break;
	case 'P' :                /* PS FILES : pour le mode latex */
	  if(mode & TEX_MODE) {
	    fprintf(out_fp, "\n\\begin{figure}[H]\n\\centerline{\\psfig{figure=%s}}\n\\end{figure}\n\n", &obuf[4]);
	    tmp = &buf[0];
	    strcpy(buf, check_char(tmp, 0));
	    strcat(buf, "\n");
	    fputs(buf, out_fp);
	  }
	  break;
	case 'I' : 
	  if(mode & HTML_MODE) fprintf(out_fp, "\n<IMG SRC=\"%s\" ALIGN=CENTER>\n", &obuf[4]);
	  break;
	}
	break;
      case '\0': 
	/*	tmp = &buf[0]; */
	MakeSection(1+is_subsub, strcpy(buf, check_char(&buf[0], 1)));
	nlus = 1;
	break;
      default: /* section */
	/*	tmp = &obuf[1]; */
	if(!(lev[0] && !lev[1] && !lev[2])) MakeSection(0, strcpy(obuf, check_char(&obuf[1], 1)));
	is_subsub = 0;
	break;
      }
    }
    /* Debut d'enumeration = "\t1)" */
    else if(NOT_IN_ANY() && buf[0] == '\t' && buf[1] == '1' && buf[2] == ')') { 
      tmp = &buf[3];
      if(mode & TEX_MODE) fprintf(out_fp, "\\begin{enumerate}\n");
      else fputs("<OL>\n", out_fp);
      fprintf(out_fp, "%s%s", Item[(mode == HTML_MODE)], strcat(strcpy(buf, check_char(tmp, 0)), "\n"));
      in_enum = 1;
    }
    /* Debut d'une liste d'elements = "\t\t" */
    else if(NOT_IN_ANY() && buf[0] == '\t' && buf[1] == '\t') { /* debut d'un itemize */
      if(mode & TEX_MODE) fprintf(out_fp, "\\begin{itemize}\n");
      else fputs("<UL>\n", out_fp);
      tmp = &buf[2];
      fprintf(out_fp, "%s%s", Item[(mode == HTML_MODE)], strcat(strcpy(buf, check_char(tmp, 0)), "\n"));
      in_item = 1;
    }
    else if(buf[0] && buf[0] != '^') {
      tmp = &buf[0];
      i = strlen(buf)-1;
      strcpy(buf, check_char(tmp, 0));
      if(!in_enum && !in_item &&
	 (in_lisp || 
	  (nlus != 0 && buf[0] == '(') || 
	  buf[i] == '.' || buf[i] == ':' || buf[i] == ')')) {
	if(mode & TEX_MODE) strcat(buf, " \\\\\n");
	else strcat(buf, "<P>\n");
      }
      else strcat(buf,"\n");
      fputs(buf, out_fp);
      nlus = 0;
    }
    else { 
      if(in_tableau) fin_tableau();
      if(in_enum) fin_enum();
      if(in_item) fin_item();
      if(in_lisp) fin_lisp();
      fputs("\n", out_fp);
    }
    strcpy(obuf,buf);
    fflush(out_fp);
  }

  if(in_tableau) fin_tableau();
  if(in_enum) fin_enum();
  if(in_item) fin_item();
  if(in_lisp) fin_lisp();
  PutEnd();
}
    
