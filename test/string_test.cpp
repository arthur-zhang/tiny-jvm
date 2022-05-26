#include <shared/jstring.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace ujvm;


TEST(test_ujvm, string_replace) {
    strings::String str = L"aabbccddaa";
//    std::wcout << strings::replaceAll(str, L".i", L"/") << std::endl;
    strings::String ret = strings::replaceAll(str, L"b", L"");
    std::wcout << ret << std::endl;
    ret = strings::replaceAll(str, L"b", L"xx");
    std::wcout << ret << std::endl;
    ret = strings::replaceAll(str, L"aa", L"A");
    std::wcout << ret << std::endl;
}
