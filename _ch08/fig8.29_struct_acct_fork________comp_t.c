#include "apue.h"
#include <sys/acct.h>

#ifdef HAS_SA_STAT
#define FMT "%-*.*s  e = %6ld, chars = %7ld, stat = %3u: %c %c %c %c\n"
#else
#define FMT "%-*.*s  e = %6ld, chars = %7ld, %c %c %c %c\n"
#endif
#ifndef HAS_ACORE
#define ACORE 0
#endif
#ifndef HAS_AXSIG
#define AXSIG 0
#endif

static unsigned long
compt2ulong(comp_t comptime)	/* convert comp_t to unsigned long */
{
	unsigned long	val;
	int				exp;

	val = comptime & 0x1fff;	/* 13-bit fraction */
	exp = (comptime >> 13) & 7;	/* 3-bit exponent (0-7) */
	while (exp-- > 0)
		val *= 8;
	return(val);
}


//#define ACCT_COMM 16

//  typedef u_int16_t comp_t;
//  
//  struct acct {
//  char ac_flag; /* 记账标记 */
//  u_int16_t ac_uid; /* 记账用户 ID */
//  u_int16_t ac_gid; /* 记账组 ID */
//  u_int16_t ac_tty; /* 控制终端 */
//  u_int32_t ac_btime; /* 进程创建时间
//  (从开机起的秒数) */
//  comp_t ac_utime; /* 用户 CPU 时间 */
//  comp_t ac_stime; /* 系统 CPU 时间 */
//  comp_t ac_etime; /* 流失的时间 */
//  comp_t ac_mem; /* 平均内存用量 (kB) */
//  comp_t ac_io; /* Characters transferred (未使用) */
//  comp_t ac_rw; /* 读写的块 (未使用) */
//  comp_t ac_minflt; /* Minor page faults */
//  comp_t ac_majflt; /* Major page faults */
//  comp_t ac_swaps; /* Number of swaps (unused) */
//  u_int32_t ac_exitcode; /* 进程终止状态
//  (参看 wait(2)) */
//  char ac_comm[ACCT_COMM+1];
//  /* 命令名 (执行文件名；以0结尾) */
//  char ac_pad[X]; /* 填充字节 */
//  };
//  
//  enum { /* 在 ac_flag 域可以设置的位 */
//  AFORK = 0x01, /* 调用 fork 之后，但还没有调用 exec */
//  ASU = 0x02, /* 使用超级用户权限 */
//  ACORE = 0x08, /* 核心转储 */
//  AXSIG = 0x10 /* 由信号杀死 */
// };

int
main(int argc, char *argv[])
{

	// 进程数据统计  
	struct acct		acdata;
	FILE			*fp;

	if (argc != 2)
		err_quit("usage: pracct filename");
	if ((fp = fopen(argv[1], "r")) == NULL)
		err_sys("can't open %s", argv[1]);
	while (fread(&acdata, sizeof(acdata), 1, fp) == 1) {
		printf(FMT, (int)sizeof(acdata.ac_comm),
			(int)sizeof(acdata.ac_comm), acdata.ac_comm,
			compt2ulong(acdata.ac_etime), compt2ulong(acdata.ac_io),
#ifdef HAS_SA_STAT
			(unsigned char) acdata.ac_stat,
#endif
			acdata.ac_flag & ACORE ? 'D' : ' ',
			acdata.ac_flag & AXSIG ? 'X' : ' ',
			acdata.ac_flag & AFORK ? 'F' : ' ',
			acdata.ac_flag & ASU   ? 'S' : ' ');
	}
	if (ferror(fp))
		err_sys("read error");
	exit(0);
}
