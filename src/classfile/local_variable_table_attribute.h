//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H
#define TINY_JVM_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H

#include "attribute_info.h"
class LocalVariable {
public:
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;

    LocalVariable(ClassReader &reader) {
        start_pc = reader.readUInt16();
        length = reader.readUInt16();
        name_index = reader.readUInt16();
        descriptor_index = reader.readUInt16();
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(length);
        os.writeUInt16(name_index);
        os.writeUInt16(descriptor_index);
        os.writeUInt16(index);
    };
};

class LocalVariableTableAttribute : public AttributeInfo {
public:
    u2 local_variable_table_length;

    LocalVariable **local_variable_table = nullptr;    // [local_variable_table_length]
    LocalVariableTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        local_variable_table_length = reader.readUInt16();
        if (local_variable_table_length != 0)
            local_variable_table = new LocalVariable *[local_variable_table_length];
        for (int pos = 0; pos < local_variable_table_length; pos++) {
            local_variable_table[pos] = new LocalVariable(reader);
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(local_variable_table_length);
        for (int pos = 0; pos < local_variable_table_length; pos++) {
            local_variable_table[pos]->dump(os);
        }
    }
};


#endif //TINY_JVM_LOCAL_VARIABLE_TABLE_ATTRIBUTE_H
