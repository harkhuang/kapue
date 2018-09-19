#include	<stdio.h>

#define	DATAFILE	"datafile"

FILE *
open_data(void)
{
	FILE	*fp;
	char	databuf[BUFSIZ];  /* setvbuf makes this the stdio buffer */

	if ((fp = fopen(DATAFILE, "r")) == NULL)
		return(NULL);

	// 设置buf为  line buffer （行缓冲）
	if (setvbuf(fp, databuf, _IOLBF, BUFSIZ) != 0)
		return(NULL);
	return(fp);		/* error */
}
