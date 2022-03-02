//
// Created by ya on 2022/3/2.
//

#ifndef TINY_JVM_ELEMENT_VALUE_H
#define TINY_JVM_ELEMENT_VALUE_H

#include "types.h"
#include "class_reader.h"

class ElementValue {
public:
    ElementValue(ClassReader &reader, u1 tag_) : tag(tag_) {}

    u1 tag;

    static ElementValue *readElementValue(ClassReader &reader);

    virtual ~ElementValue() {}
};

class SimpleElementValue : public ElementValue {
public:
    u2 const_value_index;

    SimpleElementValue(ClassReader &reader, u1 type);
};

class ClassElementValue : public ElementValue {
public:
    u2 class_info_index;
    ClassElementValue(ClassReader &reader, u1 type);
};

class EnumElementValue : public ElementValue {
public:
    EnumElementValue(ClassReader &reader, u1 type) : ElementValue(reader, type) {
        type_name_index = reader.readUint16();
        const_name_index = reader.readUint16();
    }

    u2 type_name_index;
    u2 const_name_index;
};

class ArrayElementValue : public ElementValue {
public:
    u2 num_values;
    ElementValue **values = nullptr;        // [num_values]
    ArrayElementValue(ClassReader &reader, u1 type);
};

class ElementValuePair {
public:
    ElementValuePair(ClassReader &reader) {
        element_name_index = reader.readUint16();
        value = ElementValue::readElementValue(reader);
    }

    u2 element_name_index;
    ElementValue *value;
};

class AnnotationElementValue : public ElementValue {
public:
    AnnotationElementValue(ClassReader &reader, u1 type);

    u2 type_index;
    u2 num_element_value_pairs;
    ElementValuePair **element_value_pairs = nullptr;        // [num_element_value_pairs]
};

#endif //TINY_JVM_ELEMENT_VALUE_H
