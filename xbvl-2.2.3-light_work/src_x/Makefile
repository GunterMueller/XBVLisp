# Makefile.in generated automatically by automake 1.4-p6 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/bash

srcdir = .
top_srcdir = ..

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/xbvl
pkglibdir = $(libdir)/xbvl
pkgincludedir = $(includedir)/xbvl

top_builddir = ..

ACLOCAL = /home/gunter/GM_LANGUAGES/Functional_Languages/LISP_Dialects/XBVLisp/xbvl-2.2.3-light_work/missing aclocal-1.4
AUTOCONF = autoconf
AUTOMAKE = /home/gunter/GM_LANGUAGES/Functional_Languages/LISP_Dialects/XBVLisp/xbvl-2.2.3-light_work/missing automake-1.4
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = x86_64-unknown-linux-gnu
ARFLAGS = ruv
CC = gcc
CPP = gcc -E
DATA_TYPE = int
EMACS = /usr/bin/emacs
HAVE_DIRNAME = -DHAVE_INDEX
HAVE_ERRNO = -DERRNO
HAVE_INDEX = 
HAVE_LIB = @HAVE_LIB@
HAVE_PROTOENT = -DGETPROTENT
HAVE_XRBFD = 
LIB = @LIB@
LN_S = ln -s
LTLIB = @LTLIB@
MAKEINFO = makeinfo
MODIFIEDMESA = 
NO_VFLO = 
PACKAGE = xbvl
RANLIB = ranlib
SYS_CFLAGS =  -pipe -DSHM
SYS_LDFLAGS =  
TIFF_DEF = 
TIFF_LIB = 
VERSION = 2.2.3-light
XAW_DIR = 
XAW_LIB = 
X_USE_LATIN1 = -DX_USE_LATIN1
lispdir = $(prefix)/share/emacs/site-lisp

noinst_LIBRARIES = libxbvlx.a

INCLUDES = -I$(top_srcdir)/include
CFLAGS = -O3  -pipe -DSHM  -DX_USE_LATIN1

libxbvlx_a_SOURCES = BrTree.c Convert.c Drawxbvl.c ObjAtome.c 	ObjInit.c Tortue.c XbvlActn.c XbvlVideo.c Xbvlisp.c XtToLISP.c 	crewin.c cursor.c 	xActions.c xBitmap.c xCalbks.c xCrObjet.c xDraw.c 	xEvent.c xGetRes.c xTranslation.c xWdgFncs.c xaide.c 	xcrewin.c xgetargs.c xgetch.c xgetvals.c xinit.c xkill.c 	xmenu.c xpointer.c xsetvals.c xsrc.c xvter.c xwst.c 

mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
LIBRARIES =  $(noinst_LIBRARIES)


DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE=\"xbvl\" -DVERSION=\"2.2.3-light\" -DSTDC_HEADERS=1  -I. -I$(srcdir) 
CPPFLAGS = 
LDFLAGS = 
LIBS = 
X_CFLAGS = 
X_LIBS = 
X_EXTRA_LIBS = 
X_PRE_LIBS =  -lSM -lICE
libxbvlx_a_LIBADD = 
libxbvlx_a_OBJECTS =  BrTree.o Convert.o Drawxbvl.o ObjAtome.o ObjInit.o \
Tortue.o XbvlActn.o XbvlVideo.o Xbvlisp.o XtToLISP.o crewin.o cursor.o \
xActions.o xBitmap.o xCalbks.o xCrObjet.o xDraw.o xEvent.o xGetRes.o \
xTranslation.o xWdgFncs.o xaide.o xcrewin.o xgetargs.o xgetch.o \
xgetvals.o xinit.o xkill.o xmenu.o xpointer.o xsetvals.o xsrc.o xvter.o \
xwst.o
AR = ar
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(libxbvlx_a_SOURCES)
OBJECTS = $(libxbvlx_a_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps src_x/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-noinstLIBRARIES:

clean-noinstLIBRARIES:
	-test -z "$(noinst_LIBRARIES)" || rm -f $(noinst_LIBRARIES)

distclean-noinstLIBRARIES:

maintainer-clean-noinstLIBRARIES:

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

libxbvlx.a: $(libxbvlx_a_OBJECTS) $(libxbvlx_a_DEPENDENCIES)
	-rm -f libxbvlx.a
	$(AR) cru libxbvlx.a $(libxbvlx_a_OBJECTS) $(libxbvlx_a_LIBADD)
	$(RANLIB) libxbvlx.a

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags -o $$here/TAGS $(ETAGS_ARGS) $$tags  $$unique $(LISP))

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = src_x

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
BrTree.o: BrTree.c ../include/BrTreeP.h ../include/BrTree.h
Convert.o: Convert.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
crewin.o: crewin.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
cursor.o: cursor.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
Drawxbvl.o: Drawxbvl.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h ../include/DrawxbvlP.h \
	../include/TortueP.h
ObjAtome.o: ObjAtome.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
ObjInit.o: ObjInit.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
Tortue.o: Tortue.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h ../include/TortueP.h \
	../include/DrawxbvlP.h
xActions.o: xActions.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
xaide.o: xaide.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h ../include/aide.h ../include/xaide.h \
	../include/vlisp_files.h ../include/DrawxbvlP.h \
	../include/TortueP.h
xBitmap.o: xBitmap.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h ../include/DrawxbvlP.h \
	../include/TortueP.h ../include/XbvlispP.h ../include/actions.h
XbvlActn.o: XbvlActn.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h ../include/XbvlispP.h \
	../include/actions.h
Xbvlisp.o: Xbvlisp.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h ../include/XbvlispP.h \
	../include/actions.h
XbvlVideo.o: XbvlVideo.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h ../include/XbvlispP.h \
	../include/actions.h
xCalbks.o: xCalbks.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
xcrewin.o: xcrewin.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
xCrObjet.o: xCrObjet.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
xDraw.o: xDraw.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h ../include/DrawxbvlP.h ../include/TortueP.h \
	../include/XbvlispP.h ../include/actions.h
xEvent.o: xEvent.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
xgetargs.o: xgetargs.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
xgetch.o: xgetch.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
xGetRes.o: xGetRes.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
xgetvals.o: xgetvals.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h ../include/BrTreeP.h
xinit.o: xinit.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
xkill.o: xkill.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
xmenu.o: xmenu.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
xpointer.o: xpointer.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
xsetvals.o: xsetvals.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
xsrc.o: xsrc.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
xTranslation.o: xTranslation.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
XtToLISP.o: XtToLISP.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
xvter.o: xvter.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
xWdgFncs.o: xWdgFncs.c ../include/Objets.h ../include/vlisp.h \
	../include/fons.h ../include/xwin.h ../include/R3toR4.h \
	../include/BrTree.h ../include/Xbvlisp.h ../include/Drawxbvl.h \
	../include/Tortue.h ../include/proto.h
xwst.o: xwst.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h

info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile $(LIBRARIES)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-noinstLIBRARIES mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-noinstLIBRARIES clean-compile clean-tags clean-generic \
		mostlyclean-am

clean: clean-am

distclean-am:  distclean-noinstLIBRARIES distclean-compile \
		distclean-tags distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-noinstLIBRARIES \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-noinstLIBRARIES distclean-noinstLIBRARIES \
clean-noinstLIBRARIES maintainer-clean-noinstLIBRARIES \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir info-am info dvi-am dvi check \
check-am installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
