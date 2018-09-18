#include "apue.h"

static void charatatime(char *);

int
main(void)
{
	pid_t	pid;

	// tell wait do what ?
	// how to fix the sync after fork?

	TELL_WAIT();//设置新的信号掩码 user1  user2信号

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		WAIT_PARENT();		/* parent goes first */
		charatatime("output from child\n");
	} else {
		charatatime("output from parent\n");
		// 向子进程发送user1信号
		TELL_CHILD(pid);
	}
	exit(0);
}

static void
charatatime(char *str)
{
	char	*ptr;
	int		c;

	setbuf(stdout, NULL);			/* set unbuffered */
	for (ptr = str; (c = *ptr++) != 0; )
		putc(c, stdout);
}
