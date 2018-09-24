#include "apue.h"
#include <pthread.h>

struct foo {
	int a, b, c, d;
};


// 思考:在线程之间参数传递应该如何控制?
// 参数传递  线程结束后资源会自动释放吗
// 线程资源是共享的  线程即使结束也不释放
// 可以通过传递回来的参数进行 访问者部分内存空间
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
	sleep(5);
	pthread_exit((void *)&foo);
}

void *
thr_fn2(void *arg)
{
	printf("thread 2: ID is %ld\n", pthread_self());
	pthread_exit((void *)0);
}

int
main(void)
{
	int			err;
	pthread_t	tid1, tid2;
	struct foo	*fp;
	printf("-----------开始创建线程1-------------------------\n");
	err = pthread_create(&tid1, NULL, thr_fn1, NULL);// 创建线程1
	if (err != 0)
		err_quit("can't create thread 1: %s\n", strerror(err));


	printf("获取线程2处理完的资源,并且打印输出\n");

	// 线程切换函数
	err = pthread_join(tid1, (void *)&fp);//主线程接管线程1  获取线程1的返回值(资源)
	if (err != 0)
		err_quit("can't join with thread 1: %s\n", strerror(err));
	
	
	printf("主线程-------------\n");

	// sleep(1);
	// //此处为了区分上面的试验让线程休息1秒钟	
	// // 下面的代码使得线程分离 不能继续上面代码部分的 内存空间
	// printf("-----------开始创建线程2-------------------------\n");
	// printf("parent starting second thread\n");
	// err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	// if (err != 0)
	// 	err_quit("can't  thread 2: %s\n", strerror(err));
	// sleep(1);

	err = pthread_join(tid1, (void *)&fp);
	if (err != 0)
		err_quit("can't join with thread 1: %s\n", strerror(err));
	
	printfoo("parent:\n", fp);

 
}
