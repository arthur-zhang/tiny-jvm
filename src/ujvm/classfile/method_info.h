//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_METHOD_INFO_H
#define TINY_JVM_METHOD_INFO_H

#include "class_reader.h"
#include "constant_pool.h"
#include "attribute_info.h"
#include <shared/jstring.h>

class MethodInfo {
public:
    MethodInfo(ClassReader &reader, ConstantPool *constantPool, int a);

    void dump(DataOutputStream &os);

    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo **attributes = nullptr;


    strings::String getMethodName() {
        return ((CONSTANT_Utf8_info *) constantPool_->getConstantPool()[name_index])->getConstant();
    };

    strings::String getMethodDesc() {
        return ((CONSTANT_Utf8_info *) constantPool_->getConstantPool()[descriptor_index])->getConstant();
    };

    CodeAttribute *getCode() {
        for (int i = 0; i < attributes_count; ++i) {
            auto cast = dynamic_cast<CodeAttribute *> (attributes[i]);
            if (cast != nullptr) {
                return dynamic_cast<CodeAttribute *>(attributes[i]);
            }
        }
        return nullptr;
    }

    void parseArgs() {

    }


private:
    ConstantPool *constantPool_;
};


#endif //TINY_JVM_METHOD_INFO_H
