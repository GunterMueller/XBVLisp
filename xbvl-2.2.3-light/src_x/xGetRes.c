/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* fonctions pour recuperer des ressources du monde bVLISP
 *               xGetRes.c
 *		Ernesto Sendoya
 *		  Mars 92
 */

#include <Objets.h>
/*--------------------------------------------------------------------------------
  On garde les differentes informations des ressources dans une hash-table
  --------------------------------------------------------------------------------*/
#define RES_TAB  17

struct info_res {
  String resource_name;
  String resource_type;
  struct info_res *next;
  struct info_res *c_next;
} * res_tab[RES_TAB];

struct class_info_res {
  WidgetClass widget_class;
  struct info_res * res_infos;
  struct class_info_res * next;
} class_infos ;

/* fonctions */

#ifdef STDC_HEADERS
struct info_res * initResHash(String name, String type);
struct info_res * sauveResourceData ( XtResource * resource );
String getResourceType(String res_name, WidgetClass wc );
struct info_res *  getResourceInfo(String res_name );
struct info_res * newInfoRes ( XtResource * res );
struct info_res * newInfosRecord ( String n, String t );
struct info_res * sauveResInfo( struct info_res * r );
struct class_info_res * getClassInfos(WidgetClass w_c);
struct class_info_res * InitializeWidgetClass(WidgetClass w_c);
void initWidgetClasses(void);
#else
struct info_res * initResHash();
struct info_res * sauveResourceData ();
String getResourceType();
struct info_res *  getResourceInfo();
struct info_res * newInfoRes ();
struct info_res * newInfosRecord ();
struct info_res * sauveResInfo();
struct class_info_res * getClassInfos();
struct class_info_res * InitializeWidgetClass();
void initWidgetClasses();
#endif

Int getArgListForWidget(widget, arggg, nbr)
     Widget widget;
     ArgList * arggg;
     Int *nbr;
{
  WidgetClass widget_class =  XtClass(widget);
  WidgetClass pere_class = XtIsSubclass(widget, applicationShellWidgetClass) ? NULL : XtClass(XtParent(widget));
  return getArgList(pere_class, widget_class, arggg, nbr);
}


/* recupere une liste des Ressources d'une liste d'arguments lisp                    */
/* les arguments sont ou bien des triples decrivant completement une ressource       */
/* ou bien des atomes contenant une ressource                                        */
/* on a des triples d'infos dans a1 du syle ([(nom_res rep_type valeur)ou atRes] ...)*/ 
Int getArgList(pere_class, widget_class, arggg, nbr)
     WidgetClass pere_class, widget_class;
     ArgList * arggg;
     Int *nbr;
{
  Int n = 0, get_ok, is_resize = 0 ;
  ArgList result = NULL;
  
  while (isnotnil(a1)){ /* tant que qqch. parser la liste d'arguments */
    if(result == NULL)
      result = (ArgList) XtCalloc((unsigned) 1,(unsigned) sizeof(Arg));
    else if(get_ok == True)
      result = (ArgList) XtRealloc((String)result,(unsigned) (n+1)* (unsigned)sizeof(Arg));
    /* processer une specification de ressource dans le flux d'entree */
    if(islist(car(a1))){
      push(a1);
      a1 = car (a1);
      if((get_ok = getXtArgFromLispArgs(pere_class, widget_class, &result[n])) == True){
	if(!strcmp(result[n].name, "width") ||
	   !strcmp(result[n].name, "height")) is_resize = 1;
	n += 1;
	pop_cast(a1, (Int *));
	a1 = (Int *) cdr(a1);
      }
      /*      printf( "MAVAISE ARG No %d.\n", n);      break;      */
    }
    else if((get_ok = getXtArgFromLispArgs(pere_class, widget_class, &result[n])) == True) {
      if(!strcmp(result[n].name, "width")) is_resize = 1;
      n += 1;
    }
    else{ /*       printf( "MAVAIS ARG No %d\n", n);      break;  */  }
  }
  if(n == 0 && result ) {
    XtFree((String)result);
    result = NULL;
  }
  *nbr = n;
  *arggg = result;
  return is_resize ;
}

