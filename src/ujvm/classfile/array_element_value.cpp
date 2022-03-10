//
// Created by ya on 2022/3/6.
//

#include "array_element_value.h"

ArrayElementValue::ArrayElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    num_values = reader.readUInt16();
    if (num_values != 0) {
        values = new ElementValue *[num_values]();
    }
    for (int pos = 0; pos < num_values; pos++) {
        values[pos] = ElementValue::readElementValue(reader);
    }
}