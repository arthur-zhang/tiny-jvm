//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_SOURCE_DEBUG_EXTENSION_ATTRIBUTE_HPP
#define TINY_JVM_SOURCE_DEBUG_EXTENSION_ATTRIBUTE_HPP

#include "attribute_info.h"
class SourceDebugExtensionAttribute : public AttributeInfo {
public:
    u1 *debug_extension = nullptr;

    SourceDebugExtensionAttribute(ClassReader &reader) : AttributeInfo(reader) {
        if (attribute_length != 0) {
            debug_extension = new u1[attribute_length];
            debug_extension = reader.readBytes(attribute_length);
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        if (attribute_length > 0) {
            os.writeBytes(debug_extension, attribute_length);
        }
    }
};



#endif //TINY_JVM_SOURCE_DEBUG_EXTENSION_ATTRIBUTE_HPP
