#pragma once

#include <memory>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>

#include "tiny_jvm.h"
#include "const.h"
#include "types.hpp"
#include "String.h"
#include "constant_pool.hpp"
#include "constant.hpp"
#include "field_info.h"
#include "method_info.h"

using namespace std;

class ClassFile {
private:
    u1 *bytes;
    int size;

public:
    ClassFile(const std::string &filePath) {
        std::ifstream ifs(filePath, ios::in | ios::ate | ios::binary);

        size = ifs.tellg();
        ifs.seekg(0);

        bytes = new u1[size];
        ifs.read((char *) &bytes[0], size);
        ifs.close();

        reader.init(bytes, size);
    }

    ~ClassFile() {
        if (size > 0) delete[]bytes;
        delete constantPool;
        if (interfaces_count > 0) delete[]interfaces;
        if (fields_count > 0) {
            for (int i = 0; i < fields_count; ++i) {
                delete fields[i];
            }
            delete fields;
        }
        if (methods_count > 0) {

            for (int i = 0; i < methods_count; ++i) {
                delete methods[i];
            }
            delete methods;
        }
        if (attributes_count > 0) {
            for (int i = 0; i < attributes_count; ++i) {
                delete attributes[i];
            }
            delete attributes;
        }
    }

    u4 magic = 0;
    u2 minor_version = 0;
    u2 major_version = 0;
    ConstantPool *constantPool;
    u2 access_flags = 0;
    u2 this_class = 0;
    u2 super_class = 0;
    u2 interfaces_count = 0;
    u2 *interfaces = nullptr;                // [interfaces_count]
    u2 fields_count = 0;

    FieldInfo **fields = nullptr;            // [fields_count]
    u2 methods_count = 0;
    MethodInfo **methods = nullptr;            // [methods_count];
    u2 attributes_count = 0;
    AttributeInfo **attributes = nullptr;    // [attributes_count];
    void parse();

    void dump(const string &outputPath);

private:
    ClassReader reader;

    void readMagic();

    void readVersion();

    void readConstantPool();

    void readClassInfo();

    void readInterfaces();

    void readFields();

    void readMethods();

    void readAttributes();

};