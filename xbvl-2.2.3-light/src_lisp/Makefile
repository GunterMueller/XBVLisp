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
infodir = ${prefix}/share/info
mandir = ${prefix}/share/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/xbvl
pkglibdir = $(libdir)/xbvl
pkgincludedir = $(includedir)/xbvl

top_builddir = ..

ACLOCAL = /home/gunter/GM_LANGUAGES/Functional_Languages/LISP_Dialects/XBVLisp_GM/xbvl-2.2.3-light/missing aclocal-1.4
AUTOCONF = autoconf
AUTOMAKE = /home/gunter/GM_LANGUAGES/Functional_Languages/LISP_Dialects/XBVLisp_GM/xbvl-2.2.3-light/missing automake-1.4
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

bin_PROGRAMS = xbvl
INCLUDES = -I$(top_srcdir)/include
CFLAGS = -O3  -pipe -DSHM  -DX_USE_LATIN1

xbvl_SOURCES = aide.c fdate.c gc.c ind.c list.c num.c sofons.c strings.c 	car.c err.c fil.c hash.c indinit.c misc.c out.c spec.c  	contro.c eval.c flo.c in.c init.c menu.c mod.c prolog.c str.c main.c # xgetvals.c


xbvl_LDADD = $(top_srcdir)/src_x/libxbvlx.a -lXt -lXaw -lXmu -lX11 -lm

EXTRA_DIST = Zeugma-Help.dat foobar #essaiGL dlist.c mesaListes.c # 'Zeugma-Help.:dat' Zeugma-Help.\:sec

# # init ne supporte pas d'optimisattion en inline (1/4 h)
INIT_CFLAGS = $(DEFS) $(SYS_CFLAGS) $(X_CFLAGS)  -O

# # sofons demande une série de checks particuliers
SODEFS = -DERRNO -DGETPROTENT
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS)


DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"xbvl\" -DVERSION=\"2.2.3-light\" -DSTDC_HEADERS=1 -I. -I$(srcdir) 
CPPFLAGS = 
LDFLAGS = 
LIBS = 
X_CFLAGS = 
X_LIBS = 
X_EXTRA_LIBS = 
X_PRE_LIBS =  -lSM -lICE
xbvl_OBJECTS =  aide.o fdate.o gc.o ind.o list.o num.o sofons.o \
strings.o car.o err.o fil.o hash.o indinit.o misc.o out.o spec.o \
contro.o eval.o flo.o in.o init.o menu.o mod.o prolog.o str.o main.o
xbvl_DEPENDENCIES =  $(top_srcdir)/src_x/libxbvlx.a
xbvl_LDFLAGS = 
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(xbvl_SOURCES)
OBJECTS = $(xbvl_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps src_lisp/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

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

xbvl: $(xbvl_OBJECTS) $(xbvl_DEPENDENCIES)
	@rm -f xbvl
	$(LINK) $(xbvl_LDFLAGS) $(xbvl_OBJECTS) $(xbvl_LDADD) $(LIBS)

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

subdir = src_lisp

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
aide.o: aide.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h ../include/aide.h ../include/xaide.h \
	../include/vlisp_files.h
car.o: car.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
contro.o: contro.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
err.o: err.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
eval.o: eval.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
fdate.o: fdate.c ../include/vlisp.h ../include/fons.h ../include/xwin.h
fil.o: fil.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
flo.o: flo.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
gc.o: gc.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
hash.o: hash.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
in.o: in.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
ind.o: ind.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
indinit.o: indinit.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
list.o: list.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
menu.o: menu.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
misc.o: misc.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
mod.o: mod.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
num.o: num.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
out.o: out.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
prolog.o: prolog.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h
spec.o: spec.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
str.o: str.c ../include/vlisp.h ../include/fons.h ../include/xwin.h \
	../include/R3toR4.h ../include/BrTree.h ../include/Xbvlisp.h \
	../include/Drawxbvl.h ../include/Tortue.h ../include/proto.h \
	../include/Objets.h
strings.o: strings.c ../include/vlisp.h ../include/fons.h \
	../include/xwin.h ../include/R3toR4.h ../include/BrTree.h \
	../include/Xbvlisp.h ../include/Drawxbvl.h ../include/Tortue.h \
	../include/proto.h ../include/Objets.h

info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-binPROGRAMS
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-binPROGRAMS mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-binPROGRAMS clean-compile clean-tags clean-generic \
		mostlyclean-am

clean: clean-am

distclean-am:  distclean-binPROGRAMS distclean-compile distclean-tags \
		distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-binPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir info-am info dvi-am dvi check \
check-am installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


# Flags de compilation spécifiques :
main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(ERRNO) -c $<
init.o: init.c
	$(CC) $(CFLAGS) $(INCLUDES) $(INIT_CFLAGS) -c $<
sofons.o: sofons.c
	$(CC) $(CFLAGS) $(INCLUDES) $(SODEFS) -c $<

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
