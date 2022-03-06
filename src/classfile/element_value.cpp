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

ArrayElementValue::ArrayElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    num_values = reader.readUInt16();
    if (num_values != 0) {
        values = new ElementValue *[num_values]();
    }
    for (int pos = 0; pos < num_values; pos++) {
        values[pos] = ElementValue::readElementValue(reader);
    }
}

SimpleElementValue::SimpleElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    const_value_index = reader.readUInt16();
}

ClassElementValue::ClassElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    class_info_index = reader.readUInt16();
}

AnnotationElementValue::AnnotationElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    annotationEntry = new AnnotationEntry(reader);
}

void AnnotationElementValue::dump(DataOutputStream &os) {
    ElementValue::dump(os);
    annotationEntry->dump(os);
}
