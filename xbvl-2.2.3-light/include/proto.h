
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 	proto.h
 * 	Ernesto Sendoya	
 *      janvier 1992
 */



#ifndef proto_h
#define proto_h

#ifdef STDC_HEADERS

#ifndef SYSCINC
char  * index();
#endif

/* xdraw.c */

/* xtypes.c <<<<<<<<<<<< */
extern Boolean convertBvlToXval(Widget w,XtArgVal * to_val, CHAR * nom, XtArgVal val, Int type);
extern Boolean convertStringToVal(Widget, XtArgVal *, CHAR * , XtArgVal );
extern Boolean convertXvalToBvl(Widget w, CHAR * name, union u_XtArgVal * val);
extern Boolean convertIntToVal(Widget w, XtArgVal *to_val, CHAR * name, XtArgVal val) ;
extern Boolean convertFloatToVal(Widget w, XtArgVal * to_val, CHAR * name, XtArgVal val );
 extern void init_quarks(void);

/* strings.c <<<<<<<<<<<< */
extern void genstring(CHAR *);
extern Int comstr1(void);
extern Int finstr(void);
extern Int newppn(void);
extern void newstr(CHAR *);
extern void genstring(CHAR *);
extern void finstr_normal(void);

/* xgetargs <<<<<<<<<<<< */
extern Int get_noms(CHAR ** nomtop, CHAR ** nomwind, Int *nb);
extern Int get_ints( Int * premier, Int * suivant, Int * nb);
extern Int get_oneNBR(Int * nbr_ret);
extern Int get_oneOBJET( XtArgVal * data );
extern Int getNameFromArg(CHAR ** s, Int *at );
extern CHAR * getAtomeName(struct atome * at);
extern Widget get_widget(Int *n);
extern Int  get_oneName( CHAR **nom);
extern Widget getApplicationWidget(void);
extern Int getWinParams(Int * n1, Int * nc, Int * begy, Int * begx, Int * pages);
extern init_oneNBR(Int *nbr);
extern init_oneINT(Int *data );
Int getNomArgWin( CHAR *valnom, Widget * widgette_ptr, Int *n);

/* xcrewin.c */

extern Widget CreateManagedXbVlispWidget(CHAR *name, Widget w_p, ArgList wargs, Int n, struct atome ** at);
#if 0
extern Int * GetAtome(CHAR *name) ;
#endif
extern Int * GetListElement(CHAR *name) ;
extern Int * genat(CHAR *name) ;
extern Widget XbvlCreateApplicationShell(CHAR * name, CHAR *Classe_Name, ArgList  wargs, Int i, struct atome ** aat);
extern Widget XbvlCreateManagedWidget(CHAR *n, WidgetClass c, Widget p, ArgList a, Int i, struct atome ** aat);
extern Widget XbvlCreateWidget(CHAR *n, WidgetClass c, Widget p, ArgList a, Int i, struct atome ** aat);


/* xcrewin.c */
extern Int * GetAtome(CHAR *str );
extern Int * GetListElement(CHAR *str );

/* xaffiche.c <<<<<<<<<<<<< */
#if 0
extern Int wst(CHAR * string);
#endif
extern Int fwst(CHAR * string);
extern Int fwint(Int num);
extern Int appendText(Widget fenetre, Int debut,  CHAR * chaine);

extern eXtTextReplace(Widget w , XtTextPosition start_pos, XtTextPosition end_pos, XtTextBlock  * text);

extern Int tailleNextLigne(CHAR *  s);
extern Int PreparAppend(CHAR * cpy_buf, CHAR  * string, Int offset, Int cols);
extern Int next_mot(CHAR * str);
#if 0
extern Int is_lettre(Int c);
#endif
extern formatBuffer(CHAR * buffer, Int lignes, Int cols) ;
extern Int preparTrunque(CHAR * trunque_buf, Int moislignes, Int cols);


/* hash.c <<<<<<<<<<<< */
extern Int hashpjw(CHAR * s, Int taille);
extern Int insert_atome(struct atome *, Int );
extern Boolean enleve_atome(struct atome * );
extern Int get_hashind(struct atome * );
#if 0
extern struct atome * lookfor_atome(CHAR * name);
#endif
extern CHAR * getAtomName (struct atome * at);

/* xCalbks.c <<<<<<<<<<<< */

extern void stop_xbvl(Widget w, caddr_t app_data, caddr_t  call_data);
extern void cree_new_window(Widget w, caddr_t app_data, caddr_t  call_data);
extern void comToXbvl(Widget wind, String to_xbvl, caddr_t call_data);
extern void cleanVar(Widget w, Widget * app_w, caddr_t c_d);

