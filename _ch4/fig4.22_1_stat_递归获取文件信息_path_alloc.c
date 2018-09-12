#include "apue.h"
#include <dirent.h>
#include <limits.h>
#include <errno.h>

typedef int Myfunc(const char *,const struct stat*,int);

static Myfunc myfunc;
static int myftw(char *,Myfunc *);
static int dopath(Myfunc *);
static char *fullpath;
static int nreg, ndir, nblk, nchr, nfifo, nsdlink, nsock, ntot;
static int posix_version = 0;

#define PATH_MAX_GUESS 1024

#define FTW_F   1   // file other than directory
#define FTW_D   2   // directory
#define FTW_DNR 3   // directory that cant be read
#define FTW_NS  4   // file that we cant stat



#define SUSV3   20012L
#define PATH_max 1024



#ifdef  PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif 




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



// 一个测试函数
static int myftw(char * pathname, Myfunc *func)
{
    int len;
    // 为何此处len是一个没有初始化的值？
    fullpath = path_alloc(&len);
    strncpy(fullpath, pathname, len);
    fullpath[len-1] = 0;

    return(dopath(func));
}

// 递归处理文func件夹 
static int dopath(Myfunc* func)
{
    struct stat     statbuf;
    struct dirent   *dirp;
    DIR             *dp;
    int             ret;
    char            *ptr;

    if(lstat(fullpath, &statbuf) < 0)
        return(func(fullpath, &statbuf, FTW_NS));   // return file we cant stat
    if(S_ISDIR(statbuf.st_mode) == 0)
        return(func(fullpath, &statbuf, FTW_F));    // file other than directory


    if((ret = func(fullpath, &statbuf, FTW_D) )!= 0)
        return(ret);
    
    ptr = fullpath + strlen(fullpath);
    *ptr++ = '/';
    *ptr = 0;

    if((dp = opendir(fullpath)) == NULL)
        return(func(fullpath, &statbuf, FTW_DNR));

    while((dirp = readdir(dp)) != NULL){
        if(strcmp(dirp->d_name, ".") == 0||
           strcmp(dirp->d_name, "..") == 0)
           continue;

        strcpy(ptr, dirp->d_name);

        if((ret = dopath(func)) != 0)
            break;  
    }
    ptr[-1] = 0;

    if(closedir(dp) < 0)
        err_ret("cant close directory %s", fullpath);

    return(ret);
}



// 自定义的调用   根据文件名称   获取文件信息  这里做的只是统计总共的文件数量 无它
static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch(type){
    // 统计除了目录之外的文件信息    
    case FTW_F:
        switch (statptr->st_mode & S_IFMT){
        case S_IFREG: nreg++;       break;
        case S_IFBLK: nblk++;       break;
        case S_IFCHR: nchr++;       break;
        case S_IFIFO: nfifo++;      break;
        case S_IFLNK: nsdlink++;    break;
        case S_IFSOCK: nsock++;     break;
        case S_IFDIR: ndir++;       break;
            err_dump("for S_IFDIR for %s", pathname);
        }
    // 统计目录数量
    case FTW_D:
        ndir++;
        break;
    // 报错1    
    case FTW_DNR:
        err_ret("cant read dirctory %s", pathname);
        break;
    // 报错2
    case FTW_NS:
        err_ret("stat error for %s", pathname);
    
    default:
        err_dump("unkonw type %d for pathname %s", type, pathname);
    }
    return(0);
    
}





// 递归统计文件 总的信息
int
main(int argc, char *argv[])
{
	int		ret;

#if 0 
	if (argc != 2)
		err_quit("usage:  ftw  <starting-pathname>");

	ret = myftw(argv[1], myfunc);		/* does it all */
#else
    ret = myftw("..", myfunc);
#endif
	ntot = nreg + ndir + nblk + nchr + nfifo + nsdlink + nsock;
	if (ntot == 0)
		ntot = 1;		/* avoid divide by 0; print 0 for all counts */
	printf("regular files  = %7ld, %5.2f %%\n", nreg,
	  nreg*100.0/ntot);
	printf("directories    = %7ld, %5.2f %%\n", ndir,
	  ndir*100.0/ntot);
	printf("block special  = %7ld, %5.2f %%\n", nblk,
	  nblk*100.0/ntot);
	printf("char special   = %7ld, %5.2f %%\n", nchr,
	  nchr*100.0/ntot);
	printf("FIFOs          = %7ld, %5.2f %%\n", nfifo,
	  nfifo*100.0/ntot);
	printf("symbolic links = %7ld, %5.2f %%\n", nsdlink,
	  nsdlink*100.0/ntot);
	printf("sockets        = %7ld, %5.2f %%\n", nsock,
	  nsock*100.0/ntot);

	exit(ret);
}