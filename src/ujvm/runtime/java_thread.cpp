//
// Created by arthur on 2022/6/26.
//

#include "java_thread.h"

void JavaThread::pushFrame(JavaFrame *frame) {
    stackFrame.push(frame);
}

void JavaThread::pushFrame(Method *method) {
    stackFrame.push(JavaFrame::newFrame(method));
}

JavaFrame *JavaThread::popFrame() {
    JavaFrame *ret = stackFrame.top();
    stackFrame.pop();
    return ret;
}

JavaFrame *JavaThread::currentFrame() {
    return stackFrame.top();
}

void JavaThread::start() {

}
