#include "apue.h"

/**
 * tmpnam  创建临时文件 
 * 获取临时文件路径
 * rewind文件指针  从头读取数据
 * fget按行读取，虽然是读取1024且在同一行的数据
 */
int
main(void)
{
	char	name[L_tmpnam], line[MAXLINE];
	FILE	*fp;

	// 创建临时文件  不获取它的名字
	printf("%s\n", tmpnam(NULL));		/* first temp name */

	// 创建临时文件  获取它的名字
	tmpnam(name);						/* second temp name */
	printf("%s\n", name);

	// 容错创建临时文件
	if ((fp = tmpfile()) == NULL)		/* create temp file */
		err_sys("tmpfile error");
	fputs("one line of output\n", fp);	/* write to temp file */

	fputs("xxx\n", fp);
 
	// 重定位文件指针
	rewind(fp);							/* then read it back */
	if (fgets(line, sizeof(line), fp) == NULL)
		err_sys("fgets error");
	fputs(line, stdout);				/* print the line we wrote */

	if (fgets(line, sizeof(line), fp) == NULL)
		err_sys("fgets error");
	fputs(line, stdout);				/* print the line we wrote */

	exit(0);
}
