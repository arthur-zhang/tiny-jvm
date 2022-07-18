#pragma once

#include "java_thread.h"
#include "classfile/method_info.h"
#include "system_dictionary.h"
#include "instance_oop.h"
#include <ujvm/bytecode/bytecode_interpreter.h>
#include <stack>

class Runtime {
public:
    static void callStaticMethod(JavaThread *javaThread, Method *method) {
        JavaFrame frame = JavaFrame(method);
        frame.setReturnPc(javaThread->pc_);
        javaThread->pushFrame(&frame);

        BytecodeInterpreter::interpret(javaThread);
        javaThread->popFrame();
        javaThread->pc_ = frame.getReturnPc();
    }
};