/* on traite un triple d'arguments */
Int getXtArgFromLispArgs(pere_class, w_class, argl)
     WidgetClass pere_class, w_class;
     ArgList argl;
{
  char *res_type;
  XtArgVal lisp_val;
  Int lisp_type, liber;
  Int *at = car(a1);
  a1 = cdr(a1);

  /* le nom de la ressource */  
  if(isustr(at))
    argl->name =  XtNewString(STR_FROM_STRLSP(at));
  else if(isGraphicObjet(at)){
    if(testRessource(at)){
      bcopy((char *)GET_RESSOURCE_FROM_ATOME(at), (char*) argl, sizeof(Arg));
      return True;
    }
    /* widget */
    puts("Arg: widget en MAUVAISE possition \n");
    return False; 
  }
  else {/* mauvaise specification de ressource */  
    puts ("Arg: Nom en Mauvaise Position\n");
    return False;
  }

  res_type = getResTypeFromWidgetClass(pere_class, w_class, argl->name);
/*  printf("%s :  repType calcule : %s\n ", argl->name, res_type);*/
  if(!res_type) return False;

  at = car(a1);
  
  /* on recupere la valeur de la ressource */
  if(isGraphicObjet(at)){  /* un objet Graphique */
    a1 = cdr(a1);
    if(testRessource(at)){
      ArgList ress = GET_RESSOURCE_FROM_ATOME(at);
      argl->value = ress->value;
      return True;
    }
    /* widget */
    argl->value =  (XtArgVal)GET_WIDGET_FROM_ATOME(at);
    return True;
  }
  /*                         Ou une donnee LISP pure */
  if((lisp_type = get_oneOBJET(&lisp_val)) == VIDE 
     || lisp_type == INCONNU
     || lisp_type == ERROR){
    XtFree(argl->name);
    XtFree(res_type);
    if (lisp_type == ERROR) 
      return ERROR; /* structure de liste LISP abime ! */
    a1 = cdr(a1);
    return False;
  }
  a1 = cdr(a1);
  argl->value =  getValueRessource((char *) lisp_val, res_type, lisp_type, &liber);
  if(liber == ERROR) return False ;
  return True;
}

     
/* realise la conversion d'une ressource */
XtArgVal  getValueRessource( val_res, xrep_type, lisp_typeval, ret)
     CHAR *val_res, *xrep_type;
     Int lisp_typeval;
     Int *ret;     /* signale si c'est une ressource 
		      qui a allouee de la memoire, ou si la conversion a echoue */
{
  switch(lisp_typeval){
  case NAME:
  case STRING:
    return getValueString( val_res, xrep_type, ret); 
  case INTEGER:
    return getValueInteger( (Int)val_res, xrep_type, ret); 
  case FLOAT:
    return getValueFloat( (float *)val_res, xrep_type, ret); 
  case WIDGETTE:
  case RESSOURCE:
    return (XtArgVal)val_res;
  case LISTE:
    return getValueListe( val_res, xrep_type, ret); 
  default:
    return (XtArgVal)NULL;
  }
}

XtArgVal  getValueListe(strPtr, resType, tofree)
     CHAR *strPtr, *resType;
     Int *tofree;
{ 
  XrmValue from, to;
  XtArgVal toValue;
  Cardinal nb_args = 0;
  if (strcmp(resType, XtRStringList)){
    *tofree = False ;
    return((XtArgVal) strPtr);
  }
  from.size = sizeof(String *);
  from.addr = strPtr;
  to.addr = NULL;
  *tofree = False;
  _bCvtAtomListToStringList(NULL,&nb_args,&from,&to);
  if(to.size == 0){
    *tofree = ERROR;
    return (XtArgVal)NULL;
  }
  *tofree = True;
  toValue =  *((XtArgVal *) to.addr);
  return toValue;
}

XtArgVal getValueString(strPtr, resType, tofree)
  CHAR *strPtr, *resType;
  Int *tofree;
{
  XrmValue from, to;
  XtArgVal toValue;
  
  if (strcmp(resType, XtRString) == 0){
    *tofree = True ;
    return((XtArgVal) XtNewString(strPtr));
  }
  from.size = strlen(strPtr) + 1;
  if (from.size < sizeof(String)) from.size = sizeof(String);
  from.addr = strPtr;
  to.addr = NULL;
  *tofree = False;
  XtConvert(top_widget, XtRString, &from, resType, &to);
  if(to.size == 0){
    *tofree = ERROR;
    return (XtArgVal)NULL;
  }
  if (to.size == sizeof(XtArgVal))
    toValue =  *((XtArgVal *) to.addr);
  else if (to.size == sizeof(short))
    toValue = (XtArgVal) *((short *) to.addr);
  else if (to.size == sizeof(char))
    toValue = (XtArgVal) *((char *) to.addr);
  else if (to.size < sizeof(XtArgVal))
    bcopy ((char *)to.addr, (char *)&toValue, (Int)to.size);
  else {  /* to.addr est un pointeur ver data */
    /*   c'est pas necessaire de le faire 
      toValue = (XtArgVal) XtMalloc(to.size);
      bcopy ((char *)to.addr, (char *)toValue, (Int)to.size); 
     */
    toValue = (XtArgVal)to.addr;
    *tofree = True;
  }
  return((XtArgVal) toValue);
}

