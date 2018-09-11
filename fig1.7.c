#include "apue.h"
#include <sys/wait.h>
#include <sysexits.h>
int
main(void)
{
	char	buf[MAXLINE];	/* from apue.h */
	pid_t	pid;
	int		status;

	printf("%% ");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
		   buf[strlen(buf) - 1] = 0; /* replace newline with null */

		// 此处出错排查很久  因为  1&0仍然是真  程序输出forkerror
		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) {		/* child */
			execlp(buf, buf, (char *)0);
			err_ret("couldn't execute: %s", buf);
			exit(127); // 127自定义的一个返回值
		}
		
		
			if ((pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");
		printf("%% ");

	}
	exit(0);
}