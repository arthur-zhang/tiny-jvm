#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "const.h"
#include "types.hpp"
#include "String.h"
#include "constant_pool.hpp"
#include "constant.hpp"
#include "field_info.h"
#include "method_info.h"

using namespace std;


class ClassFile {
public:
    ClassFile(const std::string &filePath) : reader(filePath) {
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

    void dump();

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


void print_methods(MethodInfo **bufs, int length, ConstantPool *constant_pool);

