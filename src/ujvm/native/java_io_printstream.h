#pragma once

#include "shared/types.h"
#include <ujvm/jni/jni.h>

extern "C" {
void Java_java_io_PrintStream_printlnNative(jobject env, jobject jobj, jobject str);
void Java_java_io_PrintStream_printlnNativeLong(jobject env, jobject jobj, jlong v);
}