cmake_minimum_required(VERSION 3.21)
project(tiny_jvm)

set(CMAKE_CXX_STANDARD 14)

add_executable(tiny_jvm
        jvm_main.cpp
        classfile/classfile_parser.cpp
        classfile/classfile_parser.h
        classfile/String.cpp
        classfile/String.h
        )
