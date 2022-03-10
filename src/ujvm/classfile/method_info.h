//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_METHOD_INFO_H
#define TINY_JVM_METHOD_INFO_H

#include "class_reader.h"
#include "constant_pool.hpp"
#include "attribute_info.h"

class MethodInfo {
public:
    MethodInfo(ClassReader &reader, ConstantPool *constantPool, int a);

    void dump(DataOutputStream &os);

    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo **attributes = nullptr;
};


#endif //TINY_JVM_METHOD_INFO_H
