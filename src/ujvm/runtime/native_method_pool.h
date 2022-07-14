#pragma once

#include "shared/jstring.h"
#include "java_frame.h"
#include <map>
#include <dlfcn.h>
#include "method.h"
#include "native_method.h"

class NativeMethodPool {
public:
    void putMethod(Method *method);

    static NativeMethodPool &get() {
        static NativeMethodPool pool;
        return pool;
    }

    NativeMethod *resolve(Method *method) {
        auto methodKey = method->getClazz()->getClassName() + method->getMethodName() + method->getMethodDesc();
        auto it = nativeMethodMap_.find(methodKey);
        if (it != nativeMethodMap_.end()) {
            return it->second;
        }
//        JNIEXPORT jint JNICALL Java_java_lang_System_identityHashCode(JNIEnv *env, jobject this, jobject x)
//        {
//            return JVM_IHashCode(env, x);
//        }
        strings::String nativeMethodName =
                L"Java_" + strings::replaceAll(method->getClazz()->getClassName(), L"/", L"_")
                + L"_" + method->getMethodName();


        void *symbol = dlsym(RTLD_DEFAULT, strings::toStdString(nativeMethodName).c_str());
        if (symbol != nullptr) {
            auto nativeMethod = new NativeMethod(method, symbol);
            nativeMethodMap_[methodKey] = nativeMethod;
            return nativeMethod;
        }
        return nullptr;
    }

private:
    map<strings::String, NativeMethod *> nativeMethodMap_;


};


