//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_EXCEPTION_TABLE_H
#define TINY_JVM_EXCEPTION_TABLE_H

#include "types.hpp"
#include "class_reader.h"

class ExceptionTable {
public:
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
    ExceptionTable(ClassReader &reader);
};


#endif //TINY_JVM_EXCEPTION_TABLE_H
