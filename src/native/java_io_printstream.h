#pragma once

#include "shared/types.h"
#include "jni/jni.h"

extern "C" {
void Java_java_io_PrintStream_printlnNative(jobject env, jobject jobj, jobject str);
}