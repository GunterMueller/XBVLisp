 /* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*
 *               xaide.c
 *	aidvlisp	Michel Saint-Tourens
 *			Ernesto Sendoya : interface X dec. 1990 
 */
#include <vlisp.h>
#include <Xbvlisp.h>
#include <aide.h>
#include <xaide.h>
#include <vlisp_files.h>
#include <X11/IntrinsicP.h>
#include <DrawxbvlP.h>

/* pour la sortie des informations */
static Widget aide_interf = NULL, top_aide = NULL, top_bitmap_aide = NULL;
Widget new_aide_o, new_list_o;

LigneAide tt;


/*---------------------------------------------------------------------*
 *	Creation de l'interface pour afficher les objets disponibles
 *                               de aide
 *      On conserve la coherence de l'interface en s'assurant que la
 *      destruction d'une de ses widgets est prise en compte par le 
 *      prochain appel de la fonction.
 *---------------------------------------------------------------------*/
void createListAide(list_o)
     String *list_o;
{
  extern Int HEIGHT, WIDTH;
  static Widget form_list = NULL;
  static Widget list_widgette = NULL, wiewport = NULL ;
  Int i;
  Dimension width_a = 0, height_a = 0;
  CHAR *top_name = "Aide (Liste)\0", buf[120], *tmp;
  Arg wargs[12];

  if(!top_aide && (list_widgette || wiewport || form_list)) list_widgette = form_list = wiewport = NULL;

  if (!list_widgette && !new_list_o) /* liste d'aide deja cree  ? */
    getWidgetFromAtomes(&list_widgette, LIST_DOC_NAME, listWidgetClass);

  if(list_widgette || new_list_o){
    /* changer la liste d'aide , si differente */
    Widget list_courr = new_list_o ? new_list_o : list_widgette ;
    changeWidgetList(list_courr, list_o);
  }
  
  /* -----------------------------------------------*
   *	Nouvelle racine pour accrocher l'ensemble    *
   * -----------------------------------------------*/
  
  if(!top_aide){
    i = 0;
    XtSetArg(wargs[i], XtNallowShellResize , True);i++;
    top_aide = XbvlCreateApplicationShell(DOC_SHELL_NAME, DOC_APP_CLASS,
					  wargs, i, NULL);
    XtAddCallback(top_aide, XtNdestroyCallback, (XtCallbackProc) cleanVar, &top_aide );
  }
  if((i = (Int)FOPENSUJETS()) != (Int)NULL) {
    fgets(buf, 120, (FILE *)i);
    if(buf[0] == '^' && buf[1] == '%' && buf[2] == 'H')  /* le titre est la... */
      top_name = strcat(&buf[4], " (Liste)\0");
    fclose((FILE *)i);
  }
  XtSetArg(wargs[0], XtNtitle, top_name);
  XtSetValues(top_aide, wargs, 1);
      
  /* pour contenir les differents widgets */
  /* ------------------------------------ */
  if(!form_list){
    i = 0;
    form_list = XbvlCreateManagedWidget(FORM1_DOC_NAME,
					formWidgetClass  ,
					top_aide,
					wargs, i, (struct atome **)NULL);
    XtAddCallback(form_list, XtNdestroyCallback, (XtCallbackProc) cleanVar, &form_list);
  }
  /* pour pouvoir acceder a tous les titres des sujets*/
  /* ------------------------------------------------ */
  if(!wiewport){
    i = 0;
    XtSetArg(wargs[i], XtNallowVert , True);i++;
    XtSetArg(wargs[i], XtNallowHoriz , True);i++;
    XtSetArg(wargs[i], XtNresizable , True);i++;
  
    wiewport = XbvlCreateManagedWidget(DOC_VIEW_NAME,
				       viewportWidgetClass  ,
				       form_list,
				       wargs, i, (struct atome **) NULL);
    XtAddCallback(wiewport, XtNdestroyCallback, (XtCallbackProc) cleanVar, &wiewport);
  }
  /* et la widgette pour lister l'aide */
  /* --------------------------------- */
  if(!list_widgette){
    i=0;
    XtSetArg(wargs[i], XtNlist,list_o);i++;
    XtSetArg(wargs[i], XtNresizable,TRUE);i++;
    list_widgette = XbvlCreateManagedWidget(LIST_DOC_NAME, 
					    listWidgetClass  ,
					    wiewport,
					    wargs, i, (struct atome **)NULL);
    XtAddCallback(list_widgette, XtNdestroyCallback, (XtCallbackProc) cleanVar, & list_widgette );
    
    i = 0;
    XtSetArg(wargs[i], XtNheight, &height_a);i++;
    XtSetArg(wargs[i], XtNwidth, &width_a);i++;
    XtGetValues(list_widgette, wargs, i);
    
    /* pour calculer correctement les dimentions du viewport */
    
    height_a = (Dimension) height_a > HEIGHT/3 ?  HEIGHT/3 : height_a;
    width_a = (Dimension)width_a > WIDTH/3 ?  WIDTH/3 : width_a;
    i = 0;
    XtSetArg(wargs[i], XtNheight, height_a);i++;
    XtSetArg(wargs[i], XtNwidth, width_a);i++;
    XtSetValues(wiewport, wargs, i);
  }
  else XtRemoveAllCallbacks(list_widgette, XtNcallback);
    
  XtAddCallback(list_widgette, XtNcallback, (XtCallbackProc) affObjet, (caddr_t)a3);
  XtRealizeWidget(top_aide); /* et on affiche l'interface */
}

