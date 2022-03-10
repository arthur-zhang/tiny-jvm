#pragma once

#include "shared/types.h"
#include "class_reader.h"
#include "data_output_stream.hpp"
#include "annotation_entry.h"

class ElementValue {
public:
    ElementValue(ClassReader &reader, u1 tag_) : tag(tag_) {};

    u1 tag;

    static ElementValue *readElementValue(ClassReader &reader);

    virtual void dump(DataOutputStream &os);

    virtual ~ElementValue() = default;
};
