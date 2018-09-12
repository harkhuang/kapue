#include "apue.h"
#include <fcntl.h>


// 为文件解除链接 
// 
//定义flags:只写，文件不存在那么就创建，文件长度戳为0
#define FLAGS O_WRONLY | O_CREAT | O_TRUNC


//创建文件的权限，用户读、写、执行、组读、执行、其他用户读、执行
#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH
 

 // open 

int
main(void)
{
	// 权限分了两种用户权限     进程打开后的读写权限
	if (open("tempfile", FLAGS |MODE) < 0)
		err_sys("open error");
	if (unlink("tempfile") < 0)
		err_sys("unlink error");
	printf("file unlinked\n");
	sleep(15);



	// 同样我们可以创建一个文件描述符的链接
	if(link("a.out","tempfile"))
		err_sys("link failed");
	printf("link done");
	printf("done\n");
	exit(0);
}
