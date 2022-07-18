#pragma once

#include <stack>
#include "java_frame.h"
#include "frame_list.h"

class JavaThread {
public:
    void pushFrame(JavaFrame *frame) {
        stackFrame.push(frame);
    }

    void pushFrame(Method *method) {
        stackFrame.push(JavaFrame::newFrame(method));
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
    FrameList stackFrame;
};
