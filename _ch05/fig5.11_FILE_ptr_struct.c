#include "apue.h"

#if defined(MACOS)
#define _IO_UNBUFFERED	__SNBF
#define _IO_LINE_BUF	__SLBF
#define _IO_file_flags	_flags
#define BUFFERSZ(fp)	(fp)->_bf._size
#else
#define _IO_UNBUFFERED	__SNBF
#define _IO_LINE_BUF	__SLBF
#define _IO_file_flags	_flags
#define BUFFERSZ(fp)	((fp)->_IO_buf_end - (fp)->_IO_buf_base)
#endif



/*
获取一个普通文件缓冲相关信息

*/
void	pr_stdio(const char *, FILE *);

int
main(void)
{
	FILE	*fp;

	fputs("enter any character\n", stdout);
	if (getchar() == EOF)
		err_sys("getchar error");
	fputs("one line to standard error\n", stderr);

	pr_stdio("stdin",  stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);

	if ((fp = fopen("/etc/motd", "r")) == NULL)
		err_sys("fopen error");
	if (getc(fp) == EOF)
		err_sys("getc error");
	pr_stdio("/etc/motd", fp);
	exit(0);
}

void
pr_stdio(const char *name, FILE *fp)
{
	printf("stream = %s, ", name);

	/*
	 * The following is nonportable.
	 */
	if (fp->_IO_file_flags & _IO_UNBUFFERED)
		printf("unbuffered");
	else if (fp->_IO_file_flags & _IO_LINE_BUF)
		printf("line buffered");
	else /* if neither of above */
		printf("fully buffered");
	
	 //(fp->_IO_buf_end - (fp)->_IO_buf_base)
 
	printf(", buffer size = %d\n", fp->_blksize);
}
