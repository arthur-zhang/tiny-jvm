//
// Created by arthur on 2022/7/14.
//

#include <iostream>
#include "java_io_printstream.h"
#include "runtime/oop_desc.h"


void Java_java_io_PrintStream_printlnNative(jobject env, jobject jobj, jobject str) {
    auto oop = (InstanceOopDesc *) str;
    strings::String *ptr = ((strings::String *) oop->getRef());
    std::cout << "hello, printme" << std::endl;
    std::wcout << *ptr << std::endl;
}
