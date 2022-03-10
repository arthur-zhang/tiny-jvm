//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_METHOD_PRAMETERS_ATTRIBUTE_H
#define TINY_JVM_METHOD_PRAMETERS_ATTRIBUTE_H

#include "attribute_info.h"

/**
 * This class represents a MethodParameters attribute.
 *
 * @see <a href="https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.24">
 MethodParameters_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 parameters_count;
    {   u2 name_index;
        u2 access_flags;
    } parameters[parameters_count];
}
 */
class MethodParametersAttribute : public AttributeInfo {
public:
    u1 parameters_count;
    MethodParameter **parameters = nullptr;                                    // [parameters_count];

    MethodParametersAttribute(ClassReader &reader);

    ~MethodParametersAttribute();

    void dump(DataOutputStream &os) override {
        os.writeUInt8(parameters_count);
        for (int i = 0; i < parameters_count; ++i) {
            parameters[i]->dump(os);
        }
    }
};


#endif //TINY_JVM_METHOD_PRAMETERS_ATTRIBUTE_H
