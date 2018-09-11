project(testinuap)
cmake_minimum_required(VERSION 3.9)
set(CMAKE_BUILD_TYPE "Debug")

enable_testing()
add_executable(test013    fig1.3_opendir_struct_dirent.c)
add_executable(test014    fig1.4test_read_write.c)
add_executable(test015    fig1.5getc_putc.c)
add_executable(test016    fig1.6getpid_getppid.c)
add_executable(test017    fig1.7fork_execlp_waitpid.c)
add_executable(test018    fig1.8strerror_errno_perror.c)
add_executable(test019    fig1.9_getuid_getgid.c)
add_executable(test0110   fig1.10_singal.c)

# 添加测试的一种方式
add_test(test013  16)
add_test(test014  17)