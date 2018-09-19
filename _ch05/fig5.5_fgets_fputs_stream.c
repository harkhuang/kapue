#include "apue.h"



// 以行缓冲获取内存buffer
int
main(void)
{
	char	buf[MAXLINE];  //这里行缓冲

	while (fgets(buf, MAXLINE, stdin) != NULL)
		if (fputs(buf, stdout) == EOF)
			err_sys("output error");

	if (ferror(stdin))
		err_sys("input error");

	exit(0);
}
