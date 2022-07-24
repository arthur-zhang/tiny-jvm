//
// Created by arthur on 2022/7/18.
//

#pragma once

#include "java_thread.h"
#include <ujvm/runtime/system_dictionary.h>
#include <ujvm/bytecode/bytecode_interpreter.h>

class MainThread : public JavaThread {
public:
    MainThread(const strings::String &mainClassName) : mainClassName_(mainClassName) {
    }

    void start() override {
        BootstrapClassLoader::get()->loadClassByName(L"MyTest");
        BootstrapClassLoader::get()->loadClassByName(L"java/lang/Object");
        BootstrapClassLoader::get()->loadClassByName(L"java/lang/System");
        BootstrapClassLoader::get()->loadClassByName(L"java/io/PrintStream");
        InstanceKlass *clz = SystemDictionary::get()->find(L"MyTest");
        Method *mainMethod = clz->findMethod(L"main", L"([Ljava/lang/String;)V");
        if (!mainMethod) PANIC("main method not found");
        mainMethod_ = mainMethod;
        this->pushFrame(mainMethod_);
        BytecodeInterpreter::interpret(this);
    }

private:
    Method *mainMethod_;
    const strings::String mainClassName_;

};
