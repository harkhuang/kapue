project(testinuap)
cmake_minimum_required(VERSION 3.9)
set(CMAKE_BUILD_TYPE "Debug")

enable_testing()

#第一章内容 perror（）  主要操作系统上的环境变量  获取和调整
add_executable(test013    fig1.3_opendir_struct_dirent.c)
add_executable(test014    fig1.4test_read_write.c)
add_executable(test015    fig1.5getc_putc.c)
add_executable(test016    fig1.6getpid_getppid.c)
add_executable(test017    fig1.7fork_execlp_waitpid.c)
add_executable(test018    fig1.8strerror_errno_perror.c)
add_executable(test019    fig1.9_getuid_getgid.c)
add_executable(test0110   fig1.10_singal.c)

#第二章  sysconf() 基于不同操作系统提供的一些系统环境变量的修改
add_executable(test02150   fig2.15_pathconf_sysconf.c)
add_executable(test02160   fig2.16_sysconf_limits.c)


#第三章  read write 系统io缓冲  其实并没有  

```
这里主要介绍了fcntl函数对我们的访问权限 用户组做了一个控制
同时介绍量我们的函数可以做一些线程相关控制的同步 和异步操作

同时介绍了空洞文件非常nice的多线程等控制

```
add_executable(test0110   fig1.10_singal.c)



add_executable(current_debug   fig1.10_singal.c)


# 添加测试的一种方式
add_test(test013  16)
add_test(test014  17)