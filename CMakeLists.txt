cmake_minimum_required(VERSION 3.21)
project(tiny_jvm)

set(CMAKE_CXX_STANDARD 14)

add_executable(tiny_jvm
        jvm_main.cpp
        classfile/classfile_parser.cpp
        classfile/classfile_parser.h
        classfile/String.cpp
        classfile/String.h
        classfile/class_reader.cpp
        classfile/class_reader.h
        classfile/field_info.cpp
        classfile/field_info.h
        classfile/constant_pool.cpp
        classfile/constant_pool.h
        classfile/constant.cpp
        classfile/constant.h
        classfile/attribute_info.cpp classfile/attribute_info.h classfile/const.h classfile/method_info.cpp classfile/method_info.h)
