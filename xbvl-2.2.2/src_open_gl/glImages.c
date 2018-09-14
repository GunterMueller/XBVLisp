#if (OPEN_GL | MESA_GL)
/* Ce fichier contiend les définition permettant de lire ou enregistrer des images
   extérieures, ceci en utilisant la librairie tiff pour les fichiers de format tiff.
   On définira aussi ici la possibilité d'importer des bitmaps pour leur utilisation
   en tant que telle, comme pattern stipple pour les polygones ou comme texture.
*/
   
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "libtk/tk.h"
#include "glXbvl.h" 

#define RGB_MAGIC 0xda01

/* Affichage d'une image lue dans un fichier */

static Int display_image_file()
{
  TK_RGBImageRec *image;
  float zx, zy;
  short pos[3];
  char is_rgba;

  pop(is_rgba); 
  pop_cast(image,(TK_RGBImageRec *)); 
  pop(pos[0]); pop(pos[1]);
  pop(pos[2]); pop(zx); pop(zy);

  /* récupération de la taille */
  glRasterPos3sv(pos);
  glPixelZoom(zx, zy);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glDrawPixels(image->sizeX, image->sizeY, ((is_rgba) ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, image->data);
  free_image_data(image);
  a1 = t;
  derec;
}

/* *************************** */
/* gestion des fichiers bitmap */
/* *************************** */

/* chargement d'un fichier bitmap comme définition de style
   de remplissage des polygones. Le bitmap doit être de taille < 32x32
   (GLpolygonStipple fichier_bitmap)
*/
Int
gl_load_polygon_stipple()
{
#ifdef HAVE_XRBFD
  unsigned char *f_name, *data = NULL;
  unsigned int w, h, xh, yh;

  if(!isstr(a1)) { a1 = nil; derec; }
  f_name = STR_FROM_STRLSP(a1);
  if(!XReadBitmapFileData(f_name, &w, &h, &data, &xh, &yh)) {
    if(w != 32 || h != 32) {
      a1 = nil;
      derec;
    }
    glPolygonStipple(data);
    XtFree(data);
    a1 = t;
  }
  else 
#endif
    a1 = nil;

  derec;
}

/* *************************************** */
/* La librairie libtiff n'est pas présente */
/* *************************************** */

#ifndef HAVE_LIBTIFF 

int
gl_dump_window()
{
  wst("La librairie libtiff est nécessaire à l'exécution de cette primitive\n");
  a1 = nil;
  derec;
}
int
gl_read_tiff()
{
  wst("La librairie libtiff est nécessaire à l'exécution de cette primitive\n");
  a1 = nil;
  derec;
}

#else

/* ********************************* */
/* La librairie libtiff est présente */
/* ********************************* */

/* ***************************************** */
/* Première partie : dump sous forme de tiff */
/* ***************************************** */

/* OpenGL image dump, written by Reto Koradi (kor@spectrospin.ch) */

/* This file contains code for doing OpenGL off-screen rendering and
   saving the result in a TIFF file. It requires Sam Leffler's libtiff
   library which is available from ftp.sgi.com.
   The code is used by calling the function StartDump(..), drawing the
   scene, and then calling EndDump(..).
   Please note that StartDump creates a new context, so all attributes
   stored in the current context (colors, lighting parameters, etc.)
   have to be set again beforing performing the actual redraw. This
   can be rather painful, but unfortunately GLX does not allow
   sharing/copying of attributes between direct and nondirect
   rendering contexts. */

#include <tiffio.h>

#define RaiseError(str) fprintf(stderr, "GLdump: %s\n", str)

extern GLObject *wobj;

/* X servers often grow bigger and bigger when allocating/freeing
   many pixmaps, so it's better to keep and reuse them if possible.
   Set this to 0 if you don't want to use that. */
#define KEEP_PIXMAP 0

static FILE *TiffFileP;
static Int Orient;
static Int ImgW, ImgH;
#ifdef USE_PIXMAP
static Bool OutOfMemory;
static Display *Dpy;
static Pixmap XPix = 0;
static GLXPixmap GPix = 0;
static GLXContext OldCtx, Ctx;
#endif

#ifdef USE_PIXMAP
static void
#ifndef __SDTC__
destroyPixmap()
#else
destroyPixmap(void)
#endif
{
  glXDestroyGLXPixmap(Dpy, GPix);
  GPix = 0;
  XFreePixmap(Dpy, XPix);
  XPix = 0;
}

static int
#if STDC_HEADERS
xErrorHandler(Display *dpy, XErrorEvent *evtP)
#else
xErrorHandler(dpy, evtP)
Display *dpy; XErrorEvent *evtP;
#endif
{
  OutOfMemory = True;
  return 0;
}

#endif

static int
#if STDC_HEADERS
writeTiff(void)
#else
writeTiff()
#endif
{
  TIFF *tif;
  Int tiffW, tiffH;
  Int bufSize, rowI;
  unsigned char *buf;
  Int res;
#ifndef USE_PIXMAP
  int gl_pack_align ;
#endif

  tif = TIFFFdOpen(fileno(TiffFileP), "output file", "w");
  if (tif == NULL) {
    RaiseError("could not create TIFF file");
    return 1;
  }

#ifndef USE_PIXMAP
  glGetIntegerv(GL_PACK_ALIGNMENT, &gl_pack_align);
#endif
  if (Orient == 0) {
    tiffW = ImgW;
    tiffH = ImgH;
    bufSize = 4 * ((3 * tiffW + 3) / 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
  } else {
    tiffW = ImgH;
    tiffH = ImgW;
    bufSize = 3 * tiffW;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
  }

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, tiffW);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, tiffH);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, "Xbvl-OpenGL");
  TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, "Image générée depuis Xbvl-Mesa/Open GL");
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (8 * 1024) / (3 * tiffW));
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  buf = malloc(bufSize * sizeof(*buf));

  res = 0;
  for (rowI = 0; rowI < tiffH; rowI++) {
    if (Orient == 0)
      glReadPixels(0, ImgH - 1 - rowI, ImgW, 1,
         GL_RGB, GL_UNSIGNED_BYTE, buf);
    else
      glReadPixels(rowI, 0, 1, ImgH,
         GL_RGB, GL_UNSIGNED_BYTE, buf);

    if (TIFFWriteScanline(tif, buf, rowI, 0) < 0) {
      RaiseError("error while writing TIFF file");
      res = 1;
      break;
    }
  }

  free(buf);

  glPixelStorei(GL_PACK_ALIGNMENT, gl_pack_align);
  TIFFFlushData(tif);
  TIFFClose(tif);

  return res;
}

