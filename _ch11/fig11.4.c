#include "apue.h"
#include <pthread.h>

struct foo {
	int a, b, c, d;
};


// 思考:在线程之间参数传递应该如何控制?
// 参数传递  线程结束后资源会自动释放吗
void
printfoo(const char *s, const struct foo *fp)
{
	printf("%s\n",s);
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
	err = pthread_join(tid1, (void *)&fp);
	if (err != 0)
		err_quit("can't join with thread 1: %s\n", strerror(err));
	sleep(1);
	printf("parent starting second thread\n");
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
		err_quit("can't 0[

		] thread 2: %s\n", strerror(err));
	sleep(1);
	printfoo("parent:\n", fp);
	exit(0);
}