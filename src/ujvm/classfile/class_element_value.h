//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_CLASS_ELEMENT_VALUE_H
#define TINY_JVM_CLASS_ELEMENT_VALUE_H

#include "element_value.h"

class ClassElementValue : public ElementValue {
public:
    u2 class_info_index;

    ClassElementValue(ClassReader &reader, u1 type);

    void dump(DataOutputStream &os) override;
};


#endif //TINY_JVM_CLASS_ELEMENT_VALUE_H