/*---------------------------------------------------------------------*
 *	initialisation de la liste des objets
 *  retourne le nombre d'objets initialises.
 *---------------------------------------------------------------------*/
String * initListAide(fd, type)
     Int fd;
     String type;
{
  String *list_objets = NULL;
  Int n = 0 , nitems;
  
  read(fd,(CHAR *)&n,sizeof(int));

  nitems = 0;  
  while( read(fd,(CHAR *)&tt, sizeof(tt)) > 0 ){
    *(index(tt.ligne, '\n')) = '\0';
    if(strinc(type) == 1){/* un sujet a prendre */
      if(list_objets == NULL) 
	list_objets = (String *) XtCalloc(2, sizeof(String));
      else 
	list_objets = (String *) XtRealloc( (String)list_objets, (nitems+2) * sizeof(String));
      list_objets[nitems] =  XtNewString(tt.ligne);
      nitems += 1;
    }
  }
  if(nitems > 0){
    list_objets[nitems] = NULL;
    qsort(list_objets, nitems, sizeof(String *), cmpsujs );
  }
  return list_objets;
}
/*---------------------------------------------------------------------*
 *	cherche si la chaine passee en argument fait partie des
 *      sujets d'aide disponibles.
 *---------------------------------------------------------------------*/
Int strinc(partie)
     String partie;
{/* est-ce une partie */
  CHAR *ii;
  
  if (! partie ) return 1;
  if ((ii=(index(tt.ligne, *partie )))!= NULL){
    Int l = strlen(partie), i=0;
    while(*ii == *(partie + i))
      {ii++; i++;}
    if(i >= l) return 1;
  }
  return 0 ;
}
/* X-SYSTEM */
#define GET_GC(w)     XtIsSubclass(w, drawxbvlWidgetClass) ? ((DrawxbvlWidget)w)->drawxbvl.gc \
                           : ( XtIsSubclass(w, xbvlispWidgetClass) ? \
			       ((XbvlispWidget)w)->text.gc :  ((XbvlispWidget)Xbvlisp_courr)->text.gc)
