#pragma once

#include <map>
#include "shared/types.h"
#include "ujvm/classfile/classfile_parser.h"
#include "ujvm/runtime/instance_class.h"

//namespace ujvm {


//todo
class InstanceClassStruct;

class BootstrapClassLoader {
public:
    static BootstrapClassLoader *get() {
        static BootstrapClassLoader loader;
        return &loader;
    }

    InstanceClassStruct *loadClass(u1 *classBytes, size_t classSize);

    InstanceClassStruct *loadClassByName(const strings::String &name);
};
//}
