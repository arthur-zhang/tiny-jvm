//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_EXCEPTION_TABLE_ATTRIBUTE_H
#define TINY_JVM_EXCEPTION_TABLE_ATTRIBUTE_H

#include "attribute_info.h"

class ExceptionTableAttribute : public AttributeInfo {
public:
    u2 number_of_exceptions;
    u2 *exception_index_table = nullptr;

    ExceptionTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        number_of_exceptions = reader.readUInt16();
        if (number_of_exceptions != 0)
            exception_index_table = new u2[number_of_exceptions];
        for (int pos = 0; pos < number_of_exceptions; pos++) {
            exception_index_table[pos] = reader.readUInt16();
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(number_of_exceptions);
        for (int i = 0; i < number_of_exceptions; i++) {
            os.writeUInt16(exception_index_table[i]);
        }
    }

    virtual ~ExceptionTableAttribute() {
        delete[]exception_index_table;
    }
};


#endif //TINY_JVM_EXCEPTION_TABLE_ATTRIBUTE_H
