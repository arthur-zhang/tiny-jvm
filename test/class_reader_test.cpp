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
using namespace ujvm;

TEST(test_classfile, dump_file) {
    ClassFile cf{"/Users/arthur/cvt_dev/clion/tiny-jvm/Test.class"};
    cf.parse();
    cf.dump("/Users/arthur/cvt_dev/clion/tiny-jvm/TestDump.class");
}

template<typename InputIterator1, typename InputIterator2>
bool
range_equal(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2) {
    int counter = 0;
    while (first1 != last1 && first2 != last2) {
        if (*first1 != *first2) {
            cout << "Not equal: " << counter << ": " << *first1 << ":::" << *first2 << endl;
            return false;
        }
        counter++;
        ++first1;
        ++first2;
    }
    bool ret = (first1 == last1) && (first2 == last2);
    if (!ret) {
        cout << "Not equal2: " << (first1 == last1) << ": " << *first1 << ": " << (first2 == last2) << endl;
    }
    return ret;
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

bool compare_files2(const std::string originFile, const std::string targetFile) {

    std::ifstream targetFileStream(targetFile, std::ios::ate | std::ios::in | std::ios::binary);
    auto file2Size = targetFileStream.tellg();

    std::ifstream originFileStream(originFile, std::ios::ate | std::ios::in | std::ios::binary);
    auto file1Size = originFileStream.tellg();


    if (file1Size != file2Size) {
        cout << " " << "filesize not equal" << file1Size << ":" << file2Size
             << "   " << targetFile << ">>>>" << endl;
        cout << targetFileStream.good() << " " << targetFileStream.bad() << " " << targetFileStream.eof() << " "
             << endl;
        return false;
    }
//    auto size1 = std::filesystem::file_size(filename1);
//    auto size2 = std::filesystem::file_size(filename2);

//    if (size1 != size2) {
//        cout << " " << "filesize not equal" << file1Size << ":" << file2Size
//             << "   " << filename2 << ">>>>" << size1 << ": " << size2 << endl;
//        return false;
//    }
    originFileStream.seekg(0);
    targetFileStream.seekg(0);

    std::istreambuf_iterator<char> begin1(originFileStream);
    std::istreambuf_iterator<char> begin2(targetFileStream);

    std::istreambuf_iterator<char> end;

    return std::equal(begin1, end, begin2);
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