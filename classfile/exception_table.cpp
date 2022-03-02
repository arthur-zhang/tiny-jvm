//
// Created by ya on 2022/3/2.
//

#include "exception_table.h"

ExceptionTable::ExceptionTable(ClassReader &reader) {
    start_pc = reader.readUint16();
    end_pc = reader.readUint16();
    handler_pc = reader.readUint16();
    catch_type = reader.readUint16();
}
