#pragma once

#include <map>
#include "shared/types.h"
#include "classfile/classfile_parser.h"
#include "ujvm/class_loader.h"
#include "ujvm/runtime/common_value.h"
#include "ujvm/runtime/oop_desc.h"
#include "method.h"

class Method;
class OopDesc;

class InstanceClassStruct {
private:
    ClassFile *cf_;
    map<strings::String, Method *> methodMap_;
    map<strings::String, OopDesc *> staticValueMap_;
    strings::String className_;
public:
    InstanceClassStruct(ClassFile *cf);

    ClassFile *getClassFile() const {
        return cf_;
    }

    const strings::String &getClassName() const {
        return className_;
    }


    inline const map<strings::String, Method *> &getMethodMap() const {
        return methodMap_;
    }

    inline Method *findMethod(const strings::String &methodName, const strings::String &methodDesc) {
        return methodMap_[methodName + methodDesc];
    }

    inline map<strings::String, OopDesc *> &getStaticValueMap() {
        return staticValueMap_;
    }


};

