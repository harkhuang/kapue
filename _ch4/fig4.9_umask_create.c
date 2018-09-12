#include "apue.h"
#include <fcntl.h>


// 分别设置当前用户全县  用户组权限   其他用户权限  S_IROTH表示其他用户全县
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)


// 内核定义权限分了4种类型
// 1 当前用户 2 用户组  3其他用户  4也是最重要的  就是进程的权限，这里的进程指的是所有进程吗？

// 测试了下touch的文件当前用户有读写权限 没有执行权限  其他和组用户有读权限
/*
hk-mac:kapue hk$ ls  -l foo bar
-rw-------  1 hk  staff  0  9 12 15:18 bar
-rw-rw-rw-  1 hk  staff  0  9 12 15:18 foo
hk-mac:kapue hk$ touch touch1
hk-mac:kapue hk$ ls -l foo bar touch1
-rw-------  1 hk  staff  0  9 12 15:18 bar
-rw-rw-rw-  1 hk  staff  0  9 12 15:18 foo
-rw-r--r--  1 hk  staff  0  9 12 15:27 touch1
*/


//so S_IRWXU 当前用户读写执行权限

// umasek 用来设置进程创建文件的权限默认权限
// 当然我们可以通过umask初始化权限及其集合  然后创建任意我们需要的权限的文件
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
