[toc]

# learn apue
## 第一章内容 perror（）  主要操作系统上的环境变量  获取和调整
add_executable(test013    fig1.3_opendir_struct_dirent.c)
add_executable(test014    fig1.4test_read_write.c)
add_executable(test015    fig1.5getc_putc.c)
add_executable(test016    fig1.6getpid_getppid.c)
add_executable(test017    fig1.7fork_execlp_waitpid.c)
add_executable(test018    fig1.8strerror_errno_perror.c)
add_executable(test019    fig1.9_getuid_getgid.c)
add_executable(test0110   fig1.10_singal.c)

## 第二章  sysconf() 基于不同操作系统提供的一些系统环境变量的修改
add_executable(test02150   fig2.15_pathconf_sysconf.c)
add_executable(test02160   fig2.16_sysconf_limits.c)


## 第三章  read write 系统io缓冲  其实并没有  

```
这里主要介绍了fcntl函数对我们的访问权限 用户组做了一个控制
同时介绍量我们的函数可以做一些线程相关控制的同步 和异步操作

同时介绍了空洞文件非常nice的多线程等控制

同时关于文件操作读写所控制，本身文件io就有的一些基础的属性，内核已经提供给我们了
当我们用到的时候肯定可以发挥更大的功能和作用啊～

```




# 第四章  主要讲文件和目录
通过文件描述符  获取文件相关属性
属性分基础属性，文件符号和链接
高级属性探索ing

//根据文件路径获取
stat()
//根据文件描述符获取
fstat()
//根据路径返回和链接符有关的信息
lfstat()

这是文件结构描述
```
 struct stat {
       mode_t    st_mode;      /* file type & mode (permissions) */ //访问权限
       ino_t     st_ino;       /* i-node number (serial number) */ // i节点信息 逻辑地址信息
       dev_t     st_dev;       /* device number (file system) */ // 物理设备数量
       dev_t     st_rdev;      /* device number for special files */ //只有字符特殊设备和块特殊设备才会有st_rdev值。此值包含实际设备的设备号。
       nlink_t   st_nlink;     /* number of links */ // 链接的个数？
       uid_t     st_uid;       /* user ID of owner */
       gid_t     st_gid;       /* group ID of owner */
       off_t     st_size;      /* size in bytes, for regular files */
       time_t    st_atime;     /* time of last access */ //访问实践
       time_t    st_mtime;     /* time of last modification */
       time_t    st_ctime;     /* time of last file status change */
       blksize_t st_blksize;   /* best I/O block size */
       blkcnt_t  st_blocks;    /* number of disk blocks allocated */
     };

```



#第四章  stat 获取文件信息  getcwd获取工作路径  chdir 更改工作路径   path_alloc可移植的获取文件名称长度
这章节不仅仅讲的文件获取方式，
前面更多的是讲内核文件是如何组织的，思考操作系统中的  逻辑地址->线性地址->物理地址
在思考文件inode  vnode  block
 [ ]  构建内核架构图  最好形成知识的回路  融会贯通太重要了
 [ ] 

add_executable(test0110   fig1.10_singal.c)



add_executable(current_debug   fig1.10_singal.c)



#第五章 标准io库

印象中这章节主要讲的就是io的缓冲  半缓冲  全缓冲  行缓冲
基于内核的实现不同  采取的不同的交互接口实现方式  这个对我理解内核中io有很大益处
一切的io操作都是围绕着stream流进行的
标准io库和内核io库的区别和练习？
行缓冲大小和全缓冲大小在传输效率上的区别？

用到的函数列表
```
int fwide()   //

```

#第六章 系统数据 
获取进程环境相关数据
包括用户  用户组  密码等



#第七章 进程环境
###main函数
内核使用exec函数创建 启动例程
###进程终止
终止（termination）的几种方式
从main函数返回
调用exit
调用_exit活着_Exit
最后一个线程从启动例程返回
最后一个线程调用pthread_ext

异常终止的三种方式
调用abort
接到一个信号并终止
最后一个线程对取消请求做出响应
###命令行参数
###环境表
###c程序的存储结构
###共享库??????
###存储器分配
###环境变量  
环境变量和环境表的区别？
###setjmp  和 longsetmp 区别？
###getrlimit 和  setrlimit？？
###小结  嗯～～～ 奈斯



# 第八章 进程控制哦  相关的全部内容

先思考几个问题
1.父子进程如何得到对方的状态？
2.得到什么样的状态，得到状态后应该怎么做？
3.mutex用于线程这是一个不可用的模型，在进程控制中不可用，那么其他的socket 还有fifo？用于通信吗？
4.信号+pid是否能控制进程？
5.手动表情 [一脸茫然]😃

这章内容主要就是解释  
竞争状态  
控制竞争的方式 
实现的原理 

我想理解这些进程控制差不多就够了