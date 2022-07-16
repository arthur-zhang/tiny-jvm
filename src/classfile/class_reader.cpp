#include "classfile/class_reader.h"

#define READ_U2(v, p)  v = ((p)[0]<<8)|(p)[1];
#define READ_U4(v, p)  v = ((p)[0]<<24)|((p)[1]<<16)|((p)[2]<<8)|(p)[3];

u1 ClassReader::peek1() {
    return *cur_;
}

u2 ClassReader::peek2() {
    u2 ret;
    READ_U2(ret, cur_);
    return ret;
}

u1 ClassReader::readUint8() {
    return *cur_++;
}

u2 ClassReader::readUInt16() {
    u2 ret;
    READ_U2(ret, cur_);
    cur_ += 2;
    return ret;
}

u4 ClassReader::readUInt32() {
    u4 ret;
    READ_U4(ret, cur_);
    cur_ += 4;
    return ret;
}

u1 *ClassReader::readBytes(int n) {
    u1 *to = new u1[n];
    u1 *ret = to;
    u1 *from = cur_;
    for (int i = 0; i < n; ++i) {
        *to++ = *from++;
    }
    cur_ += n;
    return ret;
}
