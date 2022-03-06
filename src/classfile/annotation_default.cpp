//
// Created by ya on 2022/3/6.
//

#include "annotation_default.h"

AnnotationDefaultAttribute::~AnnotationDefaultAttribute() {
    delete default_value;
}

AnnotationDefaultAttribute::AnnotationDefaultAttribute(ClassReader &reader) : AttributeInfo(reader) {
    default_value = ElementValue::readElementValue(reader);
}

void AnnotationDefaultAttribute::dump(DataOutputStream &os) {
    AttributeInfo::dump(os);
    default_value->dump(os);
}
