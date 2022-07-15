#pragma once

#include "ujvm/jni/jni.h"

class Ujvm {
public:
    static JNIENV *getJNIENV() {
        static JNIENV *jnienv = new JNIENV;
        return jnienv;
    }
};

