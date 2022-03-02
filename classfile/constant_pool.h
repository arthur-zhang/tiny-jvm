//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_CONSTANT_POOL_H
#define TINY_JVM_CONSTANT_POOL_H

#include <vector>
#include "class_reader.h"
#include "constant.h"

using namespace std;

class ConstantPool {
public:
    ConstantPool(ClassReader &reader) {
        constantPoolCount_ = reader.readUint16();
        constantPool_ = vector<Constant *>(constantPoolCount_);
        for (int i = 0; i < constantPoolCount_ - 1; ++i) {
            constantPool_[i] = readConstant(reader);
            if (constantPool_[i]->tag == CONSTANT_Double || constantPool_[i]->tag == CONSTANT_Long) {
                ++i;
            }
        }
    }

    u2 constantPoolCount_;

    const vector<Constant *> &getConstantPool() const {
        return constantPool_;
    }

private:
    vector<Constant *> constantPool_;
};


#endif //TINY_JVM_CONSTANT_POOL_H
