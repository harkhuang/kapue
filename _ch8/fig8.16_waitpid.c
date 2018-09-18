#include "apue.h"
#include <sys/wait.h>

char	*env_init[] = { "USER=hk", "PATH=/tmp", NULL };

int
main(void)
{
	pid_t	pid;

	// pid 是子进程的id号码
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {	/* specify pathname, specify environment */
		if (execle("/bin/echo", "echoall", "myarg1",
				"MY ARG2", (char *)0, env_init) < 0)
			err_sys("execle error");
	}

	// 等待某个进程  后面应该怎么做
	// 为同步而阻塞
	if (waitpid(pid, NULL, 0) < 0)
		err_sys("wait error");

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {	/* specify filename, inherit environment */
		if (execlp("/bin/echo", "echoall", "only 1 arg", (char *)0) < 0)
			err_sys("execlp error");
	}

	exit(0);
}
