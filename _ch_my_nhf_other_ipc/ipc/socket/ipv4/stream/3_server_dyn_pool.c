#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "proto.h"
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>



#define	SERVERPORT	"1988"

#define	FMT_STAMP	"sleep  2 second  and return time Stamp: %ld\r\n"


#define MINSPARESERVERS 5
#define MAXSPARESERVERS 10
#define MAXCLIENTS 20

#define IPSTRSIZE	40
#define LINESIZE	80

#define SIG_NOTIFY	SIGUSR2

enum {
	STATE_IDLE=0,
	STATE_BUSY
};



// 服务器状态器  
struct server_st {
	pid_t pid;
	int state;
	int reuse;
};

typedef struct server_st msg_t;//自定义服务器消息结构体

static struct server_st *serverpool;  //服务器所有子线程共享的服务器状态结构体
static int idle_count=0, busy_count=0;
static int sd;


// 静态的回调函数  不懂为什么要把job定义成static   防止被外部函数调用吗
static void server_job(int sd, int pos);


// 这个函数为什么这么命名? 目的是什么  是不是因为每个资源都会用到
void
usr2_handler(int s)
{
	return;
}



int
add_1_server(void)
{
	int slot;// 卡槽
	pid_t pid;

	// 增加工作任务 逻辑  空闲+使用  > maxclient  容错
	if (idle_count+busy_count >= MAXCLIENTS) {
		return -1;
	}

	// 作用?
	// 初始化pid值  当pid = -1时  表示可以添加工作任务 并且记录该工作节点位置 fd数组中第一个可以使用的
	for (slot=0;slot<MAXCLIENTS;++slot) {
		if (serverpool[slot].pid==-1) {
			break;
		}
	}

	//  找到上  标记该进程空闲?  不是应该标记使用吗???
	serverpool[slot].state = STATE_IDLE;
	pid = fork();  //fork返回值是子进程id
	if (pid<0) {
		return -1;
	}
	if (pid==0) {
		// 在子进程中添加工作序列
		server_job(sd, slot);
		exit(0);
	} else {
		// 对共享内存中的结构体 标志为子进程id
		serverpool[slot].pid = pid;

		// 为什么对空闲的进程做递增操作?
		idle_count++;
	}

	return 0;
}

int
del_1_server(void)
{
	int i;

	// 当idle为0认为没有可以删除的了
	if (idle_count == 0) {
		return -1;
	}

	// 删除一个sever
	for (i=0;i<MAXCLIENTS;++i) {

		// 条件是有着pid而且状态是idle  idle表示在使用状态
		if (serverpool[i].pid !=-1 && serverpool[i].state == STATE_IDLE) {

			// 这里所谓的竞争是不是没有考虑到busy的情况
			// 这里表示谁和谁的竞争呢?
			kill(serverpool[i].pid, SIGTERM); /* FIXME: 可能有竞争BUG */
			serverpool[i].pid = -1;
			idle_count--;
			break;
		}
	}
	return 0;
} 
// 扫描工作组状态   并且更新  根据进程号  确定状态
int
scan_pool(void)
{
	// 任务池检查器
	int i, idle=0, busy=0;

	// 
	for (i=0;i<MAXCLIENTS;++i) {

		// 找到第一个pid为-1的值   如果没有  继续往下走??why
		if (serverpool[i].pid == -1) {
			continue;
		}

		// 干掉没有被使用的进程 ?? 合适使用被释放?? 看看add 和 del
		// 这是一个探测进程是否存在的函数 
		// 冒泡老师解惑 2017年11月30日 17:11:23
		if (kill(serverpool[i].pid, 0)) {  //用于同步 探测到进程情况 更新了进程状态
			serverpool[i].pid = -1;
			continue;
		}

		
		if (serverpool[i].state == STATE_IDLE) {// 空闲进程 计数器
			idle++;
		} else if (serverpool[i].state == STATE_BUSY) {//繁忙进程计数器
			busy++;
		} else {//未知状态机
			fprintf(stderr, "Unknown state.\n");
			abort();
		}
	}
	idle_count=idle; busy_count=busy;
	return 0;
}

// 服务业务处理逻辑
// 这里处理的是真的业务
static void server_job(int sd, int pos)
{
	int client_sd;
	struct sockaddr_in peer_addr;
	socklen_t peer_addr_len;
	char ipstr[IPSTRSIZE];
	time_t stamp;
	char linebuf[LINESIZE];
	int len;
	pid_t ppid;

	ppid = getppid();//获取父进程id

	peer_addr_len = sizeof(peer_addr); //求出peer 地址的长度
	while (1) {
		serverpool[pos].state = STATE_IDLE; //状态机确定了这个卡槽可用


		// SIGUSR2这里定义为通知父进程的一个信号
		kill(ppid, SIG_NOTIFY);// 向父进程发送一个SIG_NOTIFY信号
 
		// 真的链接做业务就这么一行代码 我操! 做了三百行代码的准备 mlgb
		client_sd = accept(sd, (sockaddr *)&peer_addr, &peer_addr_len);
		if (client_sd<0) {
			if (errno==EINTR || errno==EAGAIN) {
				continue;
			}
			perror("accept()");
			exit(1);
		}

		//设置状态为忙碌
		serverpool[pos].state = STATE_BUSY;

		// 发送一个同步信号 
		kill(ppid, SIG_NOTIFY);

		inet_ntop(AF_INET, &peer_addr.sin_addr, ipstr, IPSTRSIZE);
		//printf("[%d]: Client: %s:%d\n", getpid(), ipstr, ntohs(peer_addr.sin_port));
		stamp = time(NULL);

		len = snprintf(linebuf, LINESIZE, FMT_STAMP, stamp);
		/* FIXME: If LINESIZE is not big enough */

		// 好了发送数据到客户端
		send(client_sd, linebuf, len, 0);
		
		// 睡觉五秒钟  模拟业务
		sleep(5);

		close(client_sd);
	}
	close(sd);
}

