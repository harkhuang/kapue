#include "apue.h"
#ifdef SOLARIS
#include <sys/mkdev.h>
#endif


// 两个内核函数mac不能正常调用  我把头文件拷贝了出来
static inline __int32_t  major(__uint32_t _x)
{
	return (__int32_t)(((__uint32_t)_x >> 24) & 0xff);
}
static inline __int32_t  minor(__uint32_t _x)
{
	return (__int32_t)((_x) & 0xffffff);
}



// st_rdev  和  st_dev  主设备 和 副设备的信息获取
// mac 下没有major函数  
int
main(int argc, char *argv[])
{
	int			i;
	struct stat	buf;

	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		if (stat(argv[i], &buf) < 0) {
			err_ret("stat error");
			continue;
		}


		// mac 下没有这个函数 从内核头文件中拷贝出来
		printf("dev = %d/%d", major(buf.st_dev),  minor(buf.st_dev));

		if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
			printf(" (%s) rdev = %d/%d",
					(S_ISCHR(buf.st_mode)) ? "character" : "block",
					major(buf.st_rdev), minor(buf.st_rdev));
		}
		printf("\n");
	}

	exit(0);
}

