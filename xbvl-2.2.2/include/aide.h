
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* *    aide.h    Ernesto Sendoya */

#ifndef _aide_h_
#define _aide_h_

#ifdef HAVE_INDEX
#include <strings.h>
#else
#include <string.h>
#define index strchr
#endif

/* noms des widgets de la hierarchie */
/*#define DOC_APP_CLASS   "DocbVLISP"*/
#define DOC_APP_CLASS  TOP_APP_CLASS
#define DOC_SHELL_NAME  "doc-bVLISP"
#define FORM1_DOC_NAME  "doc-form"
#define DOC_VIEW_NAME   "doc-viewPort"
#define LIST_DOC_NAME   "doc-list"
#define DOC_BIT_NAME    "doc-BITMAP"
#define DOC_BIT_VIEW    "doc-bit-view"
#define DOC_BIT_DRAW    "doc-bit-draw"

#define AIDE_ROOT        "Aide-root"
#define AIDE_INTERFACE   "aide"

extern Widget new_aide_o, new_list_o;

extern CHAR *Aide_src, *Aide_index, *Aide_section;

#define OPENINDEX() openFile("BVLISP_AIDEDFILE", Aide_index)
#define OPENSECTION() openFile("BVLISP_AIDESFILE", Aide_section)
#define FOPENSUJETS() fopenFile("BVLISP_AIDEINFILE", Aide_src)

/*
#define OPENINDEX() open(DFILE,0)
#define FOPENSUJETS() fopen(INFILE, "r")
*/

typedef struct {
  long pos;
  CHAR ligne[120];
} LigneAide;
#endif
