# SUBDIRS = bitmap

EXTRA_DIST = EDIT.vlisp GL-Drive-Help.txt GL-drive.vlisp GL-lib.vlisp LisptoTex \
	LisptoTex.c NewComms.txt README \
	Tree.vlisp X-Menus.vlisp adapt.sed adapte aide.c aidvl-gl.txt \
	aidvl-so.txt aidvl1.txt aidvl2.txt aidvlr.txt all.vlisp azer.demo \
	azer.vlisp boy.vlisp bvlisp.ini bvlisp.sys con.vlisp \
	cons-p.vlisp conte.vlisp conv.vlisp cour.vlisp editAide.txt ef-pg.vlisp \
	eff.vlisp essai.vlisp eval.vlisp eval1.vlisp getdoc.vlisp gg.vlisp \
	i-val.vlisp io.vlisp lib.vlisp libpro.vlisp lisp_names.h match.ccl \
	match.vlisp min.vlisp minser.vlisp mon.vlisp mon.vlisp.ok paraf.vlisp \
	permut.vlisp phenar.vlisp pretty.vlisp profib.vlisp prt-pg.vlisp \
	prt.vlisp prtt.vlisp rain.vlisp rec.vlisp record.vlisp rev.vlisp \
	simpl.vlisp stepper.vlisp stp.vlisp test.demo test.vlisp tfunc.vlisp \
	tortue.vlisp traaide.c traaide.script trac1.vlisp trace-pg.vlisp \
	trace.vlisp vcg.cro vcg.vlisp vlisp.ini widget.vlisp xfile.vlisp \
	xbvl.el

bin_PROGRAMS = traaide aide

lisp_LISP = xbvl.el

LISPLIBDIR = @prefix@/share/xbvlisp
aide: traaide
	$(CC) $(CFLAGS) -o aide aide.c
	cat aidvl?.txt > aidvlisp.dat
	if test -n "${AIDE_OPENGL}" ; then \
	    cat aidvl-gl.txt >> aidvlisp.dat ; \
	    cp  GL-Drive-Help.txt GL-Drive-Help.dat ; \
	    ./traaide f GL-Drive-Help ; \
	fi
	cat aidvl-so.txt >> aidvlisp.dat
	./traaide f aidvlisp

traaide: traaide.c lisp_names.h
	$(CC) $(CFLAGS) -o traaide traaide.c


install-data-am:
	$(mkinstalldirs) $(LISPLIBDIR)
#	$(INSTALL_DATA) bitmap/* $(BITMAPLIB)
	$(INSTALL_DATA) *.vlisp  $(LISPLIBDIR)
	$(INSTALL_DATA) *dat     $(LISPLIBDIR)
	$(INSTALL_DATA) *sec     $(LISPLIBDIR)
	$(INSTALL_DATA) *sys     $(LISPLIBDIR)


doc:  traaide
	./traaide l L f aidvlisp
	latex aidvlisp
	latex aidvlisp
	makeindex aidvlisp.idx
	latex aidvlisp
	dvips -o aidvlisp.ps aidvlisp.dvi

lisp_names.h: ../src_lisp/init.c
	grep "mkat(\"" ../src_lisp/init.c | sed -e s/.*mkat\(// | sed -e s/\".*\",.*/\"\1\",/ | sed -e s/\^\",/\",\",/ > src_atoms_tmp
	sed -e s/\"pour\",/\"*pour*\",/ < src_atoms_tmp | sort > src_atoms
	echo \#define NUM_LISP_NAMES `wc -l < src_atoms` > lisp_names.h
	echo static char *lisp_names\[NUM_LISP_NAMES+1\] = \{ >> lisp_names.h
	cat src_atoms >> lisp_names.h
	echo "NULL };" >> lisp_names.h
	rm -f src_atoms*