static int
#if STDC_HEADERS
EndDump(Widget drawW)
#else
EndDump(drawW)
Widget drawW;
#endif
/* Write current image to file. May only be called after StartDump(..).
   Returns 0 on success, calls RaiseError(..) and returns 1 on error. */
{
  Int res;

  res = writeTiff();
  (void) fclose(TiffFileP);

#ifdef USE_PIXMAP
  (void) glXMakeCurrent(Dpy, XtWindow(drawW), OldCtx);

#if KEEP_PIXMAP
#else
  destroyPixmap();
#endif

  glXDestroyContext(Dpy, Ctx);
#endif
  return res;
}

static int
#if STDC_HEADERS
Start_Dump(char *fileName, Int orient, Int w, Int h, Widget drawW)
#else
Start_Dump(filename, orient, w, h, drawW)
char *fileName; 
Int orient, w, h; 
Widget drawW;
#endif
/* Prepare for image dump. fileName is the name of the file the image
   will be written to. If orient is 0, the image is written in the
   normal orientation, if it is 1, it will be rotated by 90 degrees.
   w and h give the width and height (in pixels) of the desired image.
   Returns 0 on success, calls RaiseError(..) and returns 1 on error. */
{
  /*  Widget drawW = GetDrawW(); */ /* the GLwMDrawA widget used */
#ifdef USE_PIXMAP
  XErrorHandler oldHandler;
  Int attrList[10];
  XVisualInfo *visP;
  Int n, i;
#endif

  TiffFileP = fopen(fileName, "w");
  if (TiffFileP == NULL) {
    RaiseError("could not open output file");
    return 1;
  }

  Orient = orient;
  ImgW = w;
  ImgH = h;

#ifdef USE_PIXMAP

#if KEEP_PIXMAP
  if (GPix != 0 && (w != ImgW || h != ImgH))
    destroyPixmap();
#endif

  Dpy = XtDisplay(drawW);

  n = 0;
  attrList[n++] = GLX_RGBA;
  attrList[n++] = GLX_RED_SIZE; attrList[n++] = 8;
  attrList[n++] = GLX_GREEN_SIZE; attrList[n++] = 8;
  attrList[n++] = GLX_BLUE_SIZE; attrList[n++] = 8;
  attrList[n++] = GLX_DEPTH_SIZE; attrList[n++] = 1;
  attrList[n++] = None;
  visP = glXChooseVisual(Dpy,
      XScreenNumberOfScreen(XtScreen(drawW)), attrList);
  if (visP == NULL) {
    RaiseError("no 24-bit true color visual available");
    return 1;
  }

  /* catch BadAlloc error */
  OutOfMemory = False;
  oldHandler = XSetErrorHandler(xErrorHandler);

  if (XPix == 0) {
    XPix = XCreatePixmap(Dpy, XtWindow(drawW), w, h, 24);
    XSync(Dpy, False);  /* error comes too late otherwise */
    if (OutOfMemory) {
      XPix = 0;
      XSetErrorHandler(oldHandler);
      RaiseError("could not allocate Pixmap");
      return 1;
    }
  }

  if (GPix == 0) {
    GPix = glXCreateGLXPixmap(Dpy, visP, XPix);
    XSync(Dpy, False);
    XSetErrorHandler(oldHandler);
    if (OutOfMemory) {
      GPix = 0;
      XFreePixmap(Dpy, XPix);
      XPix = 0;
      RaiseError("could not allocate Pixmap");
      return 1;
    }
  }

  Ctx = glXCreateContext(Dpy, visP, NULL, False);
  if (Ctx == NULL) {
    destroyPixmap();
    RaiseError("could not create rendering context");
    return 1;
  }

  OldCtx = glXGetCurrentContext();
  (void) glXMakeCurrent(Dpy, GPix, Ctx);

#endif
  return 0;
}

