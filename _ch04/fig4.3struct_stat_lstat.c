#include "apue.h"

int
main(int argc, char *argv[])
{
	int			i;
	// 文件类型描述符  
	struct stat	buf;
	char		*ptr;

 
	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		
		// 此处用lstat不仅可以返回 真实文件信息  符号信息也可以显示 
		// 例如：/dev/stdout: symbolic link   link符
		// stat 是该符号引用文件的信息
		// 例如 ：/dev/stdout: character special   字符串描述符
		if (stat(argv[i], &buf) < 0) {
			err_ret("lstat error");
			continue;
		}
		if (S_ISREG(buf.st_mode))
			ptr = "regular";
		else if (S_ISDIR(buf.st_mode)) ///* directory */
			ptr = "directory";
		else if (S_ISCHR(buf.st_mode))
			ptr = "character special";
		else if (S_ISBLK(buf.st_mode)) // /* block special */
			ptr = "block special";
		else if (S_ISFIFO(buf.st_mode))/* fifo or socket */
			ptr = "fifo";
		else if (S_ISLNK(buf.st_mode)) /* symbolic link */
			ptr = "symbolic link";
		else if (S_ISSOCK(buf.st_mode)) 	/* socket */
			ptr = "socket";
		else
			ptr = "** unknown mode **";
		printf("%s\n", ptr);
	}
	exit(0);
}
