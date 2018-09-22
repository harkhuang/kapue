#include "apue.h"
#include <pthread.h>

pthread_t ntid;


// 初步认识线程函数  
// pthread_create的使用   创建线程首先我们要知道线程的id号码

// 考虑下主线程和新的线程之间的竞争关系
// 如果主线程执行到退出 子进程还没有完成调度 
// 这样的结果不是我们期待的
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


	// 注意参数传递  这里的强制类型转换
	printf("%d",*((int *)(arg)));
	return((void *)0);
}


void func(){
	
	// pid_t		pid;
	// pthread_t	tid;

	// pid = getpid();
	// tid = pthread_self(); // 获取线程自己的id  与进程getpid不同
	// printf("%s pid %u tid %u (0x%x)\n", "func", (unsigned int)pid,
	//   (unsigned int)tid, (unsigned int)tid);
	printf("\nxxxxxxxxxxxxxxxxxxsadasdasd\n");
}
int
main(void)
{
	int		err;

	// 此处没有用参数传递的方式 创建新的进程  
	// 我们获取到了线程号码
	// 创建线程 我们输入了线程参数和 一个线程函数
	// 线程在自己的内部获取自己id

	for(int i=0;i<1000000;i++)
	{	

	}
	int a = 11;
	err = pthread_create(&ntid, NULL, thr_fn, (&a));
	if (err != 0)
		err_quit("can't create thread: %s\n", strerror(err));
	
	
	// ！！请理解此处的主线程  
	// 既然有了主线程的概念 那么此处和新创建的线程之间就存在一个竞争关系
	// 我们想办法解决这个问题   到底谁慢?  该怎么解决
	// 作者在主进程后面加了sleep
	printids("main thread:");
	func();
	
	// 避免主进程和子进程之间的竞争  我们在主进程中睡眠等待子进程执行完毕
	// 然后在推出函数
	sleep(1);

	exit(0);
}
