#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#define   DEBUG_SWITCH 1

#ifdef DEBUG_SWITCH
#define err_sys(fmt, args...)  printf(fmt, ##args)
#else
#define err_sys(fmt, args...) 
#endif

static void	sig_quit(int);

static void
sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		err_sys("can't reset SIGQUIT");
}



/**
 * sigprocmask打开关闭信号阻塞
 * 
 * 
 */
int  testOfSigprocmask(void)
{
	sigset_t	newmask, oldmask, pendmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
		err_sys("can't catch SIGQUIT");

	/*
	 * Block SIGQUIT and save current signal mask.
	 */
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);

	//  sigprocmask 设置进程信号集
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)   // 设置新的信号集为BLOCK   保存旧信号集 
		err_sys("SIG_BLOCK error");


	// 模拟持续消费quit函数
	sleep(5);	/* SIGQUIT here will remain pending */



	// 获取未决信号集
	if (sigpending(&pendmask) < 0)   
		err_sys("sigpending error");



	// 探测信号
	if (sigismember(&pendmask, SIGQUIT))   
		printf("\nSIGQUIT pending\n");

	/*
	 * Reset signal mask which unblocks SIGQUIT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
	printf("SIGQUIT unblocked\n");

	sleep(5);	/* SIGQUIT here will terminate with core file */
	exit(0);
}
