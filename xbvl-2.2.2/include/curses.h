
/* %I%, %R%, %B%, %D%,  %T%,  %M% */
/* 5/15/81 (Berkeley) @(#)curses.h	1.8 */
/* Harald Wertz Fevrier 84 */

/* CE FICHIER N'EST PLUS UTILISE DANS LA VERSION ACTUELLE DE XBVL */
/* L'INTERFACE EST MAINTENANT GEREE PAR X WINDOWS.... */

# include	<stdio.h>
 
# define	TRUE	(1)
# define	FALSE	(0)
# define	ERR	(0)
# define	OK	(1)

char *strchr(), *strrchr();
/* # define memcpy(dst, src, len)	bcopy((src), (dst), (len)) */
/* # define strrchr(s,c)		rindex(s,c) */
/* # define strchr(s,c)			index(s,c) */

# define MAXWIN 25

/* def de fenetre */
struct _win_st {
	struct _win_st *nexwin;
	short	_cury, _curx;	short	_maxy, _maxx;
	short	_begy, _begx;	short	_flags; short _dum;
	char	_clear;	char	_leave;	char	_scroll;char _actif;
	char	**_y;
	short	*_firstch;short	*_lastch;Int 	*_pn;
};

#define	WINDOW	struct _win_st

struct listwin { WINDOW *win; struct listwin *next;
                 struct listwin *sur[MAXWIN];
                 struct listwin *sous[MAXWIN];
};











