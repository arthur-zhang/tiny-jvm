#pragma once

#include "instance_class.h"

class InstanceOop {

public:

    InstanceOop(InstanceKlass *clazz) : clazz(clazz) {}

    static InstanceOop *allocateInstance(InstanceKlass *clazz) {
        InstanceOop *oop = new InstanceOop(clazz);
        return oop;
    }

private:
    InstanceKlass *clazz;
    map<strings::String, u1 *> fieldMap;
};


