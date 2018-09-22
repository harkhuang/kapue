#include "apue.h"
#include <pthread.h>



/**
 * pthread_join(pid,ret)用于等待 pid线程结束  和线程的返回值
 * 如果线程没有结束 pthread_join将处于阻塞block
 * */




void *
thr_fn1(void *arg)
{
	printf("thread 1 returning\n");
	
	return((void *)1);
}

void *
thr_fn2(void *arg)
{
	printf("thread 2 exiting\n");
	sleep(3);
	// 线程退出后的指针 或者我们认为是一个处理结果
	// 此处线程的退出值可以被任何一个线程访问
	// 只要知道这个线程的值的话 我们就可以通过线程id去访问这个线程的退出值
	pthread_exit((void *)2);
}

int
main(void)
{
	int			err;
	pthread_t	tid1, tid2;
	void		*tret;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0)
		err_quit("can't create thread 1: %s\n", strerror(err));
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
		err_quit("can't create thread 2: %s\n", strerror(err));


	// 在这个函数里面我们尝试线程切换的作用
	// 线程切换函数  等待线程1执行完毕  线程2接管线程1返回值

	// 这里一共三个线程  上面的两个线程  和后面的切换线程

	// 此处join的作用一定要理解好  
	// 既然线程切换 那么就要等到线程执行完毕
	// 如果线程1没有退出 那么此处的主线程讲阻塞等待线程执行完毕

	// 此处的处理方式做串行的控制是非常方便的 
	err = pthread_join(tid1, &tret);
	if (err != 0)
		err_quit("can't join with thread 1: %s\n", strerror(err));
	printf("thread 1 exit code %d\n", (int)tret);
	err = pthread_join(tid2, &tret);
	if (err != 0)
		err_quit("can't join with thread 2: %s\n", strerror(err));
	printf("thread 2 exit code %d\n", (int)tret);
	exit(0);
}
