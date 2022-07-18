//
// Created by arthur on 2022/7/10.
//

#include "method.h"
#include "instance_class.h"

Method::Method(InstanceKlass *clazz, MethodInfo *methodInfo) : methodInfo_(methodInfo), clazz_(clazz) {
    cp_ = clazz->getClassFile()->constantPool;
    methodName_ = ((CONSTANT_Utf8_info *) cp_->getConstantPool()[methodInfo_->name_index])->getConstant();
    methodDesc_ = ((CONSTANT_Utf8_info *) cp_->getConstantPool()[methodInfo_->descriptor_index])->getConstant();

    parseArgsType();
    argsSlotCount_ = calcArgsSlotCount();
    if (methodInfo_->getCode()) {
        maxLocals_ = methodInfo_->getCode()->max_locals;
        maxStack_ = methodInfo_->getCode()->max_stack;
    }
    if (methodInfo->getCode()) {
        codeReader_ = CodeReader(methodInfo->getCode()->code, methodInfo->getCode()->code_length);
    }
}

void Method::parseArgsType() {
    bool isArray = false;
    for (int i = 0; i < methodDesc_.size(); ++i) {
        auto c = methodDesc_[i];
        switch (c) {
            case L'(':
                break;
            case L')':
                return;

            case L'[':
                isArray = true;
                break;
            case L'Z':    // boolean
            case L'S':    // short
            case L'C':    // char
            case L'I':    // int
            case L'J':    // long
            case L'F':    // float
            case L'D':    // double
                if (isArray) {
                    // primitive array
                    argTypes_.push_back(ValueType::ARRAY);
                    isArray = false;
                } else {
                    argTypes_.push_back(primitiveTypeToValueTypeNoWrap(c));
                }
                break;
            case L'L':
                while (methodDesc_[i] != L';') {
                    ++i; // skip
                }
                if (isArray) {
                    argTypes_.push_back(ValueType::ARRAY);
                    isArray = false;
                } else {
                    argTypes_.push_back(ValueType::OBJECT);
                }
                break;
            default:
                PANIC("should not reach here %c", c);
        }
    }
}

int Method::calcArgsSlotCount() {
    int slotCount = 0;
    for (const auto &item: argTypes_) {
        slotCount++;
        if (item == ValueType::LONG || item == ValueType::DOUBLE) {
            slotCount++;
        }
        if (!isStatic()) {
            slotCount += 1;
        }
    }
    return slotCount;
}

int Method::getArgsSlotCount() const {
    return argsSlotCount_;
}

int Method::getMaxStack() const {
    return maxStack_;
}

int Method::getMaxLocals() const {
    return maxLocals_;
}

CodeReader &Method::getCodeReader() {
    return codeReader_;
}




