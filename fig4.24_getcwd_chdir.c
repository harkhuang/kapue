#include "apue.h"



#define SUSV3   20012L
#define PATH_max 1024



#ifdef  PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif 
static int posix_version = 0;
#define PATH_MAX_GUESS 1024


// 根据不同版本号的系统  动态申请文件名字的长度

char *path_alloc(int *sizep)
{
    char    *ptr;
    int size;

    if (posix_version == 0)
        posix_version = sysconf(_SC_VERSION);

    if(pathmax == 0) {
        errno = 0;
        if((pathmax = pathconf("/", _PC_PATH_MAX)) < 0){
            if(errno ==0)
                pathmax = PATH_MAX_GUESS;
            else
                err_sys("pathconf error for _PC_PATH_MAX");
        }else{
            pathmax++;
        }
    }
    if (posix_version < SUSV3)
        size = pathmax + 1;
    else
        size = pathmax;

    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error for pathname");

    if (sizep != NULL)
        *sizep = size;
    return (ptr);
}






int
main(void)
{
	char	*ptr;
	int		size;
	
//	if (chdir("/usr/spool/uucppublic") < 0)
	if (chdir("/tmp/") < 0)
		err_sys("chdir failed");



    // 为最长文件名字申请空间  安全的 可移植的 申请一个字符串 来存储文件的名字
	ptr = path_alloc(&size);	/* our own function */

    // get current work dirctory 
    // 获取当前工作空间路径
	if (getcwd(ptr, size) == NULL)
		err_sys("getcwd failed");

	printf("cwd = %s\n", ptr);
	exit(0);
}
