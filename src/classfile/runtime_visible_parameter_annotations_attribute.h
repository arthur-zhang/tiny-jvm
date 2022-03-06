//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS_ATTRIBUTE_H
#define TINY_JVM_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS_ATTRIBUTE_H

#include "attribute_info.h"

class RuntimeVisibleParameterAnnotationsAttribute : public AttributeInfo {
public:
    u1 num_parameters;
    Annotations **parameter_annotations = nullptr;        // [num_parameters];

    RuntimeVisibleParameterAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
        num_parameters = reader.readUint8();
        if (num_parameters != 0)
            parameter_annotations = new Annotations *[num_parameters];
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos] = new Annotations(reader);
        }
    }

    void dump(DataOutputStream &os) override {
        os.writeUInt8(num_parameters);
        for (int pos = 0; pos < num_parameters; pos++) {
            parameter_annotations[pos]->dump(os);
        }
    }
};


#endif //TINY_JVM_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS_ATTRIBUTE_H