XtArgVal getValueInteger(intval, resType, tofree)
     Int intval;
     CHAR *resType;
     Int *tofree;
{
  XrmValue from, to;
  XtArgVal toValue;
  
  *tofree = False ;
  if(strcmp(resType, XtRInt) == 0)
    return (XtArgVal) intval;
  
  from.size = sizeof(Int);
  from.addr = (caddr_t) &intval ;
  to.addr = NULL;
  *tofree = False;
  XtConvert(top_widget, XtRInt, &from, resType, &to);
  if(to.size == 0){
    *tofree = ERROR;
    return 0;
  }
  if (to.size == sizeof(XtArgVal))
    toValue =  *((XtArgVal *) to.addr);
  else if (to.size == sizeof(short))
    toValue = (XtArgVal) *((short *) to.addr);
  else if (to.size == sizeof(char))
    toValue = (XtArgVal) *((char *) to.addr);
  else if (to.size < sizeof(XtArgVal))
    bcopy ((char *)to.addr, (char *)&toValue, (Int)to.size);
  else {  /* to.addr est un pointeur ver data */
    toValue = (XtArgVal) XtMalloc(to.size);
    bcopy ((char *)to.addr, (char *)toValue, (Int)to.size);
    *tofree = True;
  }
  return((XtArgVal) toValue);
}


XtArgVal getValueFloat(floatval, resType, tofree)
     float *floatval;
     CHAR *resType;
     Int *tofree;
{
  XrmValue from, to;
  XtArgVal toValue;
  
  *tofree = False ;
  if(strcmp(resType, XtRFloat) == 0)
    return (XtArgVal) floatval;
  
  *tofree = False;
  from.size = sizeof(float);
  from.addr = (caddr_t)floatval ;
  to.addr = NULL;
  if(strcmp(resType, XtRFloat) == 0){
    toValue = (XtArgVal) XtMalloc(sizeof(float));
    bcopy ((char *)&floatval, (char *)&toValue, sizeof(float));
    *tofree = True;
  }
  XtConvert(top_widget, XtRFloat, &from, resType, &to);
  if(to.size == 0){
    *tofree = ERROR;
    return 0;
  }
  if (to.size == sizeof(XtArgVal))
    toValue =  *((XtArgVal *) to.addr);
  else if (to.size == sizeof(short))
    toValue = (XtArgVal) *((short *) to.addr);
  else if (to.size == sizeof(char))
    toValue = (XtArgVal) *((char *) to.addr);
  else if (to.size < sizeof(XtArgVal))
    bcopy ((char *)to.addr, (char *)&toValue, (Int)to.size);
  else {  /* to.addr est un pointeur ver data */
    toValue = (XtArgVal) XtMalloc(to.size);
    bcopy ((char *)to.addr, (char *)toValue, (Int)to.size);
    *tofree = True;
  }
  return((XtArgVal) toValue);
}


/*--------------------------------------------------------------------------------
  retrouve la representation du type correspondant au nom de la ressource.
          Trouve la super-classe et la classe du widget et appel
	  getResTypeFromWidgetClass .
  --------------------------------------------------------------------------------*/
String getResTypeFromWidget(widget , resource_name)
     Widget widget;
     String resource_name;
{
  WidgetClass widget_class =  XtClass(widget);
  WidgetClass pere_class = 
    XtIsSubclass(widget, applicationShellWidgetClass) ? NULL : XtClass(XtParent(widget));
  return getResTypeFromWidgetClass(pere_class, widget_class, resource_name);
}

/* 
 * introuduit les informations dans la hash-table
 */

struct info_res * sauveResourceInfo( res )
     XtResource * res;
{
  return sauveResInfo(newInfoRes(res));
}


struct info_res * sauveResInfo(r)
     struct info_res * r;
{
  Int hash_ind = hashpjw(r->resource_name, RES_TAB);
  struct  info_res * cour;
  
