#pragma once

#include "classfile/classfile_parser.h"

class KClass {

};

class InstanceKClass : public KClass {
public:
    InstanceKClass(ClassFile *classFile) : classFile_(classFile) {}

private:
    ClassFile *classFile_ = nullptr;
};