/* xActions.c <<<<<<<<<<<< */
extern void cree_bVLISP_window(Widget w, XKeyEvent * event, String * params, Cardinal * num_params );
extern void faire_beep( Widget w, XKeyEvent * event, String * params, Cardinal * num_params );
/* extern void change_wind_mode(Widget w, XKeyEvent * event, String * params, Cardinal * num_params ); */
extern void toggle_background_color( Widget w, XEvent * e, String * params, Cardinal * num_params );
extern void toggle_foreground_color(Widget w,  XEvent * e, String * params, Cardinal * num_params );
extern void toggle_border_color(Widget w, XEvent * e, String * params, Cardinal * num_params );
extern void  stop_x(Widget w, XEvent * e , String * params, Cardinal * num_params );
extern void communicExpressions(Widget w, XEvent * e , String * params, Cardinal * num_params );
extern void communiqExpressions(Widget w, XEvent * e , String * params, Cardinal * num_params );

/* xhandls.c */

#ifndef XtSpecificationRelease
extern XtEventHandler handleEnterMenu( Widget  w, caddr_t unused,  register XFocusChangeEvent *event);
#else  /* apres la relaise 4, un argument un plus */
extern XtEventHandler handleEnterMenu( Widget  w, caddr_t unused,  register XFocusChangeEvent *event, Boolean * bidon);
#endif

/* xELIMINE.C , fonctions a remplacer par des fonctions LISP */
extern Int  changeWidgetBord(Widget w, Dimension bord);
extern Int changeWidgetColorBord(Widget w, CHAR *colorname);

/* misc.c <<<<<<<<<<<< */
extern void FreeStringTable(String * string_table);
extern Int  numToString(CHAR * buf, Int * bvlnum);
extern FILE * fopenFile(CHAR * var_name, CHAR * default_name);
extern Int openFile(CHAR * var_name, CHAR * default_name);
extern Int  cmpsujs(String *psujet1, String *psujet2);
extern Int faireGrab(Widget);
extern Int faireUnGrab(void);
extern Int netoy_text(CHAR * ptr, Int length, Int mode);
extern Int save_texte(CHAR **abuf, CHAR *  ptr, Int length);
extern Int GetStringsInfos(String * s_list, Int * ii);
extern Boolean isInteger( CHAR * str );
extern Boolean isFloat( CHAR *str );
extern Boolean isString( CHAR *str );
/* main.c <<<<<<<<<<<< */
extern Int * ncons(Int * n),
  * cranb(Int n ),
  * cons(Int * x, Int * y),
  valnb(Int * x);
extern XtActionProc jump_start(Widget w, XKeyEvent * event, String * params, Cardinal * num_params ); 

/* eval.c */
extern Int apply_careful(void);
extern Int apply_no_careful(void);
extern Int eval_careful(void);
extern Int eval_no_careful(void);
extern Int evca1_careful(void);
extern Int evca1_no_careful(void);
extern Int evalfu_careful(void);
extern Int evalfu_no_careful(void);

/* pour le mode careful ou non careful */
Int (*eval)(void), (*evca1)(void), (*evalfu)(void), (*apply)(void);

/* err.c */
extern Int check_error(void);
extern Int check_arg(void);

/* ind.c <<<<<<<<<<<< */
/* auxiliaires */
extern Int clr2(Int * la_liste);
extern Int  clrocc(void);
extern Int clear1a(Int * la_liste);
extern Int clearall(void);
extern Int * funion(Int * le_atom, Int * la_liste, Int indic);
extern occurloc(Int * ax1, Int *  ax2 );
extern adapteival(Int * indic,Int * variabl1,Int * variabl2);
extern Int testpro(void);
extern Int * find(Int indic, Int * la_liste);
extern Int func(void);
extern Int clause(void);

/* in.c  <<<<<<<<<<<< */
/* auxiliaires */
extern Int inchb(void), 
  getch1(void),
  getch_1(void),
  getcv(void),
  getcv_2(void),
  comm_out(void),
  comm_in(void),
  rd1(void),
  rd1_1(void),
  rsharp(void),
  rshar_1(void),
  rdmac(void),
  rdmac11(void),
  rdmac10(void),
  rdst(void),
  rdst_1(void),
  rdst_2(void),
  objlis(void),
  rdst_3(void),
  rd2(void),
  restaure(void),
  rd4(void),
  restaure1(void),
  rd4_1(void),
  tryat(void),
  crato(void),
  readind(void),
  readu(void),
  allrend(void),
  allread(void),
  readu_1(void),
  readi(void),
  read0(void),
  read1_1(void),
  read214(void),
  read215(void),
  read213(void),
  read212(void),
  read2_1(void),
  read3_1(void),
  read3_2(void),
  read3(void),
  rread43_A(void),
  ead4(void),
  read43(void),
  red48_1(void),
  red48_2(void),
  read52(void),
  rdmaq(void),
  rdmabq(void),
  rdmabq_1(void),
  rdmau(void),
  implo_1(void),
  readlin(void),
  readc_1(void),
  peekc_1(void);

