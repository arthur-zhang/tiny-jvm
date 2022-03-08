#pragma once

#include "attribute_info.h"

/**
 * Represents the default value of a annotation for a method info
 */
class AnnotationDefaultAttribute : public AttributeInfo {
public:
    AnnotationDefaultAttribute(ClassReader &reader) : AttributeInfo(reader) {
        default_value = ElementValue::readElementValue(reader);
    }

    ElementValue *default_value;

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        default_value->dump(os);
    }

    ~AnnotationDefaultAttribute() {
        delete default_value;
    }
};

