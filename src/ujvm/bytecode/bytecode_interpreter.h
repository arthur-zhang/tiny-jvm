#pragma once

#include <ujvm/runtime/method.h>
#include <ujvm/runtime/java_thread.h>

class BytecodeInterpreter {
public:
    static void interpret(Method *method, JavaThread *javaThread);
};