#include <stdio.h>
#include <stdlib.h>





int main()
{
    exit(0);
}



/**
 * 
 * 本章节主要讲内存布局
 * 
 * 布局模型
 * 
 * [正文段]  //error  正文段 在最后  常说的ELF文件 也是正文的一种
 * 
 * 
 * 
 * [环境变量]
 * [代码段]
 *   |
 *   |
 *   👇
 * [栈]
 *  |
 *  |
 *  |
 * [堆]
 *  |
 *  |
 * [xxxx]
 * [未初始化变量]
 * [数据段]
 * [正文]
 * []
*/



/*内存布局查看*/

/**
 * 
 * 
 * hk-mac:~ hk$ size /usr/bin/cc  /bin/sh
 * __TEXT	__DATA	__OBJC	others	    dec	         hex
 * 4096	    4096	0	    4294979584	4294987776	100005000	/usr/bin/cc
 * 532480	57344	0	   4295012352	4295602176	10009b000	/bin/sh

*/



/**
 * 
 * 
 * 
 * 
 * 
hk-mac:~ hk$ size -m /usr/bin/cc  
Segment __PAGEZERO: 4294967296
Segment __TEXT: 4096
	Section __text: 27
	Section __stubs: 6
	Section __stub_helper: 26
	Section __cstring: 6
	Section __unwind_info: 72
	total 137
Segment __DATA: 4096
	Section __nl_symbol_ptr: 16
	Section __la_symbol_ptr: 8
	Section __xcrun_: 4
	total 28
Segment __LINKEDIT: 12288
total 4294987776
*/