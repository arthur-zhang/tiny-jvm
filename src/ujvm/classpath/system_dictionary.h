#pragma once


#include <shared/hashmap.h>
#include <shared/jstring.h>
#include "ujvm/kclass.h"

using namespace ujvm;


class SystemDictionary final {
public:
    static SystemDictionary *get();

    bool put(const strings::String &name, KClass *clazz);

    KClass *find(strings::String &name);

private:
    HashMap<strings::String, KClass *> classMap_;
};


