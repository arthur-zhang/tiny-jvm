//
// Created by ya on 2022/3/2.
//

#include "element_value.h"

ElementValue *ElementValue::readElementValue(ClassReader &reader) {
    u1 tag = reader.readUint8();
    switch ((char) tag) {
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
        case 's': {
            return new SimpleElementValue(reader, tag);
        }
        case 'e': {
            return new EnumElementValue(reader, tag);
        }
        case 'c': {
            return new ClassElementValue(reader, tag);
        }
        case '@': {
            return new AnnotationElementValue(reader, tag);

        }
        case '[': {
            return new ArrayElementValue(reader, tag);
        }
        default: {
            std::cerr << "tag:" << tag << std::endl;
            std::cerr << "can't get here. in element_value." << std::endl;
            assert(false);
        }
    }
}


void ElementValue::dump(DataOutputStream &os) {
    os.writeUInt8(tag);
}

AnnotationElementValue::AnnotationElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    annotationEntry = new AnnotationEntry(reader);
}

void AnnotationElementValue::dump(DataOutputStream &os) {
    ElementValue::dump(os);
    annotationEntry->dump(os);
}


ClassElementValue::ClassElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    class_info_index = reader.readUInt16();
}

void ClassElementValue::dump(DataOutputStream &os) {
    ElementValue::dump(os);
    os.writeUInt16(class_info_index);
}

SimpleElementValue::SimpleElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    const_value_index = reader.readUInt16();
}

ArrayElementValue::ArrayElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    num_values = reader.readUInt16();
    if (num_values != 0) {
        values = new ElementValue *[num_values]();
    }
    for (int pos = 0; pos < num_values; pos++) {
        values[pos] = readElementValue(reader);
    }
}

EnumElementValue::EnumElementValue(ClassReader &reader, uint8_t type) : ElementValue(reader, type) {
    type_name_index = reader.readUInt16();
    const_name_index = reader.readUInt16();
}

void EnumElementValue::dump(DataOutputStream &os) {
    ElementValue::dump(os);
    os.writeUInt16(type_name_index);
    os.writeUInt16(const_name_index);
}

ElementValuePair::ElementValuePair(ClassReader &reader) {
    element_name_index = reader.readUInt16();
    value = ElementValue::readElementValue(reader);
}

void ElementValuePair::dump(DataOutputStream &os) {
    os.writeUInt16(element_name_index);
    value->dump(os);
}

AnnotationEntry::AnnotationEntry(ClassReader &reader) {
    type_index = reader.readUInt16();
    num_element_value_pairs = reader.readUInt16();
    if (num_element_value_pairs == 0)return;
    element_value_pairs = new ElementValuePair *[num_element_value_pairs];
    for (int pos = 0; pos < num_element_value_pairs; pos++) {
        element_value_pairs[pos] = new ElementValuePair(reader);
    }
}

void AnnotationEntry::dump(DataOutputStream &os) {
    os.writeUInt16(type_index);
    os.writeUInt16(num_element_value_pairs);
    for (int pos = 0; pos < num_element_value_pairs; pos++) {
        element_value_pairs[pos]->dump(os);
    }
}