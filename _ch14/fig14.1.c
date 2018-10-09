#include "apue.h"
#include <errno.h>
#include <fcntl.h>

// 读取大文件非阻塞io的报错,系统调用已经处理  我们不关注

// 理解 :这里设置了非阻塞io去读取大文件,因为读取的buf长度较大,存在一个时间窗口,
// 非阻塞io的轮训调用很快,不待read返回下一次的调用又被启动,所以会有错误出现
// 不过内核调用已经为我们处理好了这些问题


// 

/*
#define	EAGAIN		35		/* Resource temporarily unavailable 
*/
void
set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
	int		val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		err_sys("fcntl F_GETFL error");

	val |= flags;		/* turn on flags */

	if (fcntl(fd, F_SETFL, val) < 0)
		err_sys("fcntl F_SETFL error");
}



void
clr_fl(int fd, int flags)
				/* flags are the file status flags to turn off */
{
	int		val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		err_sys("fcntl F_GETFL error");

	val &= ~flags;		/* turn flags off */

	if (fcntl(fd, F_SETFL, val) < 0)
		err_sys("fcntl F_SETFL error");
}

char	buf[500000];

int
main(void)
{
	int		ntowrite, nwrite;
	char	*ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);

	set_fl(STDOUT_FILENO, O_NONBLOCK);	/* set nonblocking */

	ptr = buf;
	while (ntowrite > 0) {
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		// 每次调用  我们都输出错误的调用
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

		if (nwrite > 0) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}

	clr_fl(STDOUT_FILENO, O_NONBLOCK);	/* clear nonblocking */

	exit(0);
}
