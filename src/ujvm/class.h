#pragma once

#include "shared/string.h"

using namespace ujvm;

typedef union monitorOrHashCode {
    void *address;
    long hashCode;
} monitorOrHashCode;

#define COMMON_OBJECT_INFO(_type_) \
    _type_ ofClass; /* Pointer to the class of the instance */ \
    monitorOrHashCode mhc;

typedef struct instanceClassStruct *INSTANCE_CLASS;

typedef struct classStruct *CLASS;

/* CLASS */
struct classStruct {
    COMMON_OBJECT_INFO(INSTANCE_CLASS);
    strings::String packageName;
    strings::String baseName;
    CLASS next;
    u2 accessFlags;
    u2 key;
};
union constantPoolEntryStruct {
    struct {
        u2 classIndex;
        u2 nameTypeIndex;
    } field; // field and method
    struct {
        u2 classIndex;
        u2 nameTypeIndex;
    } method; // field and method
    CLASS clazz;


};
struct constantPoolStruct {

};
typedef struct constantPoolStruct *CONSTANTPOOL;
struct instanceClassStruct {
    struct classStruct clazz;
    INSTANCE_CLASS superClass;
    CONSTANTPOOL constantPool;

};