#pragma once

#include <map>
#include "shared/types.h"
#include "ujvm/classfile/classfile_parser.h"
#include "ujvm/class_loader.h"
#include "ujvm/runtime/common_value.h"
#include "ujvm/runtime/oop_desc.h"
#include "method.h"

class InstanceClassStruct {
private:
    ClassFile *cf_;
    map<strings::String, Method *> methodMap_;
    map<strings::String, OopDesc *> staticValueMap_;
public:
    InstanceClassStruct(ClassFile *cf) : cf_(cf), staticValueMap_({}) {
        for (int i = 0; i < cf->methods_count; ++i) {
            methodMap_[cf->methods[i]->getMethodName() + cf->methods[i]->getMethodDesc()]
                    = new Method(cf->constantPool, cf->methods[i]);
        }
    }

    ClassFile *getClassFile() const {
        return cf_;
    }


    const map<strings::String, Method *> &getMethodMap() const {
        return methodMap_;
    }

    Method *findMethod(const strings::String &methodName, const strings::String &methodDesc) {
        return methodMap_[methodName + methodDesc];
    }

    map<strings::String, OopDesc *> &getStaticValueMap() {
        return staticValueMap_;
    }


};

