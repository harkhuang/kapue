#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

extern char **environ;

pthread_mutex_t env_mutex; // 环境变量 临界变量
static pthread_once_t init_done = PTHREAD_ONCE_INIT;// 线程once属性

static void
thread_init(void)
{
	pthread_mutexattr_t attr; // 初始化线程属性参数

	pthread_mutexattr_init(&attr);// 初始化线程属性
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);// 设置线程互斥属性
	pthread_mutex_init(&env_mutex, &attr);// 对临界变量初始化
	pthread_mutexattr_destroy(&attr);// 销毁线程属性
}

int
getenv_r(const char *name, char *buf, int buflen)
{
	int i, len, olen;

	pthread_once(&init_done, thread_init);
	len = strlen(name);
	pthread_mutex_lock(&env_mutex);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) &&
		  (environ[i][len] == '=')) {
			olen = strlen(&environ[i][len+1]);
			if (olen >= buflen) {
				pthread_mutex_unlock(&env_mutex);
				return(ENOSPC);
			}
			strcpy(buf, &environ[i][len+1]);
			pthread_mutex_unlock(&env_mutex);
			return(0);
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return(ENOENT);
}
