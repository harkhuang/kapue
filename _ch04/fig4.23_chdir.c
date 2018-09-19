#include "apue.h"

// 进程环境变量 工作路径的修改  chdir
int
main(void)
{

	// 改变当前进程工作目录
	if (chdir("/tmp") < 0)
		err_sys("chdir failed");
	printf("chdir to /tmp succeeded\n");
	exit(0);
}
