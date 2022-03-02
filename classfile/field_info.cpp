//
// Created by ya on 2022/3/2.
//

#include "field_info.h"
#include "attribute_info.h"

FieldInfo::FieldInfo(ClassReader &reader, ConstantPool *constantPool) {
    access_flags = reader.readUint16();
    name_index = reader.readUint16();
    descriptor_index = reader.readUint16();
    attributes_count = reader.readUint16();
    if (attributes_count != 0)
        attributes = new AttributeInfo *[attributes_count];
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos] = AttributeInfo::readAttribute(reader, constantPool);
    }
}

FieldInfo::~FieldInfo() {
    if (attributes != nullptr) {
        for (int i = 0; i < attributes_count; i++) {
            delete attributes[i];
        }
        delete[] attributes;
    }
}
