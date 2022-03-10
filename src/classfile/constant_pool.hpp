#pragma once

#include <vector>
#include "class_reader.h"
#include "constant.hpp"

using namespace std;

class ConstantPool final {
public:
    ConstantPool(ClassReader &reader) {
        constant_pool_count_ = reader.readUInt16();
        constant_pool_ = new Constant *[constant_pool_count_];
        for (int i = 1; i < constant_pool_count_; ++i) {
            constant_pool_[i] = readConstant(reader);
            if (constant_pool_[i]->tag == CONSTANT_Double || constant_pool_[i]->tag == CONSTANT_Long) {
                ++i;
            }
        }
    }

    virtual ~ConstantPool() {
        if (constant_pool_count_ <= 0) return;
        for (int i = 1; i < constant_pool_count_; ++i) {
            u1 tag = constant_pool_[i]->tag;
            delete constant_pool_[i];
            if (tag == CONSTANT_Double || tag == CONSTANT_Long) {
                ++i;
            }
        }
        delete[]constant_pool_;
    }

    u2 constant_pool_count_;

    Constant **getConstantPool() const {
        return constant_pool_;
    }


    void dump(DataOutputStream &os) {
        os.writeUInt16(constant_pool_count_);
        for (int i = 1; i < constant_pool_count_; ++i) {
            Constant *constant = constant_pool_[i];
            constant->dump(os);
            if (constant->tag == CONSTANT_Double || constant->tag == CONSTANT_Long) {
                ++i;
            }
        }
    }

private:
    Constant **constant_pool_ = nullptr;
};


