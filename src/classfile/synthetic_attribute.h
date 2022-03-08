//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_SYNTHETIC_ATTRIBUTE_H
#define TINY_JVM_SYNTHETIC_ATTRIBUTE_H

#include "attribute_info.h"

class SyntheticAttribute : public AttributeInfo {
public:
    u1 *bytes;

    SyntheticAttribute(ClassReader &reader) : AttributeInfo(reader) {
        if (attribute_length <= 0) return;
        bytes = reader.readBytes(attribute_length);
    }

    virtual ~SyntheticAttribute() {
        if (attribute_length <= 0) return;
        delete[]bytes;
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        if (attribute_length > 0) {
            os.writeBytes(bytes, attribute_length);
        }
    }
};


#endif //TINY_JVM_SYNTHETIC_ATTRIBUTE_H
