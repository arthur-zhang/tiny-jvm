#pragma once

#include <map>
#include "shared/types.h"
#include "classfile/classfile_parser.h"
#include <ujvm/runtime/instance_class.h>


//namespace ujvm {


//todo
class InstanceKlass;

class BootstrapClassLoader {
public:
    static BootstrapClassLoader *get() {
        static BootstrapClassLoader loader;
        return &loader;
    }

    InstanceKlass *loadClass(u1 *classBytes, size_t classSize);

    InstanceKlass *loadClassByName(const strings::String &name);
};
//}
