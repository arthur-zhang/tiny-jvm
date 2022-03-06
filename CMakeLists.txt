cmake_minimum_required(VERSION 3.19.1)
project(tiny_jvm)

set(CMAKE_CXX_STANDARD 20)
include_directories(${PROJECT_SOURCE_DIR}/src)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
