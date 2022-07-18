//
// Created by arthur on 2022/7/18.
//

#pragma once

#include <stack>
#include "java_frame.h"

class FrameList {
public:
    void push(JavaFrame *frame) {
        stackFrame.push(frame);
    }

    JavaFrame *pop() {
        JavaFrame *ret = stackFrame.top();
        stackFrame.pop();
        return ret;
    }

    JavaFrame *top() {
        return stackFrame.top();
    }

private:
    std::stack<JavaFrame *> stackFrame;

};
