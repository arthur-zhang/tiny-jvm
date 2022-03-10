//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_CONSTANT_VALUE_ATTRIBUTE_H
#define TINY_JVM_CONSTANT_VALUE_ATTRIBUTE_H

#include "attribute_info.h"

class ConstantValueAttribute : public AttributeInfo {
public:
    ConstantValueAttribute(ClassReader &reader);
    u2 constant_value_index;

    void dump(DataOutputStream &os) override;
};


#endif //TINY_JVM_CONSTANT_VALUE_ATTRIBUTE_H
