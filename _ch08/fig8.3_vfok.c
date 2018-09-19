#include "apue.h"

// vfork 保证子进程先运行，如果在子进程中等待父进程的状态 或者一个信号 那么会形成死锁
int		glob = 6;		/* external variable in initialized data */

int
main(void)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;
	printf("before vfork\n");	/* we don't flush stdio */
	if ((pid = vfork()) < 0) {
		err_sys("vfork error");
	} else if (pid == 0) {		/* child */
		glob++;					/* modify parent's variables */
		var++;
		_exit(0);				/* child terminates */
	}
	else{
		printf("i am father\n");
		sleep(1);
	}
	/*
	 * Parent continues here.
	 */
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);
}
