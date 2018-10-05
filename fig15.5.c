#include "apue.h"


// 通过管道进程间通信

//此处管道是双端的  可以从一遍读一边写
// 管道这个名字形象 


// pipe(fd[2]) 生成两个管道   
// 思考 本来2个文件描述符   怎么就一个读一个写的双端呢???
// fd[0] 是读还是写?
// fd[1] 是读还是写?
 

int
main(void)
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	line[MAXLINE];

	if (pipe(fd) < 0)
		err_sys("pipe error");
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid > 0) {		/* parent */
		close(fd[0]);
		write(fd[1], "hello world\n", 12);
	} else {				/* child */
		close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(STDOUT_FILENO, line, n);
	}
	exit(0);
}
