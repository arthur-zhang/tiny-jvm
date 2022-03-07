//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_RUNTIME_INVISIBLE_ANNOTATIONS_ATTRIBUTE_HPP
#define TINY_JVM_RUNTIME_INVISIBLE_ANNOTATIONS_ATTRIBUTE_HPP

#include "attribute_info.h"
#include "annotations.hpp"

class RuntimeInvisibleAnnotations_attribute : public AttributeInfo {
public:
    Annotations *parameter_annotations;

    RuntimeInvisibleAnnotations_attribute(ClassReader &reader) : AttributeInfo(reader) {
        parameter_annotations = new Annotations(reader);
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        parameter_annotations->dump(os);
    }
};


#endif //TINY_JVM_RUNTIME_INVISIBLE_ANNOTATIONS_ATTRIBUTE_HPP
