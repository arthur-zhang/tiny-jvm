#pragma once

#include "attribute_info.h"

class RuntimeVisibleAnnotationsAttribute : public AttributeInfo {
public:
    parameter_annotations_t *parameter_annotations;

    RuntimeVisibleAnnotationsAttribute(ClassReader &reader);

    void dump(DataOutputStream &os) override;
};


