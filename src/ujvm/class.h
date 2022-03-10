#pragma once

#include "shared/string.h"

using namespace ujvm;

typedef union monitorOrHashCode {
    void *address;
    long hashCode;
};

#define COMMON_OBJECT_INFO(_type_) \
    _type_ ofClass; /* Pointer to the class of the instance */ \
    monitorOrHashCode mhc;

typedef struct instanceClassStruct *INSTANCE_CLASS;

/* CLASS */
struct classStruct {
    COMMON_OBJECT_INFO(INSTANCE_CLASS);
    strings::String packageName;

};

struct instanceClassStruct {
    struct classStruct clazz;
    INSTANCE_CLASS superClass;

};