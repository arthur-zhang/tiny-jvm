#pragma once

#include "class_reader.h"
#include "classfile/constant_pool.h"
#include "classfile/attribute_info.h"

class FieldInfo {
public:
    FieldInfo(ClassReader &reader, ConstantPool *constantPool);

    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo **attributes = nullptr;

    void dump(DataOutputStream &os);

    ~FieldInfo();
};