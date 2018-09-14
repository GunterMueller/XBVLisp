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

/* définition des données liées à une image comme texture */
Int gl_def_texture()
{
  TK_RGBImageRec *image;
  char is_rgba;
  pop_cast(image, (TK_RGBImageRec *)); pop(is_rgba);
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0,
		    (is_rgba ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, image->data);
  free_image_data(image);
  image->data = NULL;
  a1 = t;
  derec;
}


#ifdef HAVE_LIBTIFF 
#include <tiffio.h>

/* possibilité de définission d'une texture à partir d'un fichier tiff */

extern TK_RGBImageRec *LoadTiffImage(
#ifdef STDC_HEADERS
				     char *f_name
#endif
				     );


/* (GLtiffTexImage f_name) */
Int gl_def_tiff_texture()
{
  char *f_name;
  TK_RGBImageRec *image;

  if(!isstr(a1)) { a1 = nil; derec; }
  f_name = STR_FROM_STRLSP(a1);
  if(!(image = LoadTiffImage(f_name)) || !image->data) { a1 = nil; derec; }

  push(1); push(image);
  pc=gl_def_texture;
}
#else /* LIBTIFF */
Int gl_def_tiff_texture()
{
  a1 = nil;
  derec;
}
#endif

/* définition de textures à partir de fichiers rgb */
/* (GLrgbTexImage f_name) */
Int gl_def_rgb_texture()
{
  char *f_name;
  TK_RGBImageRec *image;

  if(!isstr(a1)) { a1 = nil; derec; }
  f_name = STR_FROM_STRLSP(a1);
  if(!(image = tkRGBImageLoad(f_name))) { a1 = nil; derec; }

  push(0); push(image);
  pc=gl_def_texture;
}

/* définition de textures à partir de données (liste de valeurs) */
/* (GLdataTexImage largeur hauteur liste_de_donnée)  SUBR 3 */
Int gl_def_data_texture()
{
  TK_RGBImageRec image;
  
  if(!isnum(a1) || !isnum(a2) || !islist(a3)) { a1 = nil; derec; }
  image.sizeX = valnb(a1);
  image.sizeY = valnb(a2);
  image.data = alloc_image_data(image.sizeX, image.sizeY);
  a1 = a3;
  if(!getNumArgFroma1(image.data, image.sizeX * image.sizeY * 3, GLXBVL_CHAR)) {
    free_image_data(&image);
    a1 = nil; derec;
  }
  push(0); push(&image);
  pc=gl_def_texture;
}


/* autres définitions de la librairie OpenGL à propos des textures */

/* définition des coordonnées : (GLtexCoord s [t])  SUBR2 */
   
Int gl_texture_coord() 
{
  if(!isnum(a1)) { a1 = nil; derec; }
  if(isnum(a2)) glTexCoord2f((float)fvalue(a1), (float)fvalue(a2));
  else          glTexCoord1f((float)fvalue(a1));
  a1 = t;
  derec;
}

/* définition des paramètres des textures */
  
/*  (GLtexEnv "GL_TEXTURE_ENV_MODE"|"GL_TEXTURE_ENV_COLOR" data) SUBR2 */
Int gl_texture_env()
{
  GLenum pname, param;
  float color[4];
  
  if((pname = getGLvalue(STR_FROM_STRLSP(a1))) == -1) { a1 = nil; derec; }
  
  if(isstr(a2)) {
    if((param = getGLvalue(STR_FROM_STRLSP(a2))) == -1) { a1 = nil; derec; }
    glTexEnvi(GL_TEXTURE_ENV, pname, param);
    a1 = t;
  }
  else if(islist(a2) && (a1 = a2) && 
	  getNumArgFroma1(color, 4, GLXBVL_FLOAT)) {
    glTexEnvfv(GL_TEXTURE_ENV, pname, color);
    a1 = t;
  } 
  else a1 = nil;
  derec;
}

/* (GLtexParameter type_de_texture param valeur) SUBR3 */
Int gl_texture_parameter()
{
  GLenum dest, pname, param;
  float color[4];
  
  if((dest = getGLvalue(STR_FROM_STRLSP(a1))) == -1) { a1 = nil; derec; }
  if((pname = getGLvalue(STR_FROM_STRLSP(a2))) == -1) { a1 = nil; derec; }
  
  if(isstr(a3)) {
    if((param = getGLvalue(STR_FROM_STRLSP(a3))) == -1) { a1 = nil; derec; }
    glTexParameteri(dest, pname, param);
    a1 = t;
  }
  else if(islist(a3) && (a1 = a3) && 
	  getNumArgFroma1(color, 4, GLXBVL_FLOAT)) {
    glTexParameterfv(dest, pname, color);
    a1 = t;
  } 
  else a1 = nil;
  derec;
}
  
/* (GLtexGen coord pname param) */
Int gl_texture_gen()
{
  GLenum dest, pname, param;
  float color[4];
  
  if((dest = getGLvalue(STR_FROM_STRLSP(a1))) == -1) { a1 = nil; derec; }
  if((pname = getGLvalue(STR_FROM_STRLSP(a2))) == -1) { a1 = nil; derec; }
  
  if(isstr(a3)) {
    if((param = getGLvalue(STR_FROM_STRLSP(a3))) == -1) { a1 = nil; derec; }
    glTexGeni(dest, pname, param);
    a1 = t;
  }
  else if(islist(a3) && (a1 = a3) && 
	  getNumArgFroma1(color, 4, GLXBVL_FLOAT)) {
    glTexGenfv(dest, pname, color);
    a1 = t;
  } 
  else a1 = nil;
  derec;
}

