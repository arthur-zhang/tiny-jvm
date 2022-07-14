cmake_minimum_required(VERSION 3.19.1)
project(tiny_jvm)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)

include_directories(include)
include_directories(include/shared)



set(INCLUDE_LIST include/shared/jstring.h include/shared/fs.h)
enable_testing()
add_subdirectory(src)
add_subdirectory(test)
