#pragma once


#include "shared/hashmap.h"
#include "shared/jstring.h"
#include <ujvm/classpath/class_loader.h>
#include <ujvm/runtime/instance_class.h>

using namespace ujvm;


class SystemDictionary final {
public:
    static SystemDictionary *get();

    bool put(const strings::String &name, InstanceKlass *clazz);

    InstanceKlass *find(const strings::String &name);

private:
    HashMap<strings::String, InstanceKlass *> classMap_;
};


