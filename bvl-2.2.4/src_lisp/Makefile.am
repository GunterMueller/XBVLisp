bin_PROGRAMS = bvl
INCLUDES = -I$(top_srcdir)/include
CFLAGS = -g @CFLAGS@ 

bvl_SOURCES = fdate.c gc.c ind.c list.c num.c sofons.c strings.c \
 	car.c err.c fil.c hash.c indinit.c misc.c out.c spec.c  \
 	contro.c eval.c flo.c in.c init.c mod.c prolog.c main.c

bvl_LDADD = -lm

# Flags de compilation spécifiques :
main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) $(ERRNO) -c $<

# # init ne supporte pas d'optimisattion en inline (1/4 h)
INIT_CFLAGS = $(DEFS) $(SYS_CFLAGS) -O
init.o: init.c
	$(CC) $(CFLAGS) $(INCLUDES) $(INIT_CFLAGS) -c $<

# # sofons demande une série de checks particuliers
SODEFS =  @HAVE_ERRNO@ @HAVE_PROTOENT@
sofons.o: sofons.c
	$(CC) $(CFLAGS) $(INCLUDES) $(SODEFS) -c $<
