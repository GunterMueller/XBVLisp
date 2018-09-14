#include <stdio.h>

extern int	hLoaded;

/*
 * check_loaded
 *
 * 	Checks and prints out a message if the font isn't loaded.
 */
int
check_loaded(who)
	char	*who;
{
	if (!hLoaded) {
		fprintf(stderr, "%s: no hershey font loaded.\n", who);
		return 0;
	}
	return 1;
}
