/* 
 *	sofons.c
 * definition des sockets et interface des sockets
 * Ce fichier contient toutes les fonction primitives necesaires pour
 * la gestion de sockets sous lisp
 * O.Blanc fevrier 95  
 */

/* #ifdef SOCKETS */

# include <vlisp.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <sys/param.h>
# include <netinet/in.h>
# include <netdb.h>
#ifdef ERRNO
# include <errno.h>
#endif

typedef struct _ArgumentsSockets {
  char *name ;
  Int value ;
} ArgumentsSockets ;


static ArgumentsSockets domainNames[] = {
#ifdef PF_UNSPEC
  {"UNSPEC", 	PF_UNSPEC},	/* unspecified */
#endif /* PF_UNSPEC */

#ifdef AF_UNIX
  {"UNIX", 	AF_UNIX},	/* local to host (pipes, portals) */
#else
#ifdef PF_UNIX
  {"UNIX", 	PF_UNIX},	/* local to host (pipes, portals) */
#endif /* PF_UNIX */
#endif

#ifdef AF_INET
  {"INET", 	AF_INET},	/* internetwork: UDP, TCP, etc. */
#else
#ifdef PF_INET
  {"INET", 	PF_INET},	/* internetwork: UDP, TCP, etc. */
#endif /* PF_INET */
#endif

#ifdef AF_IMPLINK
  {"IMPLINK", 	AF_IMPLINK},	/* arpanet imp addresses */
#else
#ifdef PF_IMPLINK
  {"IMPLINK", 	PF_IMPLINK},	/* arpanet imp addresses */
#endif /* PF_IMPLINK */
#endif

#ifdef PF_PUP
  {"PUP", 	PF_PUP},	/* pup protocols: e.g. BSP */
#endif /* PF_PUP */

#ifdef PF_CHAOS
  {"CHAOS", 	PF_CHAOS},	/* mit CHAOS protocols */
#endif /* PF_CHAOS */

#ifdef AF_NS
  {"NS", 	AF_NS},		/* XEROX NS protocols */
#else
#ifdef PF_NS
  {"NS", 	PF_NS},		/* XEROX NS protocols */
#endif /* PF_NS */
#endif

#ifdef PF_NBS
  {"NBS", 	PF_NBS},	/* nbs protocols */
#endif /* PF_NBS */

#ifdef PF_ECMA
  {"ECMA", 	PF_ECMA},	/* european computer manufacturers */
#endif /* PF_ECMA */

#ifdef PF_DATAKIT
  {"DATAKIT", 	PF_DATAKIT},	/* datakit protocols */
#endif /* PF_DATAKIT */

#ifdef PF_CCITT
  {"CCITT", 	PF_CCITT},	/* CCITT protocols, X.25 etc */
#endif /* PF_CCITT */

#ifdef PF_SNA
  {"SNA", 	PF_SNA},	/* IBM SNA */
#endif /* PF_SNA */

#ifdef PF_DECnet
  {"DECnet", 	PF_DECnet},	/* DECnet */
#endif /* PF_DECnet */

#ifdef PF_DLI
  {"DLI", 	PF_DLI},	/* Direct data link interface */
#endif /* DLI */

#ifdef PF_LAT
  {"LAT", 	PF_LAT},	/* LAT */
#endif /* PF_LAT */

#ifdef PF_HYLINK
  {"HYLINK", 	PF_HYLINK},	/* NSC Hyperchannel */
#endif /* PF_HYLINK */

#ifdef PF_APPLETALK
  {"APPLETALK", PF_APPLETALK},	/* Apple Talk */
#endif /* PF_APPLETALK */

#ifdef PF_NIT
  {"NIT", 	PF_NIT},	/* Network Interface Tap */
#endif /* PF_NIT */

#ifdef PF_802
  {"802", 	PF_802},	/* IEEE 802.2, also ISO 8802 */
#endif /* PF_802 */

#ifdef PF_OSI
  {"OSI", 	PF_OSI},	/* umbrella for all families used by OSI (e.g. protosw lookup) */
#endif /* OSI */

#ifdef PF_ISO
  {"OSI", 	PF_ISO},	/* umbrella for all families used by OSI (e.g. protosw lookup) */
#endif /* PF_ISO */

#ifdef PF_X25
  {"X25", 	PF_X25},	/* CCITT X.25 in particular */
#endif /* PF_X25 */

#ifdef PF_OSINET
  {"OSINET", 	PF_OSINET},	/* AFI = 47, IDI = 4 */
#endif /* PF_OSINET */

#ifdef PF_GOSIP
  {"GOSIP", 	PF_GOSIP},	/* U.S. Government OSI */
#endif /* PF_GOSIP */

  {(Int)NULL,	(Int)NULL}		/* ALWAYS THERE */
} ;


