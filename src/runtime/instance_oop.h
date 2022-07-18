//
// Created by arthur on 2022/6/29.
//

#ifndef TINY_JVM_INSTANCE_OOP_H
#define TINY_JVM_INSTANCE_OOP_H


#include "instance_class.h"

class InstanceOop {
public:
    InstanceKlass *clazz;
    map<strings::String, u1 *> fieldMap;

    InstanceOop(InstanceKlass *clazz) : clazz(clazz) {}

    static InstanceOop *allocateInstance(InstanceKlass *clazz) {
        InstanceOop *oop = new InstanceOop(clazz);
        return oop;
    }
};


#endif //TINY_JVM_INSTANCE_OOP_H
