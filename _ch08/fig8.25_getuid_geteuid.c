#include "apue.h"


/**
[root@localhost _ch8]# gcc fig8.25_getuid_geteuid.c
[root@localhost _ch8]# ./a.out
real uid = 0, effective uid = 0
 */
int
main(void)
{
	printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());
	exit(0);
}
