#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "shared/types.h"

using namespace std;

class DataOutputStream {
public:
    explicit DataOutputStream(const string &inputPath) {
        stream_ = std::ofstream(inputPath, std::ios::out | std::ios::binary | std::ios::trunc);
    }

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
        stream_.write(reinterpret_cast<const char *>(&value), 1);
    }

    void writeBytes(const u1 *bytes, u4 length) {
        for (int i = 0; i < length; ++i) {
            writeUInt8(bytes[i]);
        }
    }

    void flush() {
        stream_.flush();
        stream_.close();
    }


    ~DataOutputStream() {
        stream_.flush();
        stream_.close();
    }

private:
    std::ofstream stream_;
};