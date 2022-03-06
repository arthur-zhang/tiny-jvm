//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_RUNTIME_VISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_H
#define TINY_JVM_RUNTIME_VISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_H

#include "attribute_info.h"
#include "target_info_t.h"

class RuntimeVisibleTypeAnnotations_attribute : public AttributeInfo {
public:
    u1 *bytes;

    RuntimeVisibleTypeAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        bytes = reader.readBytes(attribute_length);
    }

    u2 num_annotations;
    type_annotation *annotations = nullptr;                    // [num_annotations];
    void dump(DataOutputStream &os) override {
        os.writeBytes(bytes, attribute_length);
    }
};


#endif //TINY_JVM_RUNTIME_VISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_H
