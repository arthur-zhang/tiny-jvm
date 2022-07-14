#pragma once

#include <ujvm/classfile/constant_pool.h>
#include <ujvm/classfile/method_info.h>
#include "ujvm/runtime/instance_class.h"
#include "oop_value_type.h"
#include "native_method.h"


class Method {
private:
    ConstantPool *cp_;
    MethodInfo *methodInfo_;
    InstanceClassStruct *clazz_;

    strings::String methodName_;
    strings::String methodDesc_;
    strings::String methodKey;
    vector<ValueType> argTypes_;
    int argsSlotCount_;
    NativeMethod *nativeMethod_;

    // (I[ILjava/lang/String;[Ljava/lang/Thread;Ljava/lang/Long;)V
    void parseArgsType();

    int calcArgsSlotCount();

public:
    Method(InstanceClassStruct *clazz, MethodInfo *methodInfo);

    CodeAttribute *getCode() const {
        return methodInfo_->getCode();
    }

    vector<ValueType> &getMethodArgs() {
        return argTypes_;
    }

    ConstantPool *getCp() const {
        return cp_;
    }

    InstanceClassStruct *getClazz() const {
        return clazz_;
    }

    const strings::String &getMethodName() const {
        return methodName_;
    }

    const strings::String &getMethodDesc() const {
        return methodDesc_;
    }

    NativeMethod *getNativeMethod() {
        if (this->isNative()) {
            if (nativeMethod_ == nullptr) {
                nativeMethod_ = NativeMethod::resolve(this);
            }
        }
        return nativeMethod_;
    }

    bool isStatic() {
        return (methodInfo_->access_flags & ACC_STATIC) != 0;
    }

    bool isNative() {
        return (methodInfo_->access_flags & ACC_NATIVE) != 0;
    }
};