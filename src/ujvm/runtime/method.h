#pragma once

#include "classfile/constant_pool.h"
#include "classfile/method_info.h"
#include "instance_class.h"
#include "oop_value_type.h"
#include "native_method.h"
#include "code_reader.h"


class Method {
private:
    ConstantPool *cp_;
    MethodInfo *methodInfo_;
    InstanceKlass *clazz_;

    strings::String methodName_;
    strings::String methodDesc_;
    strings::String methodKey;
    vector<ValueType> argTypes_;
    int argsSlotCount_;
    NativeMethod *nativeMethod_;
    int maxLocals_;
    int maxStack_;


    // (I[ILjava/lang/String;[Ljava/lang/Thread;Ljava/lang/Long;)V
    void parseArgsType();

    int calcArgsSlotCount();

    CodeReader codeReader_;
public:
    Method(InstanceKlass *clazz, MethodInfo *methodInfo);

    CodeAttribute *getCode() const {
        return methodInfo_->getCode();
    }

    CodeReader &getCodeReader() ;


    int getMaxLocals() const;

    int getMaxStack() const;

    vector<ValueType> &getMethodArgs() {
        return argTypes_;
    }

    ConstantPool *getCp() const {
        return cp_;
    }

    InstanceKlass *getClazz() const {
        return clazz_;
    }

    const strings::String &getMethodName() const {
        return methodName_;
    }

    const strings::String &getMethodDesc() const {
        return methodDesc_;
    }

    int getArgsSlotCount() const;

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