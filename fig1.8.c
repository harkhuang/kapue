#include "apue.h"
#include <errno.h>
/*
进程退出异常捕获
错误码：stream 直接获取错误信息
错误信息描述：  通过perror获取错误信息

函数  
errno  进程错误码全局设置要包括errno.h头文件
perror文件  输出错误信息   这个错误信息  是通过错误码获取到的   那么进程的第一个参数 就是进程的返回值？

*/
int
main(int argc,char*argv[])
{
    // 格式化的写入错误信息到stderr中
    // 思考如何捕获这个信息
    // eaccess  错误的访问   访问没有权限
    fprintf(stderr,"EACCES: %s\n", strerror(EACCES));


    //fprintf(stderr,"EPERM: %s\n", strerror(EPERM));
    // 另外一种方式的获取  通过错误码获取错误信息

    // 访问设置
    errno = EACCES;
    perror(argv[0]);


    // 进程的正常退出
    // 此操作会对操作系统发出默认的信号
    // 做了什么我不清楚  但是能做很多事情
    // 正常的操作是0  但是其他可能会有产生core文件 等
    exit(139);
}