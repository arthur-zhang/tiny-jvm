//
// Created by ya on 2022/3/6.
//

#include "simple_element_value.h"

SimpleElementValue::SimpleElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    const_value_index = reader.readUInt16();
}