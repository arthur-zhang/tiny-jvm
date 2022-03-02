//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_CLASS_READER_H
#define TINY_JVM_CLASS_READER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "types.h"

class ClassReader {
public:
    ClassReader(const std::string &filePath) : dataStream_(filePath, std::ios::binary) {}

    u2 peek2();

    u1 readUint8();

    u2 readUint16();

    u4 readUint32();

    u1 *readBytes(int n);

private:
    std::ifstream dataStream_;
};


#endif //TINY_JVM_CLASS_READER_H
