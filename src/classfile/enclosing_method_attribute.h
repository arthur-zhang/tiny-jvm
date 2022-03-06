//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_ENCLOSING_METHOD_ATTRIBUTE_H
#define TINY_JVM_ENCLOSING_METHOD_ATTRIBUTE_H

#include "attribute_info.h"

class EnclosingMethodAttribute : public AttributeInfo {
public:
    u2 class_index;
    u2 method_index;

    EnclosingMethodAttribute(ClassReader &reader) : AttributeInfo(reader) {
        class_index = reader.readUInt16();
        method_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(class_index);
        os.writeUInt16(method_index);
    }
};

#endif //TINY_JVM_ENCLOSING_METHOD_ATTRIBUTE_H
