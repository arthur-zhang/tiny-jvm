#ifndef TINY_JVM_SIGNATURE_ATTRIBUTE_HPP
#define TINY_JVM_SIGNATURE_ATTRIBUTE_HPP

#include "attribute_info.h"

class SignatureAttribute : public AttributeInfo {
public:
    u2 signature_index;

    SignatureAttribute(ClassReader &reader) : AttributeInfo(reader) {
        signature_index = reader.readUInt16();
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(signature_index);
    }
};


#endif //TINY_JVM_SIGNATURE_ATTRIBUTE_HPP
