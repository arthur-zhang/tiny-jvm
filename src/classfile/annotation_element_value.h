#pragma once

#include "element_value.h"

class AnnotationElementValue : public ElementValue {
public:
    AnnotationElementValue(ClassReader &reader, u1 tag);

    AnnotationEntry *annotationEntry;

    void dump(DataOutputStream &os) override;
};
