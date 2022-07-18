#pragma once

#include "jni/jni.h"

class Ujvm {
public:
    static JNIENV *getJNIENV() {
        static JNIENV *jnienv = new JNIENV;
        return jnienv;
    }
};

