#include "apue.h"

// vfork 保证子进程先运行，如果在子进程中等待父进程的状态 或者一个信号 那么会形成死锁


// 这里我尝试让子进程等待父进程信号 形成死锁
int		glob = 6;		/* external variable in initialized data */


void  sig_callback(){
	printf("i get the signal");
}
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
		printf("child pid = %d \n",getpid());
		// 这行代码永远不能打印出来  因为vfork 先运行子进程（系统机制）  
		// 等待父进程成了无稽之谈

		// 实验失败的原因   singal不能跨进程通信啊  跨进程我们尝试一下 mutex
		signal(SIGALRM,sig_callback);
		// 因为vfork子进程不拷贝后面的函数  38以后的函数，所以不会正常的退出 ，
		// 后面的代码段  父进程独享，只因为执行顺序不同的啊

		// 这个方式却导致了死锁成功  
		pause();
		_exit(0);				/* child terminates */
	}
	else{
		printf("father pid = %d \n",getpid());

		alarm(1);  // 一秒钟后发送时钟信号到子进程 因为是复制的 所以空间应该是共享的
		printf("i am father\n");
		sleep(1);
	}
	/*
	 * Parent continues here.
	 */
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);
}
