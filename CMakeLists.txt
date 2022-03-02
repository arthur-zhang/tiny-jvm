cmake_minimum_required(VERSION 3.21)
project(tiny_jvm)

set(CMAKE_CXX_STANDARD 14)

add_executable(tiny_jvm jvm_main.cpp classfile_parser.cpp classfile_parser.h String.cpp String.h)
