#pragma once

#include <stack>
#include "java_frame.h"
#include "frame_list.h"

class JavaThread {
public:
    void pushFrame(JavaFrame *frame);

    void pushFrame(Method *method);

    JavaFrame *popFrame();

    JavaFrame *currentFrame();

    virtual void start();

    u4 pc_ = 0;

private:
    FrameList stackFrame;
};
