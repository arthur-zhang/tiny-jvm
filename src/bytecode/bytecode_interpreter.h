#pragma once
#include "runtime/method.h"
#include "runtime/java_thread.h"

class BytecodeInterpreter {
public:
    static void run(Method *method, JavaThread *javaThread);
};