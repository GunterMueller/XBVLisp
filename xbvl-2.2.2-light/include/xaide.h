#ifndef XAIDEH
#define XAIDEH
/* prototypes */
#ifdef STDC_HEADERS
/* xaide.c */
extern void createListAide(String * list_o );
extern String * initListAide(Int fd, String name);
extern Int strinc(String ref);
extern Int findDefinitionObjet( CHAR **buf_obj, CHAR *objet);
extern void affObjet( Widget w, caddr_t app_data, XtListReturnStruct *objet);
extern void findWindAide(void);
extern Boolean getWidgetFromAtomes(Widget *w, CHAR *name, WidgetClass w_c);

#else
extern void createListAide();
extern String * initListAide();
extern Int strinc();
extern Int  cmpsujs();
extern Int findDefinitionObjet();
extern void affObjet();
extern void findWindAide();
extern Boolean getWidgetFromAtomes();
#endif
#endif
