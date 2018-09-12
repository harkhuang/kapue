#include "apue.h"
#include <fcntl.h>


// 分别设置当前用户全县  用户组权限   其他用户权限  S_IROTH表示其他用户全县
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)


// 内核定义权限分了4种类型
// 1 当前用户 2 用户组  3其他用户  4也是最重要的  就是进程的权限，这里的进程指的是所有进程吗？



//so S_IRWXU 当前用户读写执行权限

// umasek to set groups access
// 
int
main(void)
{
	// 初始化的用户全县？
	umask(0);
	if (creat("foo", RWRWRW) < 0)
		err_sys("creat error for foo");

	// 由于当前用户肯定是有访问和执行权限的所以只需要设置用户组和其他用户即可
	// 0777
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("bar", RWRWRW) < 0)
		err_sys("creat error for bar");
	exit(0);
}
