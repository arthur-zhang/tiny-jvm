#include <gtest/gtest.h>

#include <classfile/data_output_stream.hpp>
#include <classfile/const.h>
#include <classfile/classfile_parser.h>
#include <filesystem>

namespace fs = std::filesystem;
//TEST(test_classfile, DataOutputStream) {
//    DataOutputStream os("/Users/arthur/cvt_dev/clion/tiny-jvm/TestDump.class");
//    os.writeUInt32((u4) JVM_CLASSFILE_MAGIC);
//    os.flush();
//}

TEST(test_classfile, dump_file) {
    ClassFile cf{"/Users/arthur/cvt_dev/clion/tiny-jvm/Test.class"};
    cf.parse();
    cf.dump("/Users/arthur/cvt_dev/clion/tiny-jvm/TestDump.class");
}

template<typename InputIterator1, typename InputIterator2>
bool
range_equal(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 != *first2) {
            cout << *first1 << ":::" << *first2 << endl;
            return false;
        }
        ++first1;
        ++first2;
    }
    return (first1 == last1) && (first2 == last2);
}

bool compare_files(const std::string &filename1, const std::string &filename2) {
    std::ifstream file1(filename1);
    std::ifstream file2(filename2);

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    std::istreambuf_iterator<char> end;

    return range_equal(begin1, end, begin2, end);
}

TEST(test_classfile, list_dir) {
    std::string classesDirPath = "/Users/arthur/cvt_dev/jvm/wind_jvm/sun_src";
    int i = 0;
    for (const auto &entry: fs::recursive_directory_iterator(classesDirPath)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.is_directory()) continue;

        const string &path = entry.path();
        if (!path.ends_with(".class")) {
            continue;
        }
//        if (!path.ends_with("DesktopPanePainter.class")) {
//            continue;
//        }

        cout << "path:" << path << endl;
        ClassFile cf(path);
        cf.parse();
        string dumpFile = "/Users/arthur/cvt_dev/clion/tiny-jvm/TestDump.class";
        cf.dump(dumpFile);
        if (!compare_files(path, dumpFile)) {
            cout << "not equal, path: " << path;
            break;
        } else {
            cout << "is equal" << endl;
        }
    }
    cout << "all done";
}