/* Affichage d'une image bitmap */
static void affBitmap(w, h, bmp, w_name)
int w, h;
CHAR *bmp, *w_name;
{
  int i, x, y;
  Arg wargs[5];
  CHAR top_bitmap_name[80];
  static Widget bitmap_viewport = NULL, bitmap_drawing = NULL;
  Display *dpy;
  GC gc;
  Pixmap pxmp;
  
  if(!top_bitmap_aide && (bitmap_viewport || bitmap_drawing))
    bitmap_viewport = bitmap_drawing = NULL;

  sprintf(top_bitmap_name, "Image liée à %s\0", w_name);

  if(!top_bitmap_aide){
    i = 0;
    XtSetArg(wargs[i], XtNallowShellResize , True);i++;
    top_bitmap_aide = XbvlCreateApplicationShell(DOC_BIT_NAME, DOC_APP_CLASS,
					  wargs, i, NULL);
    XtAddCallback(top_bitmap_aide, XtNdestroyCallback, (XtCallbackProc) cleanVar, &top_bitmap_aide );
  }

  i = 0;
  XtSetArg(wargs[i], XtNtitle, top_bitmap_name); i++;
  XtSetValues(top_bitmap_aide, wargs, i);

  if(!bitmap_viewport) {
    i = 0;
    XtSetArg(wargs[i], XtNallowHoriz, True); i++;
    XtSetArg(wargs[i], XtNallowVert, True); i++;
    XtSetArg(wargs[i], XtNwidth, w); i++;
    XtSetArg(wargs[i], XtNheight, h); i++;
    bitmap_viewport = XbvlCreateManagedWidget(DOC_BIT_VIEW,
					      viewportWidgetClass  ,
					      top_bitmap_aide,
					      wargs, i, (struct atome **) NULL);
    XtAddCallback(bitmap_viewport, XtNdestroyCallback, (XtCallbackProc) cleanVar, &bitmap_viewport);
  }
  
  i = 0;
  XtSetArg(wargs[i], XtNwidth, w); i++;
  XtSetArg(wargs[i], XtNheight, h); i++;
  if(!bitmap_drawing) {
    bitmap_drawing = XbvlCreateManagedWidget(DOC_BIT_VIEW,
					     drawxbvlWidgetClass  ,
					     bitmap_viewport,
					     wargs, i, (struct atome **) NULL);
    XtAddCallback(bitmap_drawing, XtNdestroyCallback, (XtCallbackProc) cleanVar, &bitmap_drawing);
  }
  else XtSetValues(bitmap_drawing, wargs, i);

  XtSetArg(wargs[i], XtNtitle, top_bitmap_name); i++;
  XtSetValues(top_bitmap_aide, wargs, i);
  XtRealizeWidget(top_bitmap_aide);

  dpy = XtDisplay(bitmap_drawing);

  if(!XReadBitmapFile(dpy, XtWindow(bitmap_drawing), bmp,
		      &w, &h, &pxmp, &x, &y)) {
    gc = ((DrawxbvlWidget)bitmap_drawing)->drawxbvl.gc;
    i = 0;
    XtSetArg(wargs[i], XtNwidth, w); i++;
    XtSetArg(wargs[i], XtNheight, h); i++;
    XtSetValues(bitmap_drawing, wargs, i);
    XCopyPlane(dpy, pxmp, XtWindow(bitmap_drawing), gc, 0, 0, w, h, 0, 0, 1);
    XCopyPlane(dpy, pxmp, ((DrawxbvlWidget)bitmap_drawing)->drawxbvl.pixmap, gc, 0, 0, w, h, 0, 0, 1);
    XFreePixmap(dpy, pxmp);
  }
}
    
/* -----------------------------------------------------*
 *	cherche la definition d'un objet.   		*
 *  retourne la longueur de la definition 	        *
 * -----------------------------------------------------*/
