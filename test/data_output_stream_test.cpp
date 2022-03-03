#include <gtest/gtest.h>

#include <classfile/data_output_stream.hpp>
#include <classfile/const.h>
#include <classfile/classfile_parser.h>

//TEST(test_classfile, DataOutputStream) {
//    DataOutputStream os("/Users/arthur/cvt_dev/clion/tiny-jvm/TestDump.class");
//    os.writeUInt32((u4) JVM_CLASSFILE_MAGIC);
//    os.flush();
//}

TEST(test_classfile, dump_file) {
    ClassFile cf{"/Users/arthur/cvt_dev/clion/tiny-jvm/Test.class"};
    cf.parse();
    cf.dump();
}