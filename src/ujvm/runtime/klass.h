#pragma once

#include "shared/jstring.h"

using namespace ujvm;

// Klass 对象表示 Java 类的元数据
class Klass {
public:

    const strings::String &getClassName() const {
        return className_;
    }

protected:
    Klass *superClazz_;
    strings::String packageName_;
    strings::String className_;

};


