#include	<stdio.h>
#include 	<stdlib.h>


/** 内核启动和终止是一个非常复杂的过程
 * exit  内核实际调用的函数
 * _exit  （posix） 和_Exit （ISO c）立即进入内核
 */



void ptf(){
	printf("ptf\n");
}

void ptf1(){
	printf("ptf1\n");
}
int main()
{
	printf("hello, world\n");



	// 函数推出做压栈操作   最多可以写32个哦
	atexit(ptf);
	atexit(ptf1);
	exit(10); // 清理标准io库操作后推出内核  等价于return （0）；
}

// atexit调用登记这些函数 
// 一个进程可以登记32个函数
