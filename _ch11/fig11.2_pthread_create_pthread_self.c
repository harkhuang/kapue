#include "apue.h"
#include <pthread.h>

pthread_t ntid;

void
printids(const char *s) // 此函数负责打印线程相关数据
{
	pid_t		pid;
	pthread_t	tid;

	pid = getpid();
	tid = pthread_self(); // 获取线程自己的id  与进程getpid不同
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
	  (unsigned int)tid, (unsigned int)tid);
}

void *
thr_fn(void *arg)
{
	printids("new thread: ");
	return((void *)0);
}

int
main(void)
{
	int		err;

	// 此处没有用参数传递的方式 创建新的进程  
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (err != 0)
		err_quit("can't create thread: %s\n", strerror(err));
	printids("main thread:");
	sleep(1);
	exit(0);
}
