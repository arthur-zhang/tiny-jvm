//
// Created by ya on 2022/3/5.
//

#ifndef TINY_JVM_ELEMENT_VALUE_PAIR_H
#define TINY_JVM_ELEMENT_VALUE_PAIR_H

#include "class_reader.h"
#include "element_value.h"

class ElementValue;

class ElementValuePair {
public:
    ElementValuePair(ClassReader &reader);

    u2 element_name_index;
    ElementValue *value;

    void dump(DataOutputStream &os);
};


#endif //TINY_JVM_ELEMENT_VALUE_PAIR_H