  if((cour = res_tab[hash_ind])){
    /* on inserre le nouveau element a la queue 
    while(cour->next)
      cour = res_tab[hash_ind]->next ;  
    cour->next = r;                                     */

    res_tab[hash_ind] = r;
    r->next = cour;
    
  } else res_tab[hash_ind] = r;
  return r;
}

struct info_res * initResHash(name, type)
     String name;
     String type;
{
  struct info_res *new ;
  
  if(new = getResourceInfo(name))
    return new;
  new = newInfosRecord (name, type);
  sauveResInfo(new);
  return new;
    
}

struct info_res * newInfoRes (res)
     XtResource * res;
{
  return newInfosRecord(res->resource_name, res->resource_type);
}

 
struct info_res *  newInfosRecord (name, type)
     String name, type;
{
  struct info_res * new = XtNew(struct info_res);
  new->resource_name = XtNewString(name);
  new->resource_type = XtNewString(type);
  new->next = new->c_next = (struct info_res *)NULL;
  return new;
}
/*
 * ramene la prepresentation du type de la ressource
 */

String getResourceType(res_name, w_c)
     String res_name;
     WidgetClass w_c;
{
  struct info_res *ret = getResourceInfo(res_name);
  
  if(ret) return XtNewString(ret->resource_type);
  /* data non initialise pour cette classe ? */
  (void)InitializeWidgetClass(w_c); 
  if(ret = getResourceInfo(res_name))
    return XtNewString(ret->resource_type);
  return NULL;
}
struct info_res *getResourceInfo(res_name)
     String res_name;
{
  Int ind = hashpjw(res_name, RES_TAB);
  struct info_res * cour = res_tab[ind];
  
  for (; cour; cour = cour->next){
    if ( !strcmp(res_name, cour->resource_name) )
      return cour;
  }
  return NULL;
}

/* 
 * sauve les informations de la ressource si c'est ne pas encore fait
 */

struct info_res * sauveResourceData ( resource )
     XtResource * resource;
{
  struct info_res * ret;
  if((ret = getResourceInfo(resource->resource_name)) == NULL)
    ret = sauveResourceInfo (resource);
  return ret;
}


initStaticRes()
{
  (void)initResHash(XtNlist, XtRStringList);
  (void)initResHash(XtNchildren, XtRAtomeListe);
  (void)initResHash(XtNparent, XtRAtome);
  /* faire la recherche a la main */
#ifndef CONTINU_MY_SYS_BIZARRE
  (void)initResHash(XtNallowResize, XtRBoolean);
  (void)initResHash(XtNmax, XtRDimension);
  (void)initResHash(XtNmin, XtRDimension);
  (void)initResHash(XtNskipAdjust, XtRBoolean);
#endif
  (void)initResHash(XtNbottom, XtRInt); 
  (void)initResHash(XtNfromHoriz, XtRWidget);
  (void)initResHash(XtNfromVert, XtRWidget);
  (void)initResHash(XtNhorizDistance, XtRInt);
  (void)initResHash(XtNresizable, XtRBoolean);
  (void)initResHash(XtNright, XtRInt);  /* A confirmer pour FORM */
  (void)initResHash(XtNtop, XtRInt);  /* A confirmer pour FORM */
  (void)initResHash(XtNvertDistance, XtRInt);
}
/*--------------------------------------------------------------------------------
  retrouve la representation du type correspondant au nom de la ressource.
  note: cette fonction doit etre acompagnee par un systeme de caches
  pour faire un appel a XtGetResourceList par classe de widget.
  --------------------------------------------------------------------------------*/
char * getResTypeFromWidgetClass(pere_class, w_class, resource_name)
     WidgetClass pere_class, w_class;
     String resource_name;
{
  char * rtype = getResourceType(resource_name, w_class);
  if(rtype) return rtype;
  if(pere_class) return  getResourceType(resource_name, pere_class);
  return NULL;
}


/*--------------------------------------------------------------------------------
  ramene la representation du type  du widget de la ressource passee en parametre 
  retrouve la representation du type d'un nom d'une ressource.
              Note: pour regarder les Representations de Type d'une Resource.
  --------------------------------------------------------------------------------*/

