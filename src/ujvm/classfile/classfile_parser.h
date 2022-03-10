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
#include "shared/types.h"
#include "shared/string.h"
#include "constant_pool.h"
#include "constant.h"
#include "field_info.h"
#include "method_info.h"

using namespace std;

namespace ujvm {

    class ClassFile final {

    public:
        ClassFile(const std::string &filePath);

        ~ClassFile();

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
        u1 *bytes_ = nullptr;
        size_t size_;
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
}
