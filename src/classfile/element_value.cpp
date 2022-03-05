//
// Created by ya on 2022/3/2.
//

#include "element_value.h"

ElementValue *ElementValue::readElementValue(ClassReader &reader) {
    u1 tag = reader.peek1();
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

ArrayElementValue::ArrayElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    num_values = reader.readUInt16();
    if (num_values != 0) {
        values = new ElementValue *[num_values];
    }
    for (int pos = 0; pos < num_values; pos++) {
        values[pos] = ElementValue::readElementValue(reader);
    }
}

SimpleElementValue::SimpleElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    const_value_index = reader.readUInt16();
}

ClassElementValue::ClassElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    class_info_index = reader.readUInt16();
}
