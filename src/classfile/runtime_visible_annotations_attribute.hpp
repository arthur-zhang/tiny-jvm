#pragma once

#include "attribute_info.h"

class RuntimeVisibleAnnotationsAttribute : public AttributeInfo {
public:
    Annotations *parameter_annotations;

    RuntimeVisibleAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        parameter_annotations = new Annotations(reader);
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        parameter_annotations->dump(os);
    }
};
