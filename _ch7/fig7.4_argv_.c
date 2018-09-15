#include "apue.h"


//*
/**
 *将所有命令输入标准输出。 
 * 打印命令行输入
 * 
 * 
*/
 
int
main(int argc, char *argv[])
{
	int		i;

	for (i = 0; i < argc; i++)		/* echo all command-line args */
		printf("argv[%d]: %s\n", i, argv[i]);
	exit(0);
}
