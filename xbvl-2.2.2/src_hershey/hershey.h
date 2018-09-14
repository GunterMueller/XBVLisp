/*
 * Prototypage des fonctions de la librairie libhershey.a
 */

#ifdef STDC_HEADERS

extern void hfont(char *),
            hsetpath(char *),
            htextsize(int, int),
            hboxfit(double,double,int),
            htextang(double),
            hfixedwidth(Int),
            hcentertext(Int),
            hrightjustify(Int),
            hleftjustify(Int),
            hdrawchar(char),
            hcharstr(char *),
            hboxtext(double,double,double,double,char *);
            
            
extern double hgetcharsize(char, double *, double *);
             
#else

extern void
  hfont(),
  hsetpath(),
  htextsize(),
  hboxfit(),
  htextang(),
  hfixedwidth(),
  hcentertext(),
  hrightjustify(),
  hleftjustify(),
  hdrawchar(),
  hcharstr(),
  hboxtext(),
            
            
extern float 
  hgetcharsize();

#endif
             

            