Int findDefinitionObjet(buf_obj, objet)
     CHAR **buf_obj,*objet;
{
  CHAR *buf_ptr;
  Int largo = 0, fd;
  UInt buf_size;
  FILE *f;
  
  
  fflush(stdout);
  
  if((fd = OPENINDEX()) == 0 ) {
    wst("J'arrete.\n");
    XBell(display, 99);
    XBell(display, 99);
    return -1;
  }
  
  read(fd,(CHAR *)&largo,sizeof(int));
  
  
  /* trouver la position de la definition de l'objet */
  while((largo = read(fd , (CHAR *)&tt,sizeof(tt))) > 0) {
    *(index(tt.ligne,'\n'))='\0';
    if(strcmp(tt.ligne, objet) == 0)break;
  }
  close(fd);
  if(largo <= 0){
    wst("Il n'y a pas encore d'aide a ce sujet.\n");
    return -2;
  }
  if((f = FOPENSUJETS()) == NULL){
    wst("J'arrete.\n");
    return -3;
  }
  fseek(f,tt.pos+strlen(objet)+1,0); /**/
  largo = 0;
  buf_ptr = *buf_obj = XtMalloc(buf_size = (BUFSIZ*10));
  buf_ptr[0] = '\0';
  for(;;) {
    CHAR buf[128] ,*bufptr;
    
    bufptr = fgets(buf, 120, f) ;
    if(bufptr == NULL || (buf[0]=='^'&&buf[1]=='\n')) {
      fclose(f);
      return largo;
    }
    if(largo + strlen(bufptr) > buf_size) 
      buf_ptr = *buf_obj = XtRealloc(buf_ptr, buf_size += BUFSIZ);

    if(buf[0] == '^' && buf[1] == '%' && buf[2] == 'B') {
      Int w, h;
      CHAR b_name[80];
      if(sscanf(&buf[3], "%d %d %s", &w, &h, b_name)) affBitmap(w, h, b_name, objet);
    }
    else if(buf[0]!='^') {
      strcat(buf_ptr, buf); /* definition a afficher */
      largo += strlen(bufptr);
    }
  }
}
/* -----------------------------------------------------*
 *	affiche l'aide d'un element de la liste		*
 * -----------------------------------------------------*/
/* ARGSUSED */
void affObjet(w, app_data, objet)
     Widget w;
     caddr_t app_data ;  /*  utilise pour recuperer le fichier d'aide  */
     XtListReturnStruct *objet;  /* element a rechercher */
{
  Int  l, i, *aux;
  Widget w_output, w_top;
  CHAR *objetdef = NULL, *file_name, buf[120], w_name[80], *pere;
  Arg wargs[1];

  file_name = (CHAR *) app_data;
  if(isustr(file_name)) {
    push(Aide_src);
    push(Aide_index);
    Aide_src = strcpy(XtMalloc(strlen(file_name)+5), file_name);    strcat(Aide_src, ".dat");
    Aide_index = strcpy(XtMalloc(strlen(file_name)+6), file_name);  strcat(Aide_index, ".:dat");
  }

  
  if((l = findDefinitionObjet(&objetdef, objet->string))<= 0){ 
    wst("erreur "); wst(objet->string); wst(" non trouvee\n");
    goto fin_aff_Obj;
  }
  if((i = (Int)FOPENSUJETS()) != (Int)NULL) {
    fgets(buf, 120, (FILE *)i);
    if(buf[0] == '^' && buf[1] == '%' && buf[2] == 'H')  /* le titre est la... */
      sprintf(w_name, "%s (%s)\0", &buf[4], objet->string);
    fclose((FILE *)i);
  }
  else sprintf(w_name, "Aide sur %s\0", objet->string);
  /* ecriture de la definition */
  if(new_aide_o)
    w_output = new_aide_o;
  else {
    findWindAide();
    w_output = aide_interf;
  }
  /* recherche de l'application Shell parent pour changer le titre */
  w_top = w_output;
  do {
    pere = w_top->core.parent->core.name;
    if((aux = (Int *)lookfor_atome(pere)) == NULL) { w_top = NULL; break; }
    w_top = GET_WIDGET_FROM_ATOME(aux);
  }  while(w_top && !XtIsSubclass(w_top, applicationShellWidgetClass)) ;

  if(w_top) {
    XtSetArg(wargs[0], XtNtitle, w_name);
    XtSetValues(w_top, wargs, 1);
  }
  changeText(w_output , objetdef);
  XtTextSetInsertionPoint(w_output, (XtTextPosition)0);
  XteLoop();
fin_aff_Obj:
  if(objetdef) XtFree(objetdef);
  if(isustr(file_name)) {
    XtFree(Aide_src);
    XtFree(Aide_index);
    pop_cast(Aide_index, (CHAR *));
    pop_cast(Aide_src, (CHAR *));
  }
  return  ;
}

