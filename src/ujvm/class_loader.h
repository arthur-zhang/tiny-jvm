#pragma once


#include "shared/types.h"
#include "kclass.h"

namespace ujvm {

    class ClassLoader {

        KClass *loadClass(u1 *classBytes, size_t classSize);

        KClass *loadRawClass(strings::String &name);
    };


}
