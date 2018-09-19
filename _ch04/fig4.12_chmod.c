#include "apue.h"



// chmod 改变文件的权限


// chown 改变用户组  所属组  所属用户
// 派生函数 fchown   根据文件描述符改变
// 派生函数  lchown 根据链接改变
// truncate  截断文件
// ftruncat 根据文件描述符截断     

int
main(void)
{
	struct stat		statbuf;

	/* turn on set-group-ID and turn off group-execute */

	if (stat("foo", &statbuf) < 0)
		err_sys("stat error for foo");
	if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		err_sys("chmod error for foo");

	/* set absolute mode to "rw-r--r--" */

	if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
		err_sys("chmod error for bar");

	exit(0);
}
