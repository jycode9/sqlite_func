cmake_minimum_required(VERSION 3.10)
project(sqlite3)

set(STATIC_LIB_SOURCES src/sqlite3.c)

add_library(sqlite3 STATIC ${STATIC_LIB_SOURCES})

target_include_directories(sqlite3 PUBLIC ${PROJECT_SOURCE_DIR}/include)

