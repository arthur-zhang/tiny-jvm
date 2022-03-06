#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "types.hpp"

using namespace std;

class DataOutputStream {
public:
    void writeUInt32(u4 value) {
        writeUInt8((value >> 24) & 0xFF);
        writeUInt8((value >> 16) & 0xFF);
        writeUInt8((value >> 8) & 0xFF);
        writeUInt8(value & 0xFF);
    }

    void writeUInt16(u2 value) {
        writeUInt8(((value) >> 8) & 0xFF);
        writeUInt8(value & 0xFF);
    }

    void writeUInt8(u1 value) {
        stream_->write(reinterpret_cast<const char *>(&value), 1);
    }

    void writeBytes(const u1 *bytes, u4 length) {
        for (int i = 0; i < length; ++i) {
            writeUInt8(bytes[i]);
        }
    }

    void flush() {
        stream_->flush();
        stream_->close();
    }

    DataOutputStream(const string &inputPath) {
        stream_ = new std::ofstream(inputPath, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
    }

private:
    std::ofstream *stream_ = nullptr;
};