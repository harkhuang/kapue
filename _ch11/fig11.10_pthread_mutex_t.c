#include <stdlib.h>
#include <pthread.h>


// foo 结构存储计数器  和  一个锁
struct foo {
	int             f_count;// 计数器
	pthread_mutex_t f_lock;//锁  互斥锁(读写锁)
	/* ... more stuff here ... */
};

struct foo *
foo_alloc(void) /* allocate the object */
{
	struct foo *fp;
	// 为数据结构分配内存空间  这个结构数据是共享给多个进程的
	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;// 初始化结构数据
		// 初始化锁属性  默认属性  NULL属性我认为就是一个互斥锁
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return(NULL);
		}
		/* ... continue initialization ... */
	}
	return(fp);
}


// 着是个线程回到  独享共享部分内存数据
void
foo_hold(struct foo *fp) /* add a reference to the object */
{
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}



// 释放锁资源
void
foo_rele(struct foo *fp) /* release a reference to the object */
{
	// 获取锁
	pthread_mutex_lock(&fp->f_lock);

	// 确认没有人使用
	if (--fp->f_count == 0) { /* last reference */
		// 解锁
		pthread_mutex_unlock(&fp->f_lock);
		// 毁锁
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_mutex_unlock(&fp->f_lock);
	}
}
