#pragma once

#include "attribute_info.h"
#include "annotations.hpp"

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
