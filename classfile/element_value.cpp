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
            std::cerr << "can't get here. in element_value." << std::endl;
            assert(false);
        }
    }
}

AnnotationElementValue::AnnotationElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    type_index = reader.readUint16();
    num_element_value_pairs = reader.readUint16();
    if (num_element_value_pairs != 0)
        element_value_pairs = new ElementValuePair *[num_element_value_pairs];
    for (int pos = 0; pos < num_element_value_pairs; pos++) {
        element_value_pairs[pos] = new ElementValuePair(reader);
    }
}

ArrayElementValue::ArrayElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    num_values = reader.readUint16();
    if (num_values != 0) {
        values = new ElementValue *[num_values];
    }
    for (int pos = 0; pos < num_values; pos++) {
        values[pos] = ElementValue::readElementValue(reader);
    }
}

SimpleElementValue::SimpleElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    const_value_index = reader.readUint16();
}

ClassElementValue::ClassElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    class_info_index = reader.readUint16();
}