Int xGetResRepType() /* (GetResRepType [widget] non-ressource) subr2 */
{
  if(isWidget (a1)){
    Widget widget =  GET_WIDGET_FROM_ATOME(a1);
    String ressource_name;
    if(getNameFromArg((CHAR **)&ressource_name, a2) > 0){
      WidgetClass widget_class =  XtClass(widget);
      WidgetClass pere_class = 
	XtIsSubclass(widget, applicationShellWidgetClass) ? NULL : XtClass(XtParent(widget));
      String ress_rep_type = getResTypeFromWidgetClass(pere_class, widget_class, ressource_name);

      genstring(ress_rep_type);
      if(ress_rep_type) XtFree(ress_rep_type);
    } else a1 = nil;
  } else a1 = nil;
  derec;
}


/*--------------------------------------------------------------------------------
  ramene la liste de ressources d'une classe de widgets ainsi que la representation
  de type pour chaque ressource.
  --------------------------------------------------------------------------------*/

Int xGetResourceList() /* (GetResourceList widget_class) subr1 */
{
  WidgetClass widget_class = NULL, pere_class = NULL;
  String class_name;
  int num_ress, i;
  Int *list_res = nil;
  XtResourceList ress_list = NULL;
  
  if(isWidget (a1)){
    Widget widget =  GET_WIDGET_FROM_ATOME(a1);
    widget_class =  XtClass(widget);
    pere_class = 
      XtIsSubclass(widget, applicationShellWidgetClass) ? NULL : XtClass(XtParent(widget));
  }
  else if(getNameFromArg((CHAR **)&class_name, a1) > 0)
    widget_class =  getWidgetClassFromString(class_name);
  else {
    a1 = nil; 
    derec;
  }
  if(widget_class){
    XtGetResourceList(widget_class, &ress_list, &num_ress);
    for(i = 0; i < num_ress; i++){
      Int * aux = nil;
      genstring(ress_list[i].resource_type);
      aux = cons(a1, aux);
      genstring(ress_list[i].resource_name);
      aux = cons(a1, aux);
      list_res = cons(aux, list_res);
    }
    if( ress_list )
      XtFree((String)ress_list);
#ifdef XtSpecificationRelease
    if(pere_class){
      XtGetConstraintResourceList(pere_class, &ress_list, &num_ress);
      for(i = 0; i < num_ress; i++){
	Int * aux = nil;
	genstring(ress_list[i].resource_type);
	aux = cons(a1, aux);
	genstring(ress_list[i].resource_name);
	aux = cons(a1, aux);
      list_res = cons(aux, list_res);
      }
      if( ress_list )
	XtFree((String)ress_list);
    }
#endif  
  }
  a1 = list_res;
  derec;
}
/*--------------------------------------------------------------------------------
  ramene la liste de ressources d'une classe de widgets ansi que la representation
  de type pour chaque ressource.
  --------------------------------------------------------------------------------*/

Int xGetNewResourceList() /* (GetNewRessourceList widget_class) subr1 */
{
  WidgetClass widget_class = NULL, pere_class = NULL;
  String class_name;
  struct class_info_res *i_class;
  struct info_res *i_res;
  Int * list_res = nil;

  if(isWidget (a1)){
    Widget widget =  GET_WIDGET_FROM_ATOME(a1);
    widget_class =  XtClass(widget);
    pere_class = 
      XtIsSubclass(widget, applicationShellWidgetClass) ? NULL : XtClass(XtParent(widget));
  }
  else if(getNameFromArg((CHAR **)&class_name, a1) > 0)
    widget_class =  getWidgetClassFromString(class_name);
  else {
    a1 = nil; 
    derec;
  }

  if((i_class = getClassInfos(widget_class)) == NULL)
    i_class = InitializeWidgetClass(widget_class);

  for(i_res = i_class->res_infos; i_res; i_res = i_res->c_next){
    Int * aux = nil;
    genstring(i_res->resource_type);
    aux = cons(a1, aux);
    genstring(i_res->resource_name);
    aux = cons(a1, aux);
    list_res = cons(aux, list_res);
  }
  a1 = list_res;
  derec;
}

/*--------------------------------------------------------------------------------
  ramene la liste de ressources ansi que la representation de type pour chaque 
  classe initialisee.
  --------------------------------------------------------------------------------*/

