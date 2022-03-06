//
// Created by ya on 2022/2/28.
//

#include <iostream>
#include <fstream>
#include "classfile/classfile_parser.h"

using namespace std;

int main() {
    ClassFile classFile("/Users/arthur/cvt_dev/jvm/wind_jvm/sun_src/sun/misc/Contended.class");
    classFile.parse();
}

int main2() {
//    ClassFile classFile;
//    std::ifstream f("/Users/arthur/cvt_dev/clion/tiny-jvm/Test.class", std::ios::binary);

//    f >> classFile;

//    int methodCount = classFile.methods_count;
//    for (int i = 0; i < methodCount; ++i) {
//        method_info method = classFile.methods[i];
//        int nameIndex = method.name_index;
//        std::wstring methodName = ((CONSTANT_Utf8_info *) classFile.constant_pool[nameIndex - 1])->convert_to_Unicode();
//        wcout << methodName << endl;
//    }
//    for (int i = 0; i < methodCount; ++i) {
//        method_info method = classFile.methods[i];
//        int nameIndex = method.name_index;

//    print_methods(classFile.methods, methodCount, classFile.constant_pool);

//        std::wstring methodName = ((CONSTANT_Utf8_info *) classFile.constant_pool[nameIndex])->convert_to_Unicode();
//        wcout << methodName << endl;
//    }

//    getchar();
    return 0;
}
