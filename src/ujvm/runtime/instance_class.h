#pragma once

#include <map>
#include "shared/types.h"
#include "classfile/classfile_parser.h"
#include "ujvm/classpath/class_loader.h"
#include "oop_desc.h"
#include "method.h"
#include "klass.h"
#include "shared/jstring.h"

class Method;
#include "oop.h"


using namespace ujvm;

// 每个 InstanceKlass 对象表示一个具体的 Java 类（不包括 Java 数组）
class InstanceKlass : public Klass {
private:
    ClassFile *cf_;
    map<strings::String, Method *> methodMap_;
    map<strings::String, OOP> staticValueMap_;

public:
    InstanceKlass(ClassFile *cf);

    ClassFile *getClassFile() const;

    Method *findMethod(const strings::String &methodName, const strings::String &methodDesc);

    void setStaticFieldValue(strings::String &fieldName, OOP fieldValue);

    OOP getStaticFieldValue(const strings::String &fieldName);
};

