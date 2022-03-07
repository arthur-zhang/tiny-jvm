#pragma once

#include "attribute_info.h"
#include "annotations.hpp"

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