Int xGetAllResourceLists() /* (GetAllRessourceLists) subr0 */
{
  struct class_info_res * info_c ;
  Int * lilist_res = nil;
  
  for (info_c  = &class_infos; info_c->widget_class ; ){
    if(info_c->res_infos){
      struct info_res * i_res;
      Int * list_res  = nil;
      for(i_res = info_c->res_infos; i_res; i_res = i_res->c_next){
	Int * aux = nil;
	genstring(i_res->resource_type);
	aux = cons(a1, aux);
	genstring(i_res->resource_name);
	aux = cons(a1, aux);
	list_res = cons(aux, list_res);
      }
      if(list_res != nil)
	lilist_res = cons(list_res, lilist_res);
    }
    if(info_c->next)
      info_c = info_c->next;
    else break;
  }
  a1 = lilist_res;
  derec;
}

struct class_info_res * getClassInfos(widget_class)
     WidgetClass widget_class;
{
  struct class_info_res *c_infos = &class_infos;
  while(c_infos->next && c_infos->widget_class != widget_class){
    c_infos = c_infos->next;
  }
  if(c_infos->widget_class && c_infos->widget_class == widget_class)
    return c_infos;
  return NULL;
}

/* 
 * initialisation des informations de ressources
 *      pour la classe passe en parametre
 */

struct class_info_res *  InitializeWidgetClass(widget_class)
     WidgetClass widget_class;
{ 
  int i;
  struct class_info_res *new, *c_infos;
  int num_ress;
  XtResourceList ress_list = NULL;
  struct info_res * newone = NULL;

  if((new = getClassInfos(widget_class)))
    return new;  /* init. deja faite */
  
  c_infos  = &class_infos;
  /* init a faire */
  if(c_infos->widget_class){
    new =  XtNew(struct class_info_res);
    new->widget_class = widget_class;
    new->next = NULL;
    /* on inserre les infos. a la queue */
    while(c_infos->next)
      c_infos = c_infos->next;
    c_infos->next = new;
  }
  else { /* premiere initialisation */
    new = &class_infos;
    new->widget_class = widget_class;
    new->next =  NULL;
  }
  
  XtGetResourceList(widget_class, &ress_list, &num_ress);
  for(i = 0; i < num_ress; i++){
    struct info_res * ret = getResourceInfo(ress_list[i].resource_name);
    if( !newone && !ret )
      new->res_infos = newone =  sauveResourceInfo(&ress_list[i]);
    else if( !ret ){
      newone->c_next = sauveResourceInfo(&ress_list[i]);
      newone = newone->c_next;
    }
  }
  if( ress_list ) XtFree((String)ress_list);
#ifdef XtSpecificationRelease
  XtGetConstraintResourceList(widget_class, &ress_list, &num_ress);
  for(i = 0; i < num_ress; i++){
    struct info_res * ret =  getResourceInfo(ress_list[i].resource_name);
    if( !newone && !ret)
      new->res_infos = newone =  sauveResourceInfo(&ress_list[i]);
    else if( !ret ){
      newone->c_next = sauveResourceInfo(&ress_list[i]);
      newone = newone->c_next;
    }
  }
  if(ress_list) XtFree((String)ress_list);
#endif
  
#if 0
#ifdef XtSpecificationRelease
  XtInitializeWidgetClass(oneClass);
#endif
#endif
  return new;
}

void initWidgetClasses()
{
  initStaticRes();
  return ;
#if 0

  (void)InitializeWidgetClass(widgetClass);
  (void)InitializeWidgetClass(compositeWidgetClass);
  (void)InitializeWidgetClass(constraintWidgetClass);
  (void)InitializeWidgetClass(applicationShellWidgetClass);
  (void)InitializeWidgetClass(simpleWidgetClass);
  (void)InitializeWidgetClass(viewportWidgetClass);
  (void)InitializeWidgetClass(listWidgetClass);
  (void)InitializeWidgetClass(formWidgetClass);
  (void)InitializeWidgetClass(boxWidgetClass);
  (void)InitializeWidgetClass(xbvlispWidgetClass);
  (void)InitializeWidgetClass(tortueObjectClass);
#ifndef CONTINU_MY_SYS_BIZARRE
  (void)InitializeWidgetClass(panedWidgetClass);
#endif
  (void)InitializeWidgetClass(viewportWidgetClass);
  (void)InitializeWidgetClass(drawxbvlWidgetClass);
  (void)InitializeWidgetClass(dialogWidgetClass);
#if XtSpecificationRelease > 4
  (void)InitializeWidgetClass(pannerWidgetClass);
  (void)InitializeWidgetClass(portholeWidgetClass);
  (void)InitializeWidgetClass(repeaterWidgetClass);
  (void)InitializeWidgetClass(treeWidgetClass);
#endif

#endif
}

