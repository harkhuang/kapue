project(testinuap)
cmake_minimum_required(VERSION 3.9)
set(CMAKE_BUILD_TYPE "Debug")

enable_testing()

add_executable(test16    fig1.6.c)
add_executable(test17    fig1.7.c)
add_executable(current_debug    fig1.7.c)

# 添加测试的一种方式
add_test(test16  16)
add_test(test17 17)