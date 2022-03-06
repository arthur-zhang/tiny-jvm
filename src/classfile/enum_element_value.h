//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_ENUM_ELEMENT_VALUE_H
#define TINY_JVM_ENUM_ELEMENT_VALUE_H

#include "element_value.h"

class EnumElementValue : public ElementValue {
public:
    EnumElementValue(ClassReader &reader, u1 type);

    u2 type_name_index;
    u2 const_name_index;

    void dump(DataOutputStream &os) override;
};


#endif //TINY_JVM_ENUM_ELEMENT_VALUE_H
