//
// Created by ya on 2022/3/6.
//

#include "method_prameters_attribute.h"

MethodParametersAttribute::MethodParametersAttribute(ClassReader &reader) : AttributeInfo(reader) {
    parameters_count = reader.readUint8();
    if (parameters_count != 0)
        parameters = new MethodParameter *[parameters_count];
    for (int pos = 0; pos < parameters_count; pos++) {
        parameters[pos] = new MethodParameter(reader);
    }
}

MethodParametersAttribute::~MethodParametersAttribute() {
    if (parameters_count <= 0)return;

    for (int i = 0; i < parameters_count; ++i) {
        delete parameters[i];
    }
    delete[]parameters;
}