#include "apue.h"
#include <fcntl.h>
#include <utime.h>


// utime 一个文件的访问和修改时间可用utime函数更改
// 更改文件的最后访问时间
// 文件变更信息的修改
int
main(int argc, char *argv[])
{
	int				i, fd;
	struct stat		statbuf;
	struct utimbuf	timebuf;

	for (i = 1; i < argc; i++) {
		if (stat(argv[i], &statbuf) < 0) {	/* fetch current times */
			err_ret("%s: stat error", argv[i]);
			continue;
		}
		if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) { /* truncate */
			err_ret("%s: open error", argv[i]);
			continue;
		}
		close(fd);
		timebuf.actime  = statbuf.st_atime; // 文件的最后access（访问）时间
		timebuf.modtime = statbuf.st_mtime;  // 文件的最后modify （修改）时间
		if (utime(argv[i], &timebuf) < 0) {		/* reset times */
			err_ret("%s: utime error", argv[i]);
			continue;
		}
	}
	exit(0);
}