#ifndef GETPROTENT
static ArgumentsSockets protocolNames[] = {
#ifdef  	IPPROTO_IP
  {"IP", 	IPPROTO_IP},	/* dummy for IP */
#endif /* 	IPPROTO_IP*/
#ifdef  	IPPROTO_ICMP
  {"ICMP", 	IPPROTO_ICMP},	/* control message protocol */
#endif /* 	IPPROTO_ICMP*/
#ifdef  	IPPROTO_IGMP
  {"IGMP", 	IPPROTO_IGMP},	/* group control protocol */
#endif /* 	IPPROTO_IGMP*/
#ifdef  	IPPROTO_GGP
  {"GGP", 	IPPROTO_GGP},	/* gateway^2 (deprecated) */
#endif /* 	IPPROTO_GGP*/
#ifdef  	IPPROTO_TCP
  {"TCP", 	IPPROTO_TCP},	/* tcp */
#endif /* 	IPPROTO_TCP*/
#ifdef  	IPPROTO_EGP
  {"EGP", 	IPPROTO_EGP},	/* exterior gateway protocol */
#endif /* 	IPPROTO_EGP*/
#ifdef  	IPPROTO_PUP
  {"PUP", 	IPPROTO_PUP},	/* pup */
#endif /* 	IPPROTO_PUP*/
#ifdef  	IPPROTO_UDP
  {"UDP", 	IPPROTO_UDP},	/* user datagram protocol */
#endif /* 	IPPROTO_UDP*/
#ifdef  	IPPROTO_IDP
  {"IDP", 	IPPROTO_IDP},	/* xns idp */
#endif /* 	IPPROTO_IDP*/
#ifdef  	IPPROTO_HELLO
  {"HELLO", 	IPPROTO_HELLO},	/* "hello" routing protocol */
#endif /* 	IPPROTO_HELLO*/
#ifdef  	IPPROTO_ND
  {"ND", 	IPPROTO_ND},	/* UNOFFICIAL net disk proto */
#endif /* 	IPPROTO_ND*/
#ifdef  	IPPROTO_RAW
  {"RAW", 	IPPROTO_RAW},	/* raw IP packet */
#endif /* 	IPPROTO_RAW*/
  {NULL,	NULL}		/* ALWAYS THERE */
} ;
#endif /* GETPROTENT */


static ArgumentsSockets socketTypes[] = {
#ifdef  	SOCK_STREAM
  {"STREAM", 	SOCK_STREAM},	/* stream socket */
#endif /* 	SOCK_STREAM*/
#ifdef  	SOCK_DGRAM
  {"DGRAM", 	SOCK_DGRAM},	/* datagram socket */
#endif /* 	SOCK_DGRAM*/
#ifdef 	SOCK_RAW
  {"RAW",	SOCK_RAW},	/* raw-protocol interface */
#endif /*	SOCK_RAW*/
#ifdef  	SOCK_RDM
  {"RDM", 	SOCK_RDM},	/* reliably-delivered message */
#endif /* 	SOCK_RDM*/
#ifdef  SOCK_SEQPACKET
  {"SEQPACKET", SOCK_SEQPACKET}, /* sequenced packet stream */
#endif /* SOCK_SEQPACKET*/
  {NULL,	(Int)NULL}		/* ALWAYS THERE */
} ;


static char inputSocketBuffer[1024] ;

