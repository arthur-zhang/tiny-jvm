#include <gtest/gtest.h>

#include <ujvm/classfile/data_output_stream.h>
#include <ujvm/classfile/const.h>
#include <ujvm/classfile/classfile_parser.h>
#include <filesystem>
#include "ujvm/classpath/system_dictionary.h"
#include <ujvm/runtime/thread.h>
#include "ujvm/runtime/bytecode_interpreter.h"

namespace fs = std::filesystem;
//TEST(test_classfile, DataOutputStream) {
//    DataOutputStream os("/Users/arthur/cvt_dev/clion/tiny-jvm/TestDump.class");
//    os.writeUInt32((u4) JVM_CLASSFILE_MAGIC);
//    os.flush();
//}
using namespace ujvm;

u1 *getBytes(string &str) {
    size_t len = str.length();
    u1 *bytes = new u1[len];
    std::copy(str.begin(), str.end(), bytes);
    return bytes;
}


TEST(test_classfile, mutf8_test) {

    u1 bytes[] = {104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 33};
    strings::String s = decodeMUTF8(bytes, std::size(bytes));
    wcout << "s:" << s << endl;
    u1 bytes2[] = {
            104, 101, 108, 108, 111, 32, 119, 111,
            114, 108, 100, 33, 230, 140, 150, 229,
            157, 145, 231, 154, 132, 229, 188, 160,
            229, 184, 136, 229, 130, 133
    };
    cout << "str: " << strings::toStdString(decodeMUTF8(bytes2, std::size(bytes2))) << endl;
}

TEST(test_classfile, dump_file) {
    ClassFile cf{"/Users/arthur/cvt_dev/clion/tiny-jvm/Test.class"};
    cf.parse();
    cf.dump("/Users/arthur/cvt_dev/clion/tiny-jvm/TestDump.class");
}

bool compare_files(const std::string classFilePath, const std::string dumpFile) {

    std::ifstream input1(classFilePath, std::ios::binary);
    std::ifstream input2(dumpFile, std::ios::binary);

    std::vector<char> bytes1(
            (std::istreambuf_iterator<char>(input1)),
            (std::istreambuf_iterator<char>()));
    std::vector<char> bytes2(
            (std::istreambuf_iterator<char>(input2)),
            (std::istreambuf_iterator<char>()));
    size_t size1 = bytes1.size();
    size_t size2 = bytes2.size();
    if (size1 != size2) {
        cout << "size not equal " << size1 << ":" << size2 << " path: " << classFilePath << endl;
        return false;
    }
    for (int i = 0; i < size1; ++i) {
        if (bytes1[i] != bytes2[i]) {
            cout << "not equal" << bytes1[i] << ":" << bytes2[i] << " path: " << classFilePath << endl;
            return false;
        }
    }
    return true;
}

TEST(test_classfile, list_dir) {
    std::string classesDirPath = "/Users/arthur/cvt_dev/jvm/wind_jvm/sun_src";
    for (const auto &entry: fs::recursive_directory_iterator(classesDirPath)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.is_directory()) continue;

        if (!(entry.path().filename().extension().string() == ".class")) {
            continue;
        }
        const string classFilePath = entry.path().string();

        std::string dumpFile = "/Users/arthur/cvt_dev/clion/tiny-jvm/" + entry.path().filename().string();

        ClassFile cf(classFilePath);
        cf.parse();
        cf.dump(dumpFile);

        if (!compare_files(classFilePath, dumpFile)) {
            cout << "not equal, path: " << classFilePath << endl;
            break;
        }
        std::filesystem::remove(dumpFile);
    }
    cout << "all done";
}


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


TEST(test_classfile, class_read_test) {

    JavaThread *javaThread = new JavaThread;
//    Threads::currentThread = javaThread;
    Threads::currentThread = javaThread;


    BootstrapClassLoader::get()->loadClassByName(L"MyTest");
    BootstrapClassLoader::get()->loadClassByName(L"java/lang/System");
    BootstrapClassLoader::get()->loadClassByName(L"java/io/PrintStream");

    InstanceClassStruct *clz = SystemDictionary::get()->find(L"MyTest");

    ClassFile *cf = clz->getClassFile();

    auto methodInfo = findMainMethod(cf);

    BytecodeInterpreter::run(clz, new Method(cf->constantPool, methodInfo), javaThread);
//    std::cout << "this class:" << cf->this_class << std::endl;
//    Constant *cp = cf->constantPool->getConstantPool()[cf->this_class];
//    std::cout << "tag: " << (int) (cp->tag) << "#" << std::endl;
//    ASSERT_EQ(cp->tag, CONSTANT_Class);
//    auto idx = dynamic_cast<CONSTANT_Class_info *>(cp)->index;
//    std::cout << "idx: " << idx << std::endl;
//    auto cp2 = dynamic_cast<CONSTANT_Utf8_info *>(cf->constantPool->getConstantPool()[idx]);
//    auto className = cp2->bytes;
//    std::cout << "className :" << className << std::endl;




//    std::cout << codeAttr->attribute_name_index << std::endl;


}
