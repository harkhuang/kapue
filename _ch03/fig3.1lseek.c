#include "apue.h"



/*
stdin_fileno  缓冲的大小是多少？
*/
int
main(void)
{
	if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
		printf("cannot seek\n");
	else
		printf("seek OK\n");
	exit(0);
}