int
main()
{
	int ret, i;
	struct sockaddr_in myend;
	struct sigaction sa, osa;//sa是我们需要定义和捕捉在当前进程中生效的信号捕捉掩码
	sigset_t set, oset;
	int val;


	// 主要做信号处理  定义我们关心和不关心的信号  和信号的默认操作 
	// 定义信号的回调函数 和信号默认掩码
	sa.sa_handler = SIG_IGN; // 默认处理句柄
	sigemptyset(&sa.sa_mask);// 清空信号集
	sa.sa_flags = SA_NOCLDWAIT;// 设置SA_NOCLDWAIT选项后,当信号为SIGCHILD时,则调用进程的子进程终止,立即释放系统资源。
	sigaction(SIGCHLD, &sa, &osa);// 信号集设置

	sa.sa_handler = usr2_handler; // 默认的信号调用函数
	sigemptyset(&sa.sa_mask);// 清空信号集
	sa.sa_flags = 0;
	sigaction(SIG_NOTIFY, &sa, &osa);

	sigemptyset(&set);
	sigaddset(&set, SIG_NOTIFY);

	// 把新的信号集合替换旧的集合
	sigprocmask(SIG_BLOCK, &set, &oset);


	//  非常重要  这里申请的是一个共享内存  目的是为了 所有服务器线程共享数据
	// 1、将一个普通文件映射到内存中，通常在需要对文件进行频繁读写时使用，这样用内存读写取代I/O读写，以获得较高的性能；
	// 2、将特殊文件进行匿名内存映射，可以为关联进程提供共享内存空间；
	// 3、为无关联的进程提供共享内存空间，一般也是将一个普通文件映射到内存中。
	// -1是初始化的fd值  0表示偏移 
	// 读写权限的fd    参数flags：影响映射区域的各种特性。 
	// MAP_SHARE   对映射区域的写入数据会复制回文件内，而且允许其他映射该文件的进程共享。
	// MAP_ANONYMOUS  建立匿名映射。此时会忽略参数fd，不涉及文件，而且映射区域无法和其他进程共享。
	serverpool = (struct server_st*)mmap(NULL, (sizeof(struct server_st)*MAXCLIENTS), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (serverpool==MAP_FAILED) {  // 申请失败的情况处理 容错
		perror("mmap()");
		exit(1);
	}

	// 初始化共享内存  pid设置为-1
	for (i=0;i<MAXCLIENTS;++i) {
		serverpool[i].pid=-1;
	}


	// 设置套接字基本属性  ipv4  tcp  stream
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if (sd==-1) {
		perror("socket()");
		exit(1);
	}

	// val = 1; //  这个值表示可以重复使用的socket
	int bReuseaddr=1;  //可以重复使用的socket
	//设置socket高级属性  SO_REUSEADDR表示可以重复使用的socket

	/*
		int PASCAL FAR setsockopt ( SOCKET s, //socket标识
		int level, //定义层次  目前支持sol_socket 和 ipproto_tcp
		int optname,  //设置选项
		const char FAR* optval,//指向存放选项值的缓冲区    选项中可能带有参数  所以用此形势表现参数 都可以被传递  这里用到的选项仅仅是是否可以重复使用 所以 没问题true
		int optlen);//缓冲区长度
	*/

	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &bReuseaddr, sizeof(val))<0) {
		perror("setsockopt(..., SO_REUSEADDR, ...)");
	}

	// 设置ipv4
	myend.sin_family = AF_INET;
	myend.sin_port = htons(atoi(SERVERPORT)); // 设置端口号 
	inet_pton(AF_INET, "0.0.0.0", &myend.sin_addr);
	ret=bind(sd, (struct sockaddr*)&myend, sizeof(myend));
	if (ret==-1) {
		perror("bind()");
		exit(1);
	}

	// 监听这个
	ret=listen(sd, 100);
	if (ret==-1) {
		perror("listen()");
		exit(1);
	}


	// 最小剩余 工作组
	for (i=0;i<MINSPARESERVERS;++i) {
		add_1_server();
	}



	// 不停的循环调用控制工作组 进程 这里循环最大次数有限制吗???
	while (1) {


		//设置异步处理信号相关模型   
		//这里主要是控制状态机模型
		//异步操作全靠这小小信号回调的控制
		sigsuspend(&oset);//信号


        //更新pool中可用状态
		scan_pool();

        // 根据状态 做业务分配
		if (idle_count > MAXSPARESERVERS) {
		//fprintf(stderr, "[%d]: kill %d server(s).\n", getpid(), (idle_count-MAXSPARESERVERS));
			for (i=0;i<(idle_count-MAXSPARESERVERS);++i) {
				del_1_server();
			}
		} else if (idle_count < MINSPARESERVERS) {
			for (i=0;i<MINSPARESERVERS-idle_count; ++i) {
				add_1_server();
			}
		}
		for (i=0;i<MAXCLIENTS;++i) {
			if (serverpool[i].pid==-1) {
				putchar(' ');
			} else {
				if (serverpool[i].state == STATE_IDLE) {
					putchar('.');
				} else {
					putchar('x');
				}
			}
		}
		putchar('\n');
	}

	sigprocmask(SIG_SETMASK, &oset, NULL);

	exit(0);
}

