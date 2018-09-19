#include "apue.h"
#include <fcntl.h>

char	buf1[] = "abcdefghij";
char	buf2[] = "ABCDEFGHIJ";

#define	S_RWRH	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// 如何传递pthread的第三个routinu参数 ？如何把函数写成指针？


/*
通过指针偏移，让整个文件达到共享多进程能够读取和写入
并且空洞文件并不占用空间
在提升效率的同时最大的节省了存储的磁盘空间
*/

typedef void (*ptrFunc)(void );
void routinu1() {
	printf("this is pthrad1\n");
}

int
main(void)
{
	int		fd;

	if ((fd = creat("file.hole", S_RWRH)) < 0)
		err_sys("creat error");

	if (write(fd, buf1, 10) != 10)
		err_sys("buf1 write error");
	/* offset now = 10 */
	// 注意if中间的写法
	// if 处理容错同时  得到一些后文可用的返回值是一个不错的idea
	if (lseek(fd, 16384, SEEK_SET) == -1)
		err_sys("lseek error");
	/* offset now = 16384 */

	if (write(fd, buf2, 10) != 10)
		err_sys("buf2 write error");
	/* offset now = 16394 */

	// pid_t pid1,pid2;

	// ptrFunc mycall_back = routinu1;
	
	// mycall_back();
	// //pthread_create(pid1, NULL, &routinu1, NULL);
	// pthread_create(&pid1, NULL, &mycall_back, NULL);
	// pthread_create(&pid2, NULL, &mycall_back, NULL);
	exit(0);
}
