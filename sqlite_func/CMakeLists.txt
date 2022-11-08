#设置当前支持的cmake最低版本
cmake_minimum_required(VERSION 3.10)
#设置当前工程名，并设置支持的语言（若混用必须声明C）
project(sqlite_func LANGUAGES C CXX)
#添加头文件
include_directories(./include/sqlite_func.h
		./thirdparty/sqlite3.h)
#设置工程的源文件
add_executable(sqlite_func ./src/sqlite_func.cpp)
#设置外部库依赖
target_link_libraries( ${PROJECT_NAME} PUBLIC ../thirdparty/sqlite3)
