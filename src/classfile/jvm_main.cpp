//
// Created by ya on 2022/2/28.
//

#include <iostream>
#include <fstream>
#include "classfile/classfile_parser.h"
#include <filesystem>

using namespace std;

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

#include <execinfo.h>
#include <signal.h>

namespace fs = std::filesystem;

void error_handler(int sig) {
    void *array[10];
    size_t size;
    size = backtrace(array, 10); //get the void pointers for all of the entries
    cout << "Error: signal " << sig << ":\n"; //display error signal
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main() {
    signal(SIGSEGV, error_handler);

    std::string classesDirPath = "/Users/arthur/cvt_dev/jvm/wind_jvm/sun_src";
    for (const auto &entry: fs::recursive_directory_iterator(classesDirPath)) {
        std::cout << "entry: " << entry.path() << std::endl;
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.is_directory()) continue;

        if (!(entry.path().filename().extension().string() == ".class")) {
            continue;
        }
        const string classFilePath = entry.path().string();
//        if (!classFilePath.ends_with("AppleScriptEngine.class")) {
//            continue;
//        }

        std::string dumpFile = "/Users/arthur/cvt_dev/clion/tiny-jvm/" + entry.path().filename().string();

        cout << classFilePath << endl;
        ClassFile cf(classFilePath);
        cf.parse();
        cf.dump(dumpFile);

        cout << "dump done" << endl;
        if (!compare_files(classFilePath, dumpFile)) {
            cout << "not equal, path: " << classFilePath << endl;
            break;
        }
        std::filesystem::remove(dumpFile);
    }
    cout << "all done";
    return 0;
}

int main2() {
    ujvm::ClassFile classFile(
            "/Users/arthur/cvt_dev/clion/tiny-jvm/Test.class");
    classFile.parse();
}