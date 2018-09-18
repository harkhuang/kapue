#include "apue.h"
#include <sys/wait.h>


// system函数实际是在子进程中调用一个系统调用 

// 为什么直接完成一个系统调用呢?
// 因为直接完成系统调用 涉及到系统的中断, 单独一个进程并不好控制  
// 一条控制命令的完整的生命周期是有正常的输入  正常的输出才是正常的

int
system(const char *cmdstring)	/* version without signal handling */
{
	pid_t	pid;
	int		status;

	if (cmdstring == NULL)
		return(1);		/* always a command processor with UNIX */

	if ((pid = fork()) < 0) {
		status = -1;	/* probably out of processes */
	} else if (pid == 0) {				/* child */
		execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
		_exit(127);		/* execl error */
	} else {							/* parent */
		while (waitpid(pid, &status, 0) < 0) {
			// 父进程通常等待子进程结束后自己结束
			if (errno != EINTR) {
				status = -1; /* error other than EINTR from waitpid() */
				break;
			}
		}
	}

	return(status);
}

void
pr_exit(int status)
{
	if (WIFEXITED(status))
		printf("normal termination, exit status = %d\n",
				WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n",
				WTERMSIG(status),
#ifdef	WCOREDUMP
				WCOREDUMP(status) ? " (core file generated)" : "");
#else
				"");
#endif
	else if (WIFSTOPPED(status))
		printf("child stopped, signal number = %d\n",
				WSTOPSIG(status));
}

 
int
main(void)
{
	int		status;

	if ((status = system("date")) < 0)
		err_sys("system() error");
	pr_exit(status);

	if ((status = system("nosuchcommand")) < 0)
		err_sys("system() error");
	pr_exit(status);

	if ((status = system("who; exit 44")) < 0)
		err_sys("system() error");
	pr_exit(status);

	exit(0);
}
