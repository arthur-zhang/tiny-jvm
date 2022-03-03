#include "class_reader.h"

u1 ClassReader::peek1() {
    u1 first = dataStream_.peek();
    return first;
}

u2 ClassReader::peek2() {
    u1 first = dataStream_.get();
    u1 second = dataStream_.peek();
    dataStream_.unget();
    return ((first << 8) + second) & 0xFFFF;
}

u1 ClassReader::readUint8() {
    u1 result;
    dataStream_.read((char *) &result, sizeof(u1));
    return result;
}

u2 ClassReader::readUInt16() {
    u2 result;
    dataStream_.read((char *) &result, sizeof(u2));
    return htons(result);
}

u4 ClassReader::readUInt32() {
    u4 result;
    dataStream_.read((char *) &result, sizeof(u4));
    return htonl(result);
}

u1 *ClassReader::readBytes(int n) {
    u1 *bytes = new u1[n];
    for (int i = 0; i < n; ++i) {
        bytes[i] = readUint8();
    }
    return bytes;
}