#ifdef STDC_HEADERS
extern void build_projection(char, int, int);
#else
extern void build_projection();
#endif

Int
gl_dump_window()
{
  char *fname;
  Int w, h, i;
  if(isatom(a1)) fname=STR_FROM_STRLSP(ADRPNAME(a1));
  else if(isstr(a1)) fname=STR_FROM_STRLSP(a1);
  else {a1 = nil; derec;}

  if(!isnum(a2) || !isnum(a3)) {a1 = nil; derec;}
  
  if(!Start_Dump(fname, 0, fvalue(a2), fvalue(a3), (Widget) wobj->w)) {
    /* redrawing the scene */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(wobj->Proj_Mat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(wobj->Model_Mat);
    glCallList(wobj->root_list);
    glFlush();
    if(!EndDump((Widget)wobj->w)) a1 = t;
    else a1 = nil;
  }
  else a1 = nil;
  derec;
}

/* ******************************************* */
/* deuxième partie : lecture d'un fichier tiff */
/* ******************************************* */

/* Maintenant la définition pour lire un tiff vers un buffer, 
   le buffer retourné est à libérer en utilisant XtFree
 */
static TK_RGBImageRec r_image;

TK_RGBImageRec *
#ifdef STDC_HEADERS
LoadTiffImage(char *f_name)
#else
LoadTiffImage(f_name)
char *f_name;
#endif
{
  TIFF *tif = TIFFOpen(f_name, "r");
  r_image.data = NULL;
  if(tif) {
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &r_image.sizeX);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &r_image.sizeY);
    if((r_image.data = alloc_image_data(r_image.sizeX, r_image.sizeY)) != NULL) {
      if (!TIFFReadRGBAImage(tif, (uint32)r_image.sizeX, (uint32)r_image.sizeY, (uint32 *)&r_image.data, 0)) { 
	XtFree((char *)r_image.data); 
	r_image.data = NULL;
      }
    }
    TIFFClose(tif);
  }
  return (TK_RGBImageRec *)&r_image;
}

/* copy une image depuis un ficher tiff vers une position donnée 
   (GLloadTiff X Y Z FileName [ZoomX [ZoomY]]) 
*/
Int
gl_read_tiff()
{
  short pos[3];
  char *f_name;
  TK_RGBImageRec *image;
  float zx = 0.0, zy = 0.0;

  if(!getNumArgFroma1((char *) pos, 3, GLXBVL_SHORT) || !isstr(car(a1))) { a1 = nil; derec; }
  f_name = STR_FROM_STRLSP(car(a1));
  image = LoadTiffImage(f_name);
  if(!image->data) { a1 = nil; derec; }
  a1 = cdr(a1);
  if(isnum(car(a1))) {
    zx = fvalue(car(a1));
    a1 = cdr(a1);
    if(isnum(car(a1))) zy = fvalue(car(a1));
    else zy = zx;
  }
  else zx = zy = 1;
  
  push(zx); push(zy); push(pos[2]); push(pos[1]); push(pos[0]); push(image);  push(1);
  pc = display_image_file;
}

#endif

/* gestion des images RGB en utilisant la librairie GL/Mesatk */

/* lecture et affichage d'une image RGB */
/* (GLloadRGB X Y Z FileName [ZoomX [ZoomY]]) */
Int gl_read_rgb()
{
  short pos[3];
  char *f_name;
  TK_RGBImageRec *image;
  float zx = 0.0, zy = 0.0;

  if(!getNumArgFroma1((char *) pos, 3, GLXBVL_SHORT) || !isstr(car(a1))) { a1 = nil; derec; }
  f_name = STR_FROM_STRLSP(car(a1));
  if(!(image = tkRGBImageLoad(f_name))) { a1 = nil; derec; }
  a1 = cdr(a1);
  if(isnum(car(a1))) {
    zx = fvalue(car(a1));
    a1 = cdr(a1);
    if(isnum(car(a1))) zy = fvalue(car(a1));
    else zy = zx;
  }
  else zx = zy = 1;

  push(zx); push(zy); push(pos[2]); push(pos[1]); push(pos[0]); push(image); push(0);
  pc = display_image_file;
}


#endif
