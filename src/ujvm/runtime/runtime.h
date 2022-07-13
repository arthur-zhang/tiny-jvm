#pragma once

#include "java_thread.h"
#include "ujvm/classfile/method_info.h"
#include "ujvm/classpath/system_dictionary.h"
#include "instance_oop.h"
#include "bytecode_interpreter.h"
#include <stack>

class Runtime {
public:
    static void callStaticMethod(InstanceClassStruct *instanceClassStruct, JavaThread *javaThread,
                                 Method *method) {
        JavaFrame *frame = new JavaFrame(method->getCode()->max_locals, method->getCode()->max_stack);
        javaThread->stackFrame.push(frame);
        BytecodeInterpreter::run(instanceClassStruct, method, javaThread);
        javaThread->stackFrame.pop();
        delete frame;
    }
};


