#include "apue.h"
#include <errno.h>


//[root@localhost _ch9]# gcc fig9.11.c
//[root@localhost _ch9]# ./a.out
//parent: pid = 2321, ppid = 2063, pgrp = 2321, tpgrp = 2321
//child: pid = 2322, ppid = 2321, pgrp = 2321, tpgrp = 2321


static void
sig_hup(int signo)
{
	printf("SIGHUP received, pid = %d\n", getpid());
}

static void
pr_ids(char *name)
{
	printf("%s: pid = %d, ppid = %d, pgrp = %d, tpgrp = %d\n",
	    name, getpid(), getppid(), getpgrp(), tcgetpgrp(STDIN_FILENO));
	fflush(stdout);
}

int
main(void)
{
	char	c;
	pid_t	pid;

	pr_ids("parent");
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid > 0) {	/* parent */
		sleep(5);		/* sleep to let child stop itself */
		exit(0);		/* then parent exits */
	} else {			/* child */
		pr_ids("child");
		signal(SIGHUP, sig_hup);	/* establish signal handler */
		kill(getpid(), SIGTSTP);	/* stop ourself */
		pr_ids("child");	/* prints only if we're continued */
		if (read(STDIN_FILENO, &c, 1) != 1)
			printf("read error from controlling TTY, errno = %d\n",
			    errno);
		exit(0);
	}
}
