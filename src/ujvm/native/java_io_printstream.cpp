//
// Created by arthur on 2022/7/14.
//

#include <iostream>
#include "java_io_printstream.h"
#include <ujvm/runtime/oop_desc.h>

void Java_java_io_PrintStream_printlnNative(jobject env, jobject jobj, jobject str) {
    auto oopDesc = (InstanceOopDesc *) str;
    strings::String *ptr = ((strings::String *) oopDesc->getRef());
    std::cout << "hello, printme" << std::endl;
    std::wcout << *ptr << std::endl;
}

void Java_java_io_PrintStream_printlnNativeLong(jobject env, jobject jobj, jlong value) {
    std::wcout << "println: " << value << std::endl;
}
