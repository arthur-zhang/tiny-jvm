#pragma once

#include "attribute_info.h"

class RuntimeVisibleAnnotationsAttribute : public AttributeInfo {
public:
    Annotations *parameter_annotations;

    RuntimeVisibleAnnotationsAttribute(ClassReader &reader);

    void dump(DataOutputStream &os) override;
};


