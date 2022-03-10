//
// Created by ya on 2022/3/2.
//

#include "element_value.h"
#include "array_element_value.h"
#include "simple_element_value.h"
#include "enum_element_value.h"

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