//
// Created by ya on 2022/2/28.
//

#include <iostream>
#include <fstream>
#include "classfile/classfile_parser.h"

using namespace std;

int main() {
    ujvm::ClassFile classFile(
            "/Users/arthur/cvt_dev/clion/tiny-jvm/Test.class");
    classFile.parse();
}