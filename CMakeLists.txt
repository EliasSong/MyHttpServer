cmake_minimum_required (VERSION 2.8)

project (MyHttpServer)
find_library(PTHREAD pthread)
find_library(LIBEVENT event)

aux_source_directory(. DIR_LIB_SRCS)
add_library (SRC ${DIR_LIB_SRCS})

add_executable(myhttpd main.cpp)
target_link_libraries(myhttpd SRC ${PTHREAD} ${LIBEVENT}) 