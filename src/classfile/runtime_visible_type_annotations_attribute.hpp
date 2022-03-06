#pragma once

#include "attribute_info.h"
#include "target_info_t.h"

class RuntimeVisibleTypeAnnotationsAttribute : public AttributeInfo {
public:
    u1 *bytes;

    RuntimeVisibleTypeAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    u2 num_annotations;
    type_annotation *annotations = nullptr;                    // [num_annotations];
    void dump(DataOutputStream &os) override {
        os.writeBytes(bytes, attribute_length);
    }
};


