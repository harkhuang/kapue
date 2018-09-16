#include "apue.h"

#define	TOK_ADD	   5

void	do_line(char *);
void	cmd_add(void);
int		get_token(void);

char	*tok_ptr;		/* global pointer for get_token() */

// 此程序描述了一般程序的层层调用
// 问题：压栈多层厚  函数跳转并不灵活  而且效率低下 多次的弹压栈操作
// 解决办法  setjmp  longjmp 函数解决这个问题，本质上是用了寄存器指针

int
main(void)
{
	char	line[MAXLINE];

	while (fgets(line, MAXLINE, stdin) != NULL)
		do_line(line);
	exit(0);
}


void
do_line(char *ptr)		/* process one line of input */
{
	int		cmd;

	tok_ptr = ptr;
	while ((cmd = get_token()) > 0) {
		switch (cmd) {	/* one case for each command */
		case TOK_ADD:
				cmd_add();
				break;
		}
	}
}

void
cmd_add(void)
{
	int		token;

	token = get_token();
	/* rest of processing for this command */
}

int
get_token(void)
{
	/* fetch next token from line pointed to by tok_ptr */
}
