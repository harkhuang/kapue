#include "apue.h"
#include <pthread.h>

struct foo {
	int a, b, c, d;
};

void
printfoo(const char *s, const struct foo *fp)
{
	printf(s);
	printf("  structure at 0x%x\n", (unsigned)fp);
	printf("  foo.a = %d\n", fp->a);
	printf("  foo.b = %d\n", fp->b);
	printf("  foo.c = %d\n", fp->c);
	printf("  foo.d = %d\n", fp->d);
}

void *
thr_fn1(void *arg)
{
	struct foo	foo = {1, 2, 3, 4};

	printfoo("thread 1:\n", &foo);

	// 此处的推出 上面的栈资源仍然是被保留的
	pthread_exit((void *)&foo);
}

void *
thr_fn2(void *arg)
{
	printf("thread 2: ID is %d\n", pthread_self());
	pthread_exit((void *)0);
}

int
main(void)
{
	int			err;
	pthread_t	tid1, tid2;
	struct foo	*fp;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0)
		err_quit("can't create thread 1: %s\n", strerror(err));

	// 在没有创建新的线程的情况下 线程栈空间仍然可以被获取,而且是我们期望的值
	err = pthread_join(tid1, (void *)&fp);
	if (err != 0)
		err_quit("can't join with thread 1: %s\n", strerror(err));
	sleep(1);
	printf("parent starting second thread\n");
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
		err_quit("can't create thread 2: %s\n", strerror(err));
	sleep(1);
	// 此处maos访问出错.  但是ubuntu可以正常访问
	printfoo("parent:\n", fp);
	exit(0);
}
