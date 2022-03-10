//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_CLASS_READER_H
#define TINY_JVM_CLASS_READER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "shared/types.h"

class ClassReader {
private:
    u1 *buffer_start_ = nullptr;
    u1 *buffer_end_ = nullptr;
    u1 *cur_ = nullptr;
public:
    ClassReader() {}


    void init(u1 *bytes, size_t length) {
        buffer_start_ = bytes;
        buffer_end_ = buffer_start_ + length;
        cur_ = buffer_start_;
    }

    u1 peek1();

    u2 peek2();

    u1 readUint8();

    u2 readUInt16();

    u4 readUInt32();

    u1 *readBytes(int n);

};


#endif //TINY_JVM_CLASS_READER_H
