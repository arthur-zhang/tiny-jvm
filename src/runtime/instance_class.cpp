//
// Created by arthur on 2022/6/29.
//
#include "instance_class.h"
#include "classfile/constant.h"

strings::String parseClassName(ClassFile *cf) {
    auto classInfo = (CONSTANT_Class_info *) cf->constantPool->getConstantPool()[cf->this_class];
    return ((CONSTANT_Utf8_info *) cf->constantPool->getConstantPool()[classInfo->index])->getConstant();
}

InstanceClassStruct::InstanceClassStruct(ClassFile *cf) : cf_(cf), staticValueMap_({}) {
    className_ = parseClassName(cf);
    for (int i = 0; i < cf->methods_count; ++i) {
        methodMap_[cf->methods[i]->getMethodName() + cf->methods[i]->getMethodDesc()]
                = new Method(this, cf->methods[i]);
    }
}


