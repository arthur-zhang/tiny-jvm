//
// Created by ya on 2022/3/6.
//

#include "annotation_element_value.h"

AnnotationElementValue::AnnotationElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    annotationEntry = new AnnotationEntry(reader);
}

void AnnotationElementValue::dump(DataOutputStream &os) {
    ElementValue::dump(os);
    annotationEntry->dump(os);
}
