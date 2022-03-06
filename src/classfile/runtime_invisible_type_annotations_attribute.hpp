//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_HPP
#define TINY_JVM_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_HPP

#include "attribute_info.h"

// todo
class RuntimeInvisibleTypeAnnotationsAttribute : public AttributeInfo {
public:
    u1 *bytes;

    RuntimeInvisibleTypeAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    void dump(DataOutputStream &os) override {
        os.writeBytes(bytes, attribute_length);
    }
};


#endif //TINY_JVM_RUNTIME_INVISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_HPP
