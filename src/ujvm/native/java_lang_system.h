#pragma once

#include "types.h"
#include "jstring.h"
#include <shared/types.h>
#include "ujvm/jni/jni.h"

using namespace ujvm;

extern "C" {
void Java_java_lang_System_exitInternal(JNIENV *env, jclass java_lang_System, jint status);

}

void *java_lang_system_search_method(strings::String &signature);