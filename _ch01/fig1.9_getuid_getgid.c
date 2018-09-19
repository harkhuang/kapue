#include "apue.h"


/*

getgid()   // 当前组id      
getpid()   // 当前进程id
getuid()   // 会话id   不是会话  而是用户id  用户id用于管理整体操作系统权限问题而定义的
getppid()   //父进程id
*/
int 
main(void)
{
    printf("uuid = %d\n gid = %d\n pid = %d\n ppid = %d\n", getuid(), getgid(), getpid(),getppid());
    exit(0);
}