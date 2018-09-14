#include "apue.h"

/**
 *技巧型  通过无名临时文件 输出文件描述符   并且输入到临时文件  虽然我们不知道临时文件的名字
 *  
 * 临时文件技术
 * 临时文件创建  其实删除了 利用tmpnam产生一个唯一的路径
 * 然后创建一个文件 
 * 然后unlink
 */
int
main(int argc, char *argv[])
{
	if (argc != 3)
		err_quit("usage: a.out <directory> <prefix>");

	// tempnam 打开临时文件 返回文件操作io流操作
	printf("%s\n", tempnam(argv[1][0] != ' ' ? argv[1] : NULL,
	  argv[2][0] != ' ' ? argv[2] : NULL));


	
	getchar();
	exit(0);
  }
