#pragma once

#include "java_thread.h"
#include "classfile/method_info.h"
#include "classpath/system_dictionary.h"
#include "instance_oop.h"
#include "bytecode/bytecode_interpreter.h"
#include <stack>

class Runtime {
public:
    static void callStaticMethod(InstanceKlass *instanceClassStruct, JavaThread *javaThread,
                                 Method *method) {
        JavaFrame *frame = new JavaFrame(method->getCode()->max_locals, method->getCode()->max_stack);
        javaThread->pushFrame(frame);
        BytecodeInterpreter::run(method, javaThread);
//        javaThread->popFrame();
//        delete frame;
    }
};


