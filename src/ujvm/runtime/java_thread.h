//
// Created by arthur on 2022/6/26.
//

#ifndef TINY_JVM_JAVA_THREAD_H
#define TINY_JVM_JAVA_THREAD_H

#include <stack>
#include "java_frame.h"

class JavaThread {
public:
    std::stack<JavaFrame *> stackFrame;

    JavaThread() {
    }

    void pushFrame(JavaFrame *frame) {
        stackFrame.push(frame);
    }

    JavaFrame *popFrame() {
        JavaFrame *ret = stackFrame.top();
        stackFrame.pop();
        return ret;
    }

    JavaFrame *currentFrame() {
        return stackFrame.top();
    }
};


#endif //TINY_JVM_JAVA_THREAD_H
