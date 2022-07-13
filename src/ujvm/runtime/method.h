#pragma once

#include <ujvm/classfile/constant_pool.h>
#include <ujvm/classfile/method_info.h>
#include "oop_value_type.h"

class Method {
private:
    ConstantPool *cp_;
    MethodInfo *methodInfo_;

    strings::String getMethodName();;
    strings::String getMethodDesc();;
    strings::String methodName_;
    strings::String methodDesc_;
    vector<ValueType> argTypes_;
    int argsSlotCount_;

    // (I[ILjava/lang/String;[Ljava/lang/Thread;Ljava/lang/Long;)V
    void parseArgsType();


    int calcArgsSlotCount();

public:
    Method(ConstantPool *cp, MethodInfo *methodInfo) : cp_(cp), methodInfo_(methodInfo) {
        methodName_ = getMethodName();
        methodDesc_ = getMethodDesc();
        parseArgsType();
        argsSlotCount_ = calcArgsSlotCount();
    }

    CodeAttribute *getCode() const {
        return methodInfo_->getCode();
    }

    vector<ValueType> &getMethodArgs() {
        return argTypes_;
    }

    ConstantPool *getCp() const {
        return cp_;
    }

    bool isStatic() {
        return (methodInfo_->access_flags & ACC_STATIC) != 0;
    }
};