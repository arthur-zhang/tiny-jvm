//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_FIELD_INFO_H
#define TINY_JVM_FIELD_INFO_H

#include "class_reader.h"
#include "constant_pool.h"
#include "attribute_info.h"

class FieldInfo {
public:
    FieldInfo(ClassReader &reader, ConstantPool *constantPool) {
        access_flags = reader.readUint16();
        name_index = reader.readUint16();
        descriptor_index = reader.readUint16();
        attributes_count = reader.readUint16();
        if (attributes_count != 0)        // !!!!! attention !!!!!
            attributes = new attribute_info *[attributes_count];
        for (int pos = 0; pos < attributes_count; pos++) {
            attributes[pos] = parseAttribute(reader, constantPool);
        }
    }

    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info **attributes = nullptr;        // [attributes_count]
    ~FieldInfo() {
        if (attributes != nullptr) {
            for (int i = 0; i < attributes_count; i++) {
                delete attributes[i];
            }
            delete[] attributes;
        }
    }
};

#endif //TINY_JVM_FIELD_INFO_H