/* out.c <<<<<<<<<<< */
extern Int terpri(void),
  prini(void),
  probj(void),
  prob1(void),
  pannot(void),
  pann1(void),
  prob3_1A(void),
  prob4A(void),
  probc(void),
  pro32(void),
  pro31A(void),
  pro31(void),
  probc33(void),
  probc33(void),
  prob3(void),
  prob3_1(void),
  prch(void),
  prch_1(void),
  prspc(void),
  prato_11(void),
  prato(void),
  prato_2(void),
  pvch(void),
  pvcch(void),
  prnum(void),
  prstr(void),
  prstr_1(void),
  nilall(void),
  prich_1(void),
  terpr0(void);

/* in.c */
#if 0
extern Int * cherchat( CHAR *  adstr );
extern Int * creatom(CHAR * adstr);
#endif
extern Int * cherchComm(Int * x);
extern Int * cherchcomm(Int * x);
 

/* car.c <<<<<<<<<<<< */
extern Int * CAR(Int * x), * CDR(Int * x), 
  * car (Int * x), * cdr (Int * x );

/* num.c <<<<<<<<<<<< */
extern Int convert(void);
extern Int ipow(Int x, Int n);
extern Int plnum(void);

/* flo.c <<<<<<<<<<<< */
extern double valflo (Int * x);
Int * craflo(double f);
extern Int prflo(void),
  veriflo(void);
  
/* spec.c <<<<<<<<<<<< */
extern Int titr1(void),
  makem1(void),
  makem2(void),
  titr2(void),
  unstep1(void),
  pour1(void),
  macout(void),
  break3(void),
  restart(void),
  break1(void);
extern XtActionProc  TITRE(Widget w, XEvent * e, String * params, Cardinal * num_params);

/* indinit.c <<<<<<<<<<< */
extern indinit(void);

/* init.c <<<<<<<<<<<<< */
#ifndef NO_VFLO
extern init(Int tflo, Int tnum, Int tstr, Int tat, Int tlst , Int tpil) ;
#else
     extern init(Int tnum,Int tstr,Int tat,Int tlst,Int tpil) ;
#endif
     extern Int initer(void);
     extern Int initat(void);

/* xinit.c <<<<<<<<<<<< */
extern void X_init(void);
extern void cree_emnuVlisp(void);
/* extern void  getwinressorces(Widget interface, vlisp_data *  win_data); */
extern void mapmenu(Position * x, Position * y );

/* xmenu.c <<<<<<<<<<<< */
extern void PlaceMenu( Widget w, XButtonEvent * event,  String * params, Cardinal * num_params);
extern void PaneChosen(Widget w, Int pane_number, caddr_t call_data);
extern void init_pop_up_menu(Widget top_widget);

/* xkill.c */
extern void XteDestroyWidget(Widget w);


/* xgetch.c <<<<<<<<  */
extern Int xgetch1(XEvent *e);
extern void XteLoop(void);
extern void communiqCharToVlisp ( Widget w, XKeyEvent *e, String *p, Cardinal *n);

/* aide.c <<<<<<<<< */
extern void liste_aide(Widget w, XKeyEvent *event, String * params,  Cardinal * num_params);
extern void enleve_aide(Widget w, XKeyEvent *event, String * params,  Cardinal * num_params);
extern void select_aide( Widget w, XKeyEvent *event, String *params, Cardinal * num_params);

/* xSetValuess.c <<<<<<<< */
extern void XteSetValue(Widget w, String n, XtArgVal lv);
extern Boolean changeWidgetListOfString(Widget widget, String  str);
extern Boolean changeWidgetList(Widget widget, String * str);

/* xetvals.c <<<<<<<< */
extern void XteGetValue(Widget w, String n, XtArgVal * lv);

/* xpointer.c */
extern void  posPointerCentreWin(Widget wid);

/* Convert.c */

extern Int * ConvertValueToAtomeListe (CHAR *strPtr, CHAR * resType, Int * tofree);
extern Int cvtStrToStrLst(String ** ret_list, String  start);

#else

extern void finstr_normal();


/* xgetargs <<<<<<<<<<<< */
extern Widget get_widget();
extern Widget getApplicationWidget();
extern CHAR * getAtomeName();


/* xcrewin.c */
Widget CreateManagedXbVlispWidget();
/* crewin.c */
extern Int * GetListElement() ;
extern Int * genat();
extern Widget XbvlCreateApplicationShell();
extern Widget XbvlCreateManagedWidget();
extern Widget XbvlCreateWidget();
extern Int * GetListElement();

