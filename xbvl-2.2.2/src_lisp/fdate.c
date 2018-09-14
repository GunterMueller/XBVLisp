/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/*	fdate
 *	Pour avoir la date en francais. Patrick Greussay, Novembre 1981
 *	adaptation a bvlisp, Harald Wertz, Avril 1983
 */

#include <time.h>
#define xwin_h   /* pas de xwin.h */
#include <vlisp.h>


long clock_var;
struct tm *ttt;
struct tm *localtime();
CHAR *jours[]={"Dimanche","Lundi","Mardi","Mercredi",
	       "Jeudi","Vendredi","Samedi"};
CHAR *mois[]={"Janvier","Fevrier","Mars","Avril","Mai",
	      "Juin","Juillet","Aout","Septembre",
	      "Octobre","Novembre","Decembre"};

cpstr(){while(*ppn++ = *pchar++)
	if(ppn>hstr) gcstrsv(); 
	ppn--;}

cpnum(){gsynum();}

Int date(){
	time(&clock_var);
	ttt=localtime(&clock_var);
	newppn();
	
	pchar=jours[ttt->tm_wday];
	cpstr();*ppn++ = ' ';
	na1=ttt->tm_mday;cpnum();*ppn++=' ';
	pchar=mois[ttt->tm_mon];cpstr();*ppn++=' ';
	na1=ttt->tm_year;*ppn++ = '1';*ppn++ = '9';cpnum();*ppn++=' ';
	na1=ttt->tm_hour;cpnum();*ppn++=':';
	na1=ttt->tm_min;cpnum();*ppn++=':';
	na1=ttt->tm_sec;cpnum();
	finstr();
}

Int timepp(){
	time(&clock_var);
	ttt=localtime(&clock_var);
/*	if(a1==nil){
		na1=ttt->tm_sec;
		na2=ttt->tm_min;
		na1=(60 * na1)+na2;
		na2=ttt->tm_hour;
		na1=(60 * na1)+na2;
		a1=cranb(na1);
		derec;}
	else { */
		newppn();
		na1=ttt->tm_year;cpnum();*ppn++=':';
		na1=ttt->tm_mon;na1++;
		if(na1<10) *ppn++='0';
		cpnum();*ppn++=':';
		na1=ttt->tm_mday;
		if(na1<10) *ppn++='0';
		cpnum();*ppn++=':';
		na1=ttt->tm_hour;
		if(na1<10) *ppn++='0';
		cpnum();*ppn++=':';
		na1=ttt->tm_min;
		if(na1<10) *ppn++='0';
		cpnum();*ppn++=':';
		na1=ttt->tm_sec;
		if(na1<10) *ppn++='0';
		cpnum();	
		*ppn++=0;
		finstr(); /* } */
}
