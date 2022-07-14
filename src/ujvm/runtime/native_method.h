#pragma once

class Method;

class NativeMethod {

private:
    Method *method_;
    void *nativeSymbol_;
public:
    NativeMethod(Method *method, void *nativeSymbol) : method_(method), nativeSymbol_(nativeSymbol) {}

    void *getNativeSymbol() const;

    static NativeMethod *resolve(Method *method);
};


