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

class AnnotationEntry;//todo

class AnnotationElementValue : public ElementValue {
public:
    AnnotationElementValue(ClassReader &reader, u1 tag);

    AnnotationEntry *annotationEntry;

    void dump(DataOutputStream &os) override;
};

class ClassElementValue : public ElementValue {
public:
    u2 class_info_index;

    ClassElementValue(ClassReader &reader, u1 type);

    void dump(DataOutputStream &os) override;
};