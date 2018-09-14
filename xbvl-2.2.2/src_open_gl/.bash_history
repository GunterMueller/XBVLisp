tar cvfz work.old.tar.gz work
rm -rf work
tar xvfz tmp/WORK.TGZ 
cd tmp/xbvlisp.2.1/
cd src_open_gl/
sed -e s/GL_SHORT/GLXBVL_SHORT/g < glFuncs.c > glFuncs.c.i
grep SHORT glFuncs.c.i 

cd ../include/
cd ../src_open_gl/
for i in *.c; do sed -e s/GL_CHAR/GLXBVL_CHAR/g < glFuncs.c > glFuncs.c.i
for i in *.c; do  sed -e s/GL_CHAR/GLXBVL_CHAR/g < $i > $i.n; mv $i.n $i; sed -e s/GL_FLOAT/GLXBVL_FLOAT/g < $i > $i.n; mv $i.n $i; sed -e s/GL_DOUBLE/GLXBVL_DOUBLE/g < $i > $i.n; mv $i.n $i; done
eit
exit
