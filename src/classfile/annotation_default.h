#pragma once

/**
 * Represents the default value of a annotation for a method info
 */
#include "attribute_info.h"

class AnnotationDefaultAttribute : public AttributeInfo {
public:
    AnnotationDefaultAttribute(ClassReader &reader);

    ElementValue *default_value;

    void dump(DataOutputStream &os) override;

    virtual ~AnnotationDefaultAttribute();
};

