bin_PROGRAMS = xbvl
INCLUDES = -I$(top_srcdir)/include
CFLAGS = @CFLAGS@ @XAW_DIR@ @X_USE_LATIN1@

xbvl_SOURCES = aide.c fdate.c gc.c ind.c list.c num.c sofons.c strings.c \
	car.c err.c fil.c hash.c indinit.c misc.c out.c spec.c  \
	contro.c eval.c flo.c in.c init.c menu.c mod.c prolog.c str.c main.c # xgetvals.c

xbvl_LDADD = @X_LIBS@ -lX11 @XAW_DIR@ @XAW_LIB@ $(top_srcdir)/src_x/libxbvlx.a -lm

EXTRA_DIST = Zeugma-Help.dat foobar #essaiGL dlist.c mesaListes.c # 'Zeugma-Help.:dat' Zeugma-Help.\:sec

# Flags de compilation spécifiques :
main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(ERRNO) -c $<

# # init ne supporte pas d'optimisattion en inline (1/4 h)
INIT_CFLAGS = $(DEFS) $(SYS_CFLAGS) $(X_CFLAGS) @MODIFIEDMESA@ -O
init.o: init.c
	$(CC) $(CFLAGS) $(INCLUDES) $(INIT_CFLAGS) -c $<

# # sofons demande une série de checks particuliers
SODEFS =  @HAVE_ERRNO@ @HAVE_PROTOENT@
sofons.o: sofons.c
	$(CC) $(CFLAGS) $(INCLUDES) $(SODEFS) -c $<
