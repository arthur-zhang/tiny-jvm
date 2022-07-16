//
// Created by ya on 2022/3/2.
//

#include "classfile/field_info.h"
#include "classfile/attribute_info.h"

FieldInfo::FieldInfo(ClassReader &reader, ConstantPool *constantPool) {
    access_flags = reader.readUInt16();
    name_index = reader.readUInt16();
    descriptor_index = reader.readUInt16();
    attributes_count = reader.readUInt16();
    if (attributes_count != 0)
        attributes = new AttributeInfo *[attributes_count];
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos] = AttributeInfo::readAttributeInfo(reader, constantPool);
    }
}

FieldInfo::~FieldInfo() {
    if (attributes_count > 0) {
        for (int i = 0; i < attributes_count; i++) {
            delete attributes[i];
        }
        delete[] attributes;
    }
}

void FieldInfo::dump(DataOutputStream &os) {
    os.writeUInt16(access_flags);
    os.writeUInt16(name_index);
    os.writeUInt16(descriptor_index);
    os.writeUInt16(attributes_count);
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos]->dump(os);
    }
}
