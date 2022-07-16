#pragma once


#include <shared/hashmap.h>
#include "shared/jstring.h"
#include "ujvm/class_loader.h"

using namespace ujvm;


class SystemDictionary final {
public:
    static SystemDictionary *get();

    bool put(const strings::String &name, InstanceClassStruct *clazz);

    InstanceClassStruct *find(const strings::String &name);

private:
    HashMap<strings::String, InstanceClassStruct *> classMap_;
};


