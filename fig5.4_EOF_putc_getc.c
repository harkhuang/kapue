
#include <stdio.h>



// fwide  是标准libc中库函数 不要和 SUS（singnal unix specification）混淆
// 可以通过它来设置字符串的宽度


// 设置缓冲的方式

// setbuf(*fp, *buf)   为指定文件指针设置缓冲区大小
// *buf = null 表示不带缓冲

// setvbuf(*fp,*buf,mode,size)
// mode ->1. _IOFBF  full buffer
//         2. _IOLBF   line buffer
//         3._IONBF    nobuffer



/*行io的api*/
//getc fgetc
//putc  fgetc





int
main(void)
{
	int		c;


	// 设置字符集宽度
	int fw = fwide(stdin, -100);
	printf("%d",fw);

	while ((c = getc(stdin)) != EOF)
	{
		printf("len = %d\n",strlen(stdin));
		if (putc(c, stdout) == EOF)
			printf("output error");
	}
	

	if (ferror(stdin))
		printf("input error");







	
	exit(0);
}
