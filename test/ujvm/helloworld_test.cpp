#include <gtest/gtest.h>

#include "classfile/data_output_stream.h"
#include "shared/const.h"
#include "classfile/classfile_parser.h"
#include <filesystem>
#include "classpath/system_dictionary.h"
#include "runtime/thread.h"
#include "bytecode/bytecode_interpreter.h"

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

    JavaThread *javaThread = new JavaThread;
//    Threads::currentThread = javaThread;
    Threads::currentThread = javaThread;
    BootstrapClassLoader::get()->loadClassByName(L"MyTest");
    BootstrapClassLoader::get()->loadClassByName(L"java/lang/Object");
    BootstrapClassLoader::get()->loadClassByName(L"java/lang/System");
    BootstrapClassLoader::get()->loadClassByName(L"java/io/PrintStream");
    InstanceClassStruct *clz = SystemDictionary::get()->find(L"MyTest");
    ClassFile *cf = clz->getClassFile();
    auto methodInfo = findMainMethod(cf);
    JavaFrame *frame = new JavaFrame(methodInfo->getCode()->max_locals, methodInfo->getCode()->max_stack);
    javaThread->pushFrame(frame);
    BytecodeInterpreter::run(new Method(clz, methodInfo), javaThread);
//    delete frame;
}
