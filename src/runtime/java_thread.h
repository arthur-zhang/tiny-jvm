//
// Created by arthur on 2022/6/26.
//

#ifndef TINY_JVM_JAVA_THREAD_H
#define TINY_JVM_JAVA_THREAD_H

#include <stack>
#include "java_frame.h"

class JavaThread {
public:
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

private:
    std::stack<JavaFrame *> stackFrame;
};


#endif //TINY_JVM_JAVA_THREAD_H
