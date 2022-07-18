#pragma once

#include <map>
#include "shared/types.h"
#include "classfile/classfile_parser.h"
#include "classfile/class_loader.h"
#include "oop_desc.h"
#include "method.h"
#include "klass.h"

class Method;

class OopDesc;

// 每个 InstanceKlass 对象表示一个具体的 Java 类（这里的 Java 类不包括 Java 数组）
class InstanceKlass : public Klass {
private:
    ClassFile *cf_;
    map<strings::String, Method *> methodMap_;
    map<strings::String, OopDesc *> staticValueMap_;

public:
    InstanceKlass(ClassFile *cf);

    ClassFile *getClassFile() const {
        return cf_;
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