/* Open a Socket */
Int soCreate()	/* (SOcreate "domain" "type" "protocol") */
{
  Int domain, type, protocol, i, aSocket, option ;
  char *f_domainName = NULL, *f_typeName = NULL, *f_protocolName = NULL ;
#ifdef GETPROTENT
  struct protoent *protocol_entry;
#endif
  
  if(isatom(a1)) f_domainName = STR_FROM_STRLSP(((ATOMEPTR) a1)->pn) ;
  else if(isstr(a1)) f_domainName = STR_FROM_STRLSP(a1) ;

  if(isatom(a2)) f_typeName = STR_FROM_STRLSP(((ATOMEPTR) a2)->pn) ;
  else if(isstr(a2)) f_typeName = STR_FROM_STRLSP(a2) ;

  if(isatom(a3)) f_protocolName = STR_FROM_STRLSP(((ATOMEPTR) a3)->pn) ;
  else if(isstr(a3)) f_protocolName = STR_FROM_STRLSP(a3) ;

  /* Get domain info */
  for(i = 0 ; domainNames[i].name != (char *)NULL ; ++i) 
    if(domainNames[i].name && ! strcasecmp(domainNames[i].name, f_domainName)) {
      domain = domainNames[i].value ;
      break ;
    }
  /* get protocol info */
#ifdef GETPROTENT
  if((protocol_entry = getprotobyname(f_protocolName)) != NULL) {
    protocol = protocol_entry->p_proto;
  }
#else
  for(i = 0 ; protocolNames[i].name != (char *)NULL ; ++i) 
    if(protocolNames[i].name && ! strcasecmp(protocolNames[i].name, f_protocolName)) {
      protocol = protocolNames[i].value ;
      break ;
    }
#endif
  /* get type info */
  for(i = 0 ; socketTypes[i].name != (char *)NULL ; ++i) 
    if(socketTypes[i].name && ! strcasecmp(socketTypes[i].name, f_typeName)) {
      type = socketTypes[i].value ;
      break ;
    }
  if((aSocket = socket(domain, type, protocol))  < 0 && *atcareful == (Int)t) {
#ifdef ERRNO
    wst("Socket: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
  }
  option = 1 ;
#ifdef SO_REUSEADDR
  if(setsockopt(aSocket, SOL_SOCKET, SO_REUSEADDR, (void*) &option, sizeof(option)) < 0 && *atcareful == (Int)t) {
#ifdef ERRNO
    wst("Setsockop: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
  }
#endif
#ifdef SO_REUSEPORT
  if(setsockopt(aSocket, SOL_SOCKET, SO_REUSEPORT, (void*) &option, sizeof(option)) < 0 && *atcareful == t)  {
#ifdef ERRNO
    wst("Setsockop: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
  }
#endif
  a1 = (Int *)aSocket  ;
  derec ;
}



Int soClose() /* (SOclose socket) */
{
  if(isnum(a1)) 
    a1 = (Int *)close(valnb(a1)) ;
  derec ;
}


# define ctol(x) (((0L|(x)[0])<<24)|((0L|(x)[1]) << 16)|((0L|(x)[2])<<8)|(0L|(x)[3]))
# define c4tol(w,x,y,z) (((0L|(w))<<24)|((0L|(x)) << 16)|((0L|(y))<<8)|(0L|(z)))


/* initiate a connection on a socket*/
Int soConnect() /* (SOconnect Socket hostName port) */
{
  Int s = -1, port, b;
  char *hostName;
  
  struct hostent *aHost ;
  struct sockaddr anAddress ;
  struct sockaddr_in *pt = (struct sockaddr_in *) &anAddress ;

  if(isnum(a1))  s = valnb(a1) ;
  else if(isatom(a1)) s = valnb(((ATOMEPTR) a1)->cval) ;
  if(s == -1) {
    a1 = nil ;
    derec ;
  }

  if(!isstr(a2) || isatom(a2)) {
    a1 = nil ;
    derec ;
  }

  /* Address name can either be a host name or a host internet address. */

  if(isatom(a2)) hostName = STR_FROM_STRLSP(((ATOMEPTR) a2)->pn) ;
  else if(isstr(a2)) hostName = STR_FROM_STRLSP(a2) ;

  if(isnum(a3))  port = valnb(a3) ;
  else if(isatom(a3)) port = valnb(((ATOMEPTR) a3)->cval) ;

  aHost = gethostbyname(hostName) ;

  if(aHost) {
    pt->sin_family = aHost->h_addrtype ;
    pt->sin_port = port ;
    bcopy((char*)aHost->h_addr, (char*)&pt->sin_addr.s_addr, aHost->h_length) ;
    if((b = connect(s, & anAddress, sizeof(anAddress)))< 0 && *atcareful == (Int)t)  {
#ifdef ERRNO
      wst("connect: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
    }
  }
  else {
    Int e1, e2, e3, e4 ;

    pt->sin_family = AF_INET;
    pt->sin_port = port ;

    if(!strcasecmp(hostName, "ANY")) {
      pt->sin_addr.s_addr = INADDR_ANY;
      if((b = bind(s, & anAddress, sizeof(anAddress))) < 0 && *atcareful == (Int)t)  {
#ifdef ERRNO
	wst("Bind: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
      }
    }
    else if(sscanf(hostName, "%d.%d.%d.%d", &e1, &e2, &e3, &e4) == 4) {
      pt->sin_addr.s_addr = c4tol(e1,e2,e3,e4) ;
      if((b = connect(s, & anAddress, sizeof(anAddress))) < 0 && *atcareful == (Int)t)  {
#ifdef ERRNO
	wst("Connect: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
      }
    }

  }
  if(b < 0) a1 = nil ;
  else a1 = t ;
  derec ;
}

Int soListen() /* (SOlisten Socket <number of connections>) */
{
  Int s = -1 ;
  Int fd ;
				/* retreive socket number */
  if(isnum(a1)) s = valnb(a1) ;
  else if(isatom(a1)) s = valnb(((ATOMEPTR)a1)->cval) ;
  if(s == -1) {
    a1 = nil ;
    derec ;
  }

  if(!listen(s, valnb(a2)))
    a1 = t ;
  else {
    if(*atcareful == (Int)t) {
#ifdef ERRNO
      wst("Listen: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
    }    
    a1 = nil ;
  }
  derec ;
}

/* output a string on a socket */
static Int writeString(s, aString)
Int s; char *aString;
{
  Int nWrite = 0 ;
  if(*atcareful == (Int)t) wst(aString) ;
  nWrite = write(s, aString, strlen(aString)) ;
  return nWrite ;
}

/* output a number */
static Int writeNum(s, i)
Int s, i;
{
  char buf[50] ;
  sprintf(buf, "%d", i) ;
  if(*atcareful == (Int)t) wst(buf) ;
  return write(s, buf, strlen(buf)) ;
}

/* output a list */
static Int writeList(s, pt)
Int s, *pt;
{
  Int nWrite ;

  for(nWrite = 0 ; ! isnil(pt) ; pt = cdr(pt)) {
    if(isstr(car(pt))) {
      if(nWrite) nWrite += write(s, " ", 1) ;
      nWrite += write(s, "\"", 1) ;
      nWrite += writeString(s, STR_FROM_STRLSP(car(pt))) ;
      nWrite += write(s, "\"", 1) ;
    }
    else if(isnum(car(pt))) {
      if(nWrite) nWrite += write(s, " ", 1) ;
      nWrite += writeNum(s, valnb(car(pt))) ;
    }
    else if(islist(car(pt))) {
      nWrite += write(s, "(", 1) ;
      nWrite += writeList(s, car(pt)) ;
      nWrite += write(s, ")", 1) ;
    }
    else if(isatom(car(pt))) {
      if(nWrite) nWrite += write(s, " ", 1) ;
      nWrite += writeString(s, getAtomName((ATOMEPTR)car(pt))) ;
    }
  }
  return nWrite ;
}

/* output an atom */
static Int writeAtomFval(s, anAtom)
Int s; ATOMEPTR anAtom;
{
  Int nWrite = 0 ;

  if(islist(anAtom->fval)) {
    nWrite += write(s, "(de ", 4) ;
    nWrite += writeString(s, getAtomName(anAtom)) ;
    nWrite += write(s, " ", 1) ;
    nWrite += writeList(s, anAtom->fval) ;
    nWrite += write(s, ")", 1) ;
  }
  else 
    nWrite += writeString(s, getAtomName(anAtom)) ;
  return nWrite ;
}


/* write to a socket */
Int soWrite() /* (SOwrite Socket ...) NSUBR */
{
  Int s = -1 ;
				/* retreive socket number */
  if(isnum(car(a1))) s = valnb(car(a1)) ;
  else if(isatom(car(a1))) s = valnb(((ATOMEPTR)car(a1))->cval) ;
  if(s == -1) {
    a1 = nil ;
    derec ;
  }

  a1 = cdr(a1) ;
  if(isnil(a1)) {
    a1 = t ;
    derec ;
  }

  if(isatom(car(a1)) && ((ATOMEPTR)car(a1))->fval)
    a1 = (Int *)writeAtomFval(s, (ATOMEPTR)car(a1)) ;
  else 
    a1 = (Int *)writeList(s, a1) ;
  derec ;
}

Int soRead() /* (SOread Socket <timeLimit>) */
{
  int s = -1, fd, addLen, numberOfBytes ;
  struct sockaddr anAddress ;
  char *pt = NULL ;
  char *copy = NULL ;
  unsigned long inputSize = 0, newSize = 0 ;
  long nRead = 0 ;
  fd_set ready ;
  struct timeval tv ;

				/* retreive socket number */
  if(isnil(a1)) {
    a1 = nil ;
    derec ;
  }
    
  if(isnum(a1)) s = valnb(a1) ;
  else if(isatom(a1)) s = valnb(((ATOMEPTR)a1)->cval) ;
  if(s == -1) {
    a1 = nil ;
    derec ;
  }

  bzero( (void*) &tv, sizeof(tv)) ;
  if(isnotnil(a2)) {
    if(isnum(a2)) tv.tv_sec = valnb(a2) ;
    else if(isatom(a2)) tv.tv_sec = valnb(((ATOMEPTR)a2)->cval) ;
  }
  if(isnotnil(a3)) {
    if(isnum(a3)) tv.tv_usec = valnb(a3) ;
    else if(isatom(a3)) tv.tv_usec = valnb(((ATOMEPTR)a3)->cval) ;
  }
  if(tv.tv_sec || tv.tv_usec) {
    FD_ZERO(&ready) ;
    FD_SET(s, &ready) ;
    if(select(s + 1, &ready, (fd_set*)0, (fd_set*)0, &tv) < 0 && *atcareful == (Int)t)  {
#ifdef ERRNO
      wst("Select: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
    }
    if(!FD_ISSET(s, &ready)) {
      a1 = 0 ;
      derec ;
    }
  }
  bzero((void*) &anAddress, sizeof(anAddress))  ;
  addLen = sizeof(anAddress) ;
  if((fd = accept(s, &anAddress, &addLen))< 0 && *atcareful == (Int)t)  {
#ifdef ERRNO
    wst("Accept: ") ; wst(strerror(errno)) ; wst("\n") ;
#endif
  }
  
  if(fd < 0) {
    a1 = nil ;
    derec ;
  }
  while((nRead = read(fd, inputSocketBuffer, sizeof(inputSocketBuffer))) > 0) {
    newSize += nRead ;
    copy = (char *) malloc(newSize + 1) ;
    if(!copy) {			/* Verify allocation */
      free(pt) ;
      close(fd) ;
      a1 = nil ;
      derec ;
    }
    if(pt) 
      bcopy(pt, copy, inputSize) ;
    bcopy(inputSocketBuffer, (void*)(copy + inputSize), nRead) ;
    inputSize = newSize ;
    if(pt) free(pt) ;
    pt = copy ;
  }
  close(fd) ;
  if(!pt) {
    a1 = 0 ;
    derec ;
  }
  pt[inputSize] = '\0' ;
  newstr(pt);
  free(pt) ;
  a1 = (Int *)aux1;
/*  pushf(eval); */
 pushf(implode); pushf(explode); derec;
}




Int soHostName() /* (SOhostname) */
{
  static char hName[MAXHOSTNAMELEN] ; /* defined from sys/param.h */
  Int hNameLen = MAXHOSTNAMELEN ;
  Int returnValue = gethostname(hName, &hNameLen) ;
  if(returnValue < 0) {
    a1 = nil ;
    derec ;
  }
  pchar = hName ;
  comstr1() ;
}

/* #endif */ /* HAVE_SOCKET */
