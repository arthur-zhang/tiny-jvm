#pragma once

#include "classfile/classfile_parser.h"

class KClass {

};

class InstanceKClass : public KClass {

private:
    ClassFile *classFile_ = nullptr;
};