/*
 *	aide	Patrick Greussay, 15 Novembre 1981
 *		revise 14 Septembre 1982
 */
#include <stdio.h>
#include "../include/vlisp_files.h"
#ifdef HAVE_INDEX
#include <strings.h>
#else
#include <string.h>
#define index strchr
#endif

int i,j,k;
struct{long pos; char ligne[120];}t;
int nbs;
int fd; FILE *f;
char *l;
char buf[120],*p,bufcom[120],*p2;

main(argc,argv) char **argv;{

	if((fd=open(DFILE,0))<0)
	{
	  printf("\007\nFile %s inexistante. J'arrete.\n",DFILE);
	  exit(1);
	}
	read(fd,&nbs,sizeof(int));
	if(argc==1)
	{
	  printf("Vous etes manifestement dans l'embarras,\n\
il faut dire:   aidbvlisp sujet	(exemple: aidbvlisp user-doc)\n\
Les sujets disponibles sont:\n");
	  k=6;
	  while(read(fd,&t,sizeof(t)) > 0)
	  {
	    *(index(t.ligne,'\n'))='\0';
	    printf("%12s",t.ligne);
	    if(!--k){k=6; printf("\n");}
	  }
	  putchar('\n');
	  exit(0);
	}
	while(read(fd,&t,sizeof(t)) > 0)
	{
	  *(index(t.ligne,'\n'))='\0';
	  if(strcmp(t.ligne,argv[1]) == 0)goto e1;
	}
	printf("Ya pas encore d'aide a ce sujet.\n"); exit(0);
  e1:
	close(fd);
	if((f=fopen(INFILE,"r")) ==NULL)
	{
	  printf("\007\nFile %s inexistante. J'arrete.\n",INFILE);
	  exit(1);
	}
	fseek(f,t.pos,0);
	for(;;)
	{
	  l=fgets(buf,120,f);
	  if(l==NULL || buf[0]=='^')exit(0);
	  p=buf;
	  while(*p != '\n')
	  {
	    if(*p == '(' && *(p+1) == '|')
	    {
	      p += 2;
	      p2 = bufcom;
	      for(;;){*p2++ = *p++; if(*p == '|' && *(p+1) == ')')break;}
	      p +=2; *p2 = 0;
	      system(bufcom);
	    }
	    else putchar(*p++);
	  }
	  putchar(*p);
	}
}






