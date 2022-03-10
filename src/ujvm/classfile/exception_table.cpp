//
// Created by ya on 2022/3/2.
//

#include "exception_table.h"


ExceptionTable::ExceptionTable(ClassReader &reader) {
    start_pc = reader.readUInt16();
    end_pc = reader.readUInt16();
    handler_pc = reader.readUInt16();
    catch_type = reader.readUInt16();
}

void ExceptionTable::dump(DataOutputStream &os) {
    os.writeUInt16(start_pc);
    os.writeUInt16(end_pc);
    os.writeUInt16(handler_pc);
    os.writeUInt16(catch_type);
}
