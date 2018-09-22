#include "apue.h"  
#include <pthread.h>  
  
pthread_t ntid;//线程ID  
  
void printids(const char *s)  
{  
        pid_t pid;  
        pthread_t tid;  
        pid = getpid();  
        tid = pthread_self();  
        printf("%s pid %u tid %u (0x%x)\n",s,(unsigned int)pid,  
                        (unsigned int)tid,(unsigned int)tid);  
}  


// 这里的两个存在竞争条件？
void *thrfun(void *arg){  
       // sleep(1);//使得主线程先退出  
        printids("new thread");  
  
        return ((void *)0);  
}  
  
int main(){  
        int err;  
        err = pthread_create(&ntid,NULL,thrfun,NULL);  
  
        if(err != 0)  
                err_quit("can't create thread: %s\n",strerror(err));
        printids("main thread");  
  
        sleep(1);  
  
        exit(0);//注意是进程（不是线程）退出  
}  

 