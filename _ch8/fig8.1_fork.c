#include "apue.h"

int		glob = 6;		/* external variable in initialized data */
char	buf[] = "a write to stdout\n";

int
main(void)
{
	// 父子共享
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;

	// 写入同时验证返回值是否等于我们写入的内容长度
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		err_sys("write error");
	printf("before fork\n");	/* we don't flush stdout */


	// 返回值小于0表示fork失败
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	// 子进程控制
	} else if (pid == 0) {		/* child */

	// 此处内容子进程独享
		glob++;					/* modify variables */
		var++;
		sleep(2);
	} else {
		// 此处内容父进程独享
		sleep(1);				/* parent */
	}


	// 父子共享
	// 此处仍然属于父进程only?  不是的 父子进程共享才对
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

	// 父子进程推出原理和操作
	// 父子进程都操作此段  那么都会推出 这里让父亲睡一会儿 防止孤儿进程产生
	exit(0);
}


/**
 * 
hk-mac:_ch8 hk$ ./a.out
a write to stdout
before fork
pid = 5784, glob = 7, var = 89
pid = 5783, glob = 6, var = 88
 * 
 * 
 * 
 * 
 * 
 * */