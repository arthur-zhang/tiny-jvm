//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_DEPRECATED_ATTRIBUTE_H
#define TINY_JVM_DEPRECATED_ATTRIBUTE_H

#include "attribute_info.h"

class DeprecatedAttribute : public AttributeInfo {
public:
    u1 *bytes;

    DeprecatedAttribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeBytes(bytes, attribute_length);
    }

    virtual ~DeprecatedAttribute() {
        if (attribute_length > 0) delete[]bytes;
    }
};


#endif //TINY_JVM_DEPRECATED_ATTRIBUTE_H
