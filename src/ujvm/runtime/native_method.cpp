//
// Created by arthur on 2022/7/13.
//

#include "native_method.h"
#include "native_method_pool.h"

NativeMethod *NativeMethod::resolve(Method *method) {
    return NativeMethodPool::get().resolve(method);
}

void *NativeMethod::getNativeSymbol() const {
    return nativeSymbol_;
}
