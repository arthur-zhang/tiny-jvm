#pragma once
#include "ujvm/runtime/method.h"
#include "ujvm/runtime/java_thread.h"

class BytecodeInterpreter {
public:
    static void run(Method *method, JavaThread *javaThread);
};