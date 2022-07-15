//
// Created by arthur on 2022/6/26.
//

#ifndef TINY_JVM_JAVA_FRAME_H
#define TINY_JVM_JAVA_FRAME_H

#include <stack>
#include <map>
#include "ujvm/class_loader.h"

struct Slot {
public:
    bool isObject = false;
    union {
        int32_t i32;
        void *ref;
    };
};

class SlotArray final {
public:
    explicit SlotArray(size_t size) : size_(size), elements_(nullptr) {
        if (size > 0) {
            elements_ = new Slot[size];
            memset(this->elements_, '\0', sizeof(Slot) * size);
        }
    }

    void setRef(size_t pos, jobject ref) {
        elements_[pos].isObject = true;
        elements_[pos].ref = ref;
    }


    jobject getRef(int pos) {
        return elements_[pos].ref;
    }

    void setInt(size_t pos, int v) {
        elements_[pos].isObject = false;
        elements_[pos].i32 = v;
    }

    int32_t getInt(size_t pos) {
        return elements_[pos].i32;
    }

    jlong getLong(size_t pos) {
        int32_t low = elements_[pos + 1].i32;
        int32_t high = elements_[pos].i32;
        return (((jlong) high) << 32) | low;
    }

    void setLong(size_t pos, jlong v) {
        elements_[pos].i32 = (jint) (v >> 32);
        elements_[pos + 1].i32 = (jint) v;
    }

    virtual ~SlotArray() {
        if (elements_ != nullptr) {
            delete[]elements_;
            this->elements_ = nullptr;
        }
    }

private:
    size_t size_;
    Slot *elements_ = nullptr;
};

class Locals final {
private:
    SlotArray slotArray_;

public:
    explicit Locals(size_t size) : slotArray_(size) {
    }

    SlotArray &getSlotArray() {
        return slotArray_;
    }
};

class Stack final {
private:
    SlotArray slotArray_;
    int pos_;
public:
    explicit Stack(size_t size) : slotArray_(size), pos_(0) {
    }

    void pushInt(int v) {
        slotArray_.setInt(pos_++, v);
    }

    int popInt() {
        return slotArray_.getInt(--pos_);
    }

    jlong popLong() {
        pos_ -= 2;
        return slotArray_.getLong(pos_);
    }

    void pushRef(jobject ref) {
        slotArray_.setRef(pos_++, ref);
    }

    jobject popRef() {
        return slotArray_.getRef(--pos_);
    }

    jobject top() {
        return slotArray_.getRef(pos_ - 1);
    }
};

class JavaFrame {

private:
    Stack operandStack_;
    Locals locals_;
public:
    Stack &getOperandStack() {
        return operandStack_;
    }

    Locals &getLocals() {
        return locals_;
    }

    explicit JavaFrame(int maxLocals, int maxStack) : locals_(maxLocals), operandStack_(maxStack) {}

    static JavaFrame *newFrame(int maxLocals, int maxStack) {
        return new JavaFrame(maxLocals, maxStack);
    }
};


#endif //TINY_JVM_JAVA_FRAME_H
