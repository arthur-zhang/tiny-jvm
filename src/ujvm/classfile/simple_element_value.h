//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_SIMPLE_ELEMENT_VALUE_H
#define TINY_JVM_SIMPLE_ELEMENT_VALUE_H

#include "element_value.h"

class SimpleElementValue : public ElementValue {
public:
    u2 const_value_index;

    SimpleElementValue(ClassReader &reader, u1 type);

    void dump(DataOutputStream &os) override {
        ElementValue::dump(os);

        os.writeUInt16(const_value_index);
    }
};


#endif //TINY_JVM_SIMPLE_ELEMENT_VALUE_H