/*---------------------------------------------------------------------*
 *  si l'interface d'affichage n'existe pas, elle est cree.						 *
 *---------------------------------------------------------------------*/
void findWindAide()
{
  if(! aide_interf &&
     getWidgetFromAtomes(&aide_interf, AIDE_INTERFACE, xbvlispWidgetClass) == False){
    aide_interf =  xCreateXbVlispWidget(AIDE_ROOT, AIDE_INTERFACE,
					xvl_data.lignes,
					xvl_data.colonnes + 1,
					xvl_data.pages,
					-1, -1);
    XtAddCallback(aide_interf, XtNdestroyCallback, (XtCallbackProc) cleanVar, & aide_interf );
  }
}

/* ----------------------------------------------------------------------*
 * fait le lien entre une variable interne du systeme et un widget 
 * cree par le programmeur.
 *-----------------------------------------------------------------------*/

Boolean getWidgetFromAtomes(ww, name, w_classe) 
     Widget * ww;
     CHAR * name;
     WidgetClass w_classe;
{
  Int *att;
  Widget w;
  
  if((att = (Int *)lookfor_atome(name)) != NULL
     &&  isWidget(att)
     && XtIsSubclass((w =  GET_WIDGET_FROM_ATOME(att)), w_classe)){
    *ww = w;
    XtAddCallback(w, XtNdestroyCallback, (XtCallbackProc) cleanVar, ww );
    return True;
  }
  return False;
}


/********************************************
  Action: enleve les fenetres liees a l'aide
 ********************************************/
void enleve_aide(w, event, params, num_params) 
     Widget w;
     XKeyEvent *event;
     String * params;
     Cardinal * num_params;
{

#ifdef XtSpecificationRelease
  CoreWidget pw = aide_interf;
#else
  WidgetRec *pw = (WidgetRec *) aide_interf;
#endif
  Widget wxaide;
  CHAR *pere;
  Int *tmp;

  /* et si l'aide est gérée par une fonction Lisp ??? */
  if(*ataide != (Int) nil && isWidget(*ataide) && (wxaide = GET_WIDGET_FROM_ATOME(*ataide)))
    XteDestroyWidget(wxaide);
  
  if(top_aide && isWidget(top_aide)) XteDestroyWidget(top_aide);
  if(top_bitmap_aide && isWidget(top_bitmap_aide)) XteDestroyWidget(top_bitmap_aide);

  if(aide_interf && isWidget(aide_interf)) {
    do {
#ifdef XtSpecificationRelease
      pere = ((CoreWidget)pw->core.parent)->core.name;
#else
      pere = ((WidgetRec *)pw->core.parent)->core.name;
#endif
      if((tmp = (Int *)lookfor_atome(pere)) == NULL) break;
      pw = GET_WIDGET_FROM_ATOME(tmp);
    }  while(pw && !XtIsSubclass(pw, applicationShellWidgetClass)) ;

    if(pw) XteDestroyWidget(pw);
  }
  aide_interf = top_aide = top_bitmap_aide = NULL;
  return;
}
