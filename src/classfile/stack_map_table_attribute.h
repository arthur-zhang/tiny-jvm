//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_STACK_MAP_TABLE_ATTRIBUTE_H
#define TINY_JVM_STACK_MAP_TABLE_ATTRIBUTE_H

#include "attribute_info.h"

class StackMapTableAttribute : public AttributeInfo {
public:
    StackMapTableAttribute(ClassReader &reader);

    void dump(DataOutputStream &os) override;

    virtual ~StackMapTableAttribute();

private:
    u1 *bytes;
};


#endif //TINY_JVM_STACK_MAP_TABLE_ATTRIBUTE_H
