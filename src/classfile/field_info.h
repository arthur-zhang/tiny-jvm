//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_FIELD_INFO_H
#define TINY_JVM_FIELD_INFO_H

#include "class_reader.h"
#include "constant_pool.hpp"
#include "attribute_info.h"

class FieldInfo {
public:
    FieldInfo(ClassReader &reader, ConstantPool *constantPool);

    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo **attributes = nullptr;
    void dump(DataOutputStream&os);

    ~FieldInfo();
};

#endif //TINY_JVM_FIELD_INFO_H
