
#include "java_lang_system.h"

void Java_java_lang_System_exitInternal(JNIENV *env, jclass java_lang_System, jint status) {
    exit(status);
}
