//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_CONSTANT_POOL_HPP
#define TINY_JVM_CONSTANT_POOL_HPP

#include <vector>
#include "class_reader.h"
#include "constant.hpp"

using namespace std;

class ConstantPool {
public:
    ConstantPool(ClassReader &reader) {
        constantPoolCount_ = reader.readUInt16();
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

    void dump(DataOutputStream &os) {
        os.writeUInt16(constantPoolCount_);
        for (int i = 0; i < constantPoolCount_ - 1; ++i) {
            Constant *constant = constantPool_[i];
            constant->dump(os);
            if (constant->tag == CONSTANT_Double || constant->tag == CONSTANT_Long) {
                ++i;
            }
        }
    }

private:
    vector<Constant *> constantPool_;
};


#endif //TINY_JVM_CONSTANT_POOL_HPP
