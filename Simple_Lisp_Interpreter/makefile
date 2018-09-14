# -------------------------------------------------------------
# 	makefile pour interpreteur Lisp en C
#       	(lisp a liaison dynamique)
#
#	P. Greussay   1985
#
#	+ modifications  F. Balmas   1990 1997
#
# -------------------------------------------------------------

 
OPT= -c -w -cckr

llisp: main.o eval.o mem.o read.o print.o std.o
	cc main.o eval.o mem.o read.o print.o std.o -o llisp 
main.o: main.h main.c
	cc $(OPT) main.c
eval.o: main.h eval.c
	cc $(OPT) eval.c
mem.o: main.h mem.c
	cc $(OPT) mem.c
read.o: main.h read.c
	cc $(OPT) read.c
print.o: main.h print.c
	cc $(OPT) print.c
std.o: main.h std.c
	cc $(OPT) std.c

