#pragma once

#include "types.hpp"
#include "class_reader.h"
#include "data_output_stream.hpp"

class ElementValue {
public:
    ElementValue(ClassReader &reader, u1 tag_) : tag(tag_) {}

    u1 tag;

    static ElementValue *readElementValue(ClassReader &reader);

    virtual void dump(DataOutputStream &os) = 0;

    virtual ~ElementValue() = default;
};

class SimpleElementValue : public ElementValue {
public:
    u2 const_value_index;

    SimpleElementValue(ClassReader &reader, u1 type);

    void dump(DataOutputStream &os) override {
        os.writeUInt16(const_value_index);
    }
};

class ClassElementValue : public ElementValue {
public:
    u2 class_info_index;

    ClassElementValue(ClassReader &reader, u1 type);

    void dump(DataOutputStream &os) override {
        os.writeUInt16(class_info_index);
    }
};

class EnumElementValue : public ElementValue {
public:
    EnumElementValue(ClassReader &reader, u1 type) : ElementValue(reader, type) {
        type_name_index = reader.readUInt16();
        const_name_index = reader.readUInt16();
    }

    u2 type_name_index;
    u2 const_name_index;

    void dump(DataOutputStream &os) override {
        os.writeUInt16(type_name_index);
        os.writeUInt16(const_name_index);
    }
};

class ArrayElementValue : public ElementValue {
public:
    u2 num_values;
    ElementValue **values = nullptr;        // [num_values]
    ArrayElementValue(ClassReader &reader, u1 type);

    void dump(DataOutputStream &os) override {
        os.writeUInt16(num_values);
        for (int pos = 0; pos < num_values; pos++) {
            values[pos]->dump(os);
        }
    }
};

class ElementValuePair {
public:
    ElementValuePair(ClassReader &reader) {
        element_name_index = reader.readUInt16();
        value = ElementValue::readElementValue(reader);
    }

    u2 element_name_index;
    ElementValue *value;

    void dump(DataOutputStream &os) {
        os.writeUInt16(element_name_index);
        value->dump(os);
    }
};

class AnnotationElementValue : public ElementValue {
public:
    AnnotationElementValue(ClassReader &reader, u1 tag = '@') : ElementValue(reader, tag) {
        type_index = reader.readUInt16();
        num_element_value_pairs = reader.readUInt16();
        if (num_element_value_pairs != 0)
            element_value_pairs = new ElementValuePair *[num_element_value_pairs];
        for (int pos = 0; pos < num_element_value_pairs; pos++) {
            element_value_pairs[pos] = new ElementValuePair(reader);
        }
    }

    u2 type_index;
    u2 num_element_value_pairs;
    ElementValuePair **element_value_pairs = nullptr;        // [num_element_value_pairs]

    void dump(DataOutputStream &os) override {
        os.writeUInt16(type_index);
        os.writeUInt16(num_element_value_pairs);
        for (int pos = 0; pos < num_element_value_pairs; pos++) {
            element_value_pairs[pos]->dump(os);
        }
    }
};