/* strings.c <<<<<<<<<<<< */
extern void genstring();
extern void finstr_normal();
extern void newstr();

/* xaffiche.h <<<<<<<<<<<<< */
extern void InsertText();

#ifndef XtSpecificationRelease
extern eXtTextReplace();
#else
extern eXawTextReplace();
#endif
/*
#ifndef SYSCINC
extern index();
#endif
*/
/* hash.c <<<<<<<<<<<< */
extern Int hashpjw();
extern Int insert_atome();
extern Boolean enleve_atome();
extern Int get_hashind();
#if 0
extern struct atome * lookfor_atome();
#endif
extern CHAR * getAtomName ();

/* xcursor.c <<<<<<<<<<<< */


/* xbufin.c <<<<<<<<<<<< */

/* xGC.c <<<<<<<<<<<< */
extern GC init_gc();
extern GC init_gc_Xor();
extern GC init_gc_draw();
extern GC invert_gc();
extern void init_interfaceGCs();
extern void init_tortueGCs();

/* xCalbks.c <<<<<<<<<<<< */

extern void stop_xbvl();
extern void cree_new_window();
extern void comToXbvl();
extern void cleanVar();

/* xactions.c <<<<<<<<<<<< */
extern void cree_bVLISP_window();
extern void insert_selection();
extern void delete_selection();
extern void end_of_line();
extern void faire_beep();
/* extern void change_wind_mode(); */
extern void toggle_background_color();
extern void toggle_foreground_color();
extern void toggle_border_color();
extern void  stop_x();
extern void communiqExpressions();

/* xhandls.c */
extern XtEventHandler majEditBuf();
extern XtEventHandler handleEnterMenu();
extern XtEventHandler expose_draw();

/* misc.c <<<<<<<<<<<< */
extern void FreeStringTable();
extern FILE *fopenFile();
extern Int  cmpsujs();
extern Int netoy_text();
extern Int save_texte();
extern Boolean isInteger();
extern Boolean isFloat();
extern Boolean isString();

/* xkill.c */
extern void XteDestroyWidget();

/* main.c <<<<<<<<<<<< */
extern Int * ncons(),
  * cranb(),
  * cons(),
  valnb();
extern XtActionProc jump_start();

/* eval.c */
extern Int apply_careful();
extern Int apply_no_careful();
extern Int eval_careful();
extern Int eval_no_careful();
extern Int evca1_careful();
extern Int evca1_no_careful();
extern Int evalfu_careful();
extern Int evalfu_no_careful();

/* pour le mode careful ou non careful */
Int (*eval)(), (*evca1)(), (*evalfu)(), (*apply)();

/* err.c */
extern Int check_error();
extern Int check_arg();

/* ind.c <<<<<<<<<<<< */
/* auxiliaires */
extern Int * funion();
extern Int * find();

/* in.c  <<<<<<<<<<<< */
#if 0
extern Int * cherchat();
extern Int * creatom();
#endif
extern Int * cherchComm();
extern Int * cherchcomm();

/* auxiliaires */

/* out.c <<<<<<<<<<< */
extern Int fwst();
extern Int fwint();
extern Int * cherchComm(),
  * cherchcomm();
 

/* car.c <<<<<<<<<<<< */
extern Int * CAR(), * CDR(), 
  * car (), * cdr ();

/* num.c <<<<<<<<<<<< */

/* flo.c <<<<<<<<<<<< */
extern double valflo ();
extern Int * craflo(),
  prflo(),
  veriflo();
  
/* spec.c <<<<<<<<<<<< */
extern XtActionProc  TITRE();

/* indinit.c <<<<<<<<<<< */

/* init.c <<<<<<<<<<<<< */
#ifndef NO_VFLO

#else

#endif

/* xinit.c <<<<<<<<<<<< */
extern void cree_menuVlisp();
extern void  getwinressorces();
extern void mapmenu();

/* xmenu.c <<<<<<<<<<<< */
extern void PlaceMenu();
extern void PaneChosen();
extern void init_pop_up_menu();

/* xgetch.c <<<<<<<<  */
extern Int xgetch1();
extern void XteLoop();
extern void communiqCharToVlisp ();
 
/* aide.c <<<<<<<<<<<< */
/* action  de lancement de l'aide et action d'affichage des infos d'un sujet */
extern void liste_aide();
extern void enleve_aide();
extern void select_aide();

/* xSetValuess <<<<<<<<<< */
extern void XteSetValue();
extern Boolean changeWidgetListOfString();
extern Boolean changeWidgetList();

/* xSetValuess <<<<<<<<<< */
extern void XteGetValue();

/* xpointer.c */
extern void  posPointerCentreWin();

/* Convert.c */
extern Int * ConvertValueToAtomeListe ();
extern Int cvtStrToStrLst();

#endif




#endif
