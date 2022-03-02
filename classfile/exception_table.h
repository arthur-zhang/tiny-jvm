//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_EXCEPTION_TABLE_H
#define TINY_JVM_EXCEPTION_TABLE_H

#include "types.h"
#include "class_reader.h"

class exception_table {
public:
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;

    exception_table(ClassReader &reader) {
        start_pc = reader.readUint16();
        end_pc = reader.readUint16();
        handler_pc = reader.readUint16();
        catch_type = reader.readUint16();
    }
};


#endif //TINY_JVM_EXCEPTION_TABLE_H
