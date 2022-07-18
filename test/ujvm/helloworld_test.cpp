#include <gtest/gtest.h>

#include "classfile/data_output_stream.h"
#include "shared/const.h"
#include "classfile/classfile_parser.h"
#include <filesystem>
#include <ujvm/runtime/system_dictionary.h>
#include <ujvm/runtime/thread.h>
#include <ujvm/bytecode/bytecode_interpreter.h>
#include <ujvm/runtime/main_thread.h>

namespace fs = std::filesystem;
using namespace ujvm;


MethodInfo *findMainMethod(ClassFile *cf) {
    auto mc = cf->methods_count;
    for (int i = 0; i < mc; i++) {
        auto cp = dynamic_cast<CONSTANT_Utf8_info *>(cf->constantPool->getConstantPool()[(cf->methods[i]->name_index)]);
        std::cout << cp->bytes << std::endl;
        if (cp->getConstant() == L"main") {
            return cf->methods[i];
        }
    }
    return nullptr;
}


TEST(test_hellowold, class_read_test) {



    MainThread mainThread(L"MyTest");
    Threads::currentThread = &mainThread;

    mainThread.start();


//    delete frame;
}
