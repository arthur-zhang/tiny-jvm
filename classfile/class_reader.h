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

    u2 peek2() {
        u1 first = dataStream_.get();
        u1 second = dataStream_.peek();
        dataStream_.unget();
        return ((first << 8) + second) & 0xFFFF;
    }

    u1 readUint8() {
        u1 result;
        dataStream_.read((char *) &result, sizeof(u1));
        return result;
    }

    u2 readUint16() {
        u2 result;
        dataStream_.read((char *) &result, sizeof(u2));
        return htons(result);
    }

    u4 readUint32() {
        u4 result;
        dataStream_.read((char *) &result, sizeof(u4));
        return htonl(result);
    }

    u1 *readBytes(int n) {
        u1 *bytes = new u1[n];
        for (int i = 0; i < n; ++i) {
            bytes[i] = readUint8();
        }
        return bytes;
    }

    void readUTF(unsigned char *bytes, int length) {
        dataStream_.read((char *) bytes, length);
    }
private:
    std::ifstream dataStream_;
};


#endif //TINY_JVM_CLASS_READER_H
