#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef	OPEN_MAX
static long	openmax = OPEN_MAX;
#else
static long	openmax = 0;
#endif
/*
根据sysconf函数获取系统中对于资源的限制

首先知道资源可以调整  但是默认情况下  它的性能不用担心
*/


/*
 * If OPEN_MAX is indeterminate, we're not
 * guaranteed that this is adequate.
 */
#define	OPEN_MAX_GUESS	256

long
open_max(void)
{
	if (openmax == 0) {		/* first time through */
		errno = 0;
		if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
			if (errno == 0)
			{
				openmax = OPEN_MAX_GUESS;	/* it's indeterminate */
				
			}
				
			else
				err_sys("sysconf error for _SC_OPEN_MAX");
		}
	}

	return(openmax);
}

int main(void)
{
	printf("%ld \n", open_max());
	exit(0);
}