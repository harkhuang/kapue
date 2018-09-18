#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define err_sys(fmt, args...)  printf(fmt, ##args)
static void
sig_int(int signo)
{
	printf("caught SIGINT\n");
}

static void
sig_chld(int signo)
{
	printf("caught SIGCHLD\n");
}

int
main(void)
{
	if (signal(SIGINT, sig_int) == SIG_ERR) // 捕获进程退出
		err_sys("signal(SIGINT) error");
	if (signal(SIGCHLD, sig_chld) == SIG_ERR) //捕获子进程退出
		err_sys("signal(SIGCHLD) error");
	if (system("/bin/ed") < 0) // 执行一个系统函数
		err_sys("system() error");
	exit(0);
}
