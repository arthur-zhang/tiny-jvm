//
// Created by arthur on 2022/6/29.
//
#include "instance_class.h"
#include "classfile/constant.hpp"

strings::String parseClassName(ClassFile *cf) {
    auto classInfo = (CONSTANT_Class_info *) cf->constantPool->getConstantPool()[cf->this_class];
    return ((CONSTANT_Utf8_info *) cf->constantPool->getConstantPool()[classInfo->index])->getConstant();
}

InstanceKlass::InstanceKlass(ClassFile *cf) : cf_(cf), staticValueMap_({}) {
    className_ = parseClassName(cf);
    for (int i = 0; i < cf->methods_count; ++i) {
        methodMap_[cf->methods[i]->getMethodName() + cf->methods[i]->getMethodDesc()]
                = new Method(this, cf->methods[i]);
    }
}

Method *InstanceKlass::findMethod(const strings::String &methodName, const strings::String &methodDesc) {
    return methodMap_[methodName + methodDesc];
}

void InstanceKlass::setStaticFieldValue(strings::String &fieldName, OOP fieldValue) {
    staticValueMap_[fieldName] = fieldValue;
}

OOP InstanceKlass::getStaticFieldValue(const strings::String &fieldName) {
    return staticValueMap_[fieldName];
}

ClassFile *InstanceKlass::getClassFile() const {
    return cf_;
}


