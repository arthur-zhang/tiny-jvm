//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_LOCAL_VARIABLE_TYPE_TABLE_ATTRIBUTE_H
#define TINY_JVM_LOCAL_VARIABLE_TYPE_TABLE_ATTRIBUTE_H

#include "attribute_info.h"

class LocalVariableType {
public:
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 signature_index;
    u2 index;

    LocalVariableType(ClassReader &reader) {
        start_pc = reader.readUInt16();
        length = reader.readUInt16();
        name_index = reader.readUInt16();
        signature_index = reader.readUInt16();
        index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(length);
        os.writeUInt16(name_index);
        os.writeUInt16(signature_index);
        os.writeUInt16(index);
    }
};

class LocalVariableTypeTableAttribute : public AttributeInfo {
public:
    u2 local_variable_type_table_length;
    LocalVariableType **local_variable_type_table;

    LocalVariableTypeTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        local_variable_type_table_length = reader.readUInt16();
        if (local_variable_type_table_length != 0)
            local_variable_type_table = new LocalVariableType *[local_variable_type_table_length];
        for (int pos = 0; pos < local_variable_type_table_length; pos++) {
            local_variable_type_table[pos] = new LocalVariableType(reader);
        }
    }

    virtual ~LocalVariableTypeTableAttribute() {
        if (local_variable_type_table_length > 0) {
            for (int pos = 0; pos < local_variable_type_table_length; pos++) {
                delete local_variable_type_table[pos];
            }
            delete[]local_variable_type_table;
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(local_variable_type_table_length);
        for (int pos = 0; pos < local_variable_type_table_length; pos++) {
            local_variable_type_table[pos]->dump(os);
        }
    }
};


#endif //TINY_JVM_LOCAL_VARIABLE_TYPE_TABLE_ATTRIBUTE_H
