#include <pthread.h>
#include <stdio.h>

static unsigned long long thread_nr = 0;

pthread_mutex_t mutex_;

void* inc_thread_nr(void* arg) {
    (void*)arg;
    pthread_mutex_lock(&mutex_);
    thread_nr ++;
    pthread_mutex_unlock(&mutex_);

    /* printf("thread_nr = %d\n", thread_nr); */

    sleep(300000);
}

int main(int argc, char *argv[])
{
    int err;
    int cnt = 0;

    pthread_t pid[1000000];

    pthread_mutex_init(&mutex_, NULL);

    while (cnt < 1000000) {
        // 创建失败则跳出循环
        err = pthread_create(&pid[cnt], NULL, (void*)inc_thread_nr, NULL);
        if (err != 0) {
            break;
        }
        cnt++;
    }

    pthread_join(pid[cnt], NULL);

    pthread_mutex_destroy(&mutex_);
    printf("Maximum number of threads per process is = %d\n", thread_nr);
}