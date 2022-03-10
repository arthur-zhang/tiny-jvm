#pragma once

#include "shared/types.h"
#include "class_reader.h"
#include "data_output_stream.hpp"

class ElementValue {
public:
    ElementValue(ClassReader &reader, u1 tag_) : tag(tag_) {};

    u1 tag;

    static ElementValue *readElementValue(ClassReader &reader);

    virtual void dump(DataOutputStream &os);

    virtual ~ElementValue() = default;
};

/**
 annotation {
     u2 type_index;
     u2 num_element_value_pairs;
     {
       u2 element_name_index;
       element_value value;
     } element_value_pairs[num_element_value_pairs];
  }
 */
class ElementValuePair;

class AnnotationEntry {
public:
    u2 type_index;
    u2 num_element_value_pairs;
    ElementValuePair **element_value_pairs = nullptr;        // [num_element_value_pairs]
    AnnotationEntry(ClassReader &reader);

    void dump(DataOutputStream &os);
};

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

class SimpleElementValue : public ElementValue {
public:
    u2 const_value_index;

    SimpleElementValue(ClassReader &reader, u1 type);

    void dump(DataOutputStream &os) override {
        ElementValue::dump(os);

        os.writeUInt16(const_value_index);
    }
};

class ArrayElementValue : public ElementValue {
public:
    u2 num_values;
    ElementValue **values = nullptr;

    ArrayElementValue(ClassReader &reader, u1 tag);

    void dump(DataOutputStream &os) override {
        ElementValue::dump(os);
        os.writeUInt16(num_values);
        for (int pos = 0; pos < num_values; pos++) {
            values[pos]->dump(os);
        }
    }
};

class EnumElementValue : public ElementValue {
public:
    EnumElementValue(ClassReader &reader, u1 type);

    u2 type_name_index;
    u2 const_name_index;

    void dump(DataOutputStream &os) override;
};

class ElementValuePair {
public:
    ElementValuePair(ClassReader &reader);

    u2 element_name_index;
    ElementValue *value;

    void dump(DataOutputStream &os);
};