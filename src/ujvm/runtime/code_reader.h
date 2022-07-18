#pragma once

#include <shared/types.h>

class CodeReader {
public:
    CodeReader() : data_(nullptr), codeLen_(0) {
    }

    CodeReader(u1 *data, u4 codeLen) : data_(data), codeLen_(codeLen) {}

    u1 readU1(u4 &pc) {
        return data_[pc++];
    }

    u2 readU2(u4 &pc) {
        u2 ret = (data_[pc] << 8) + data_[pc + 1];
        pc += 2;
        return ret;
    }

    bool ended(u4 &pc) const {
        return pc >= codeLen_;
    }

private:
    u1 *data_;
    u4 codeLen_;
};