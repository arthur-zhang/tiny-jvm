//
// Created by ya on 2022/3/6.
//

#include "enum_element_value.h"

EnumElementValue::EnumElementValue(ClassReader &reader, std::uint8_t type) : ElementValue(reader, type) {
    type_name_index = reader.readUInt16();
    const_name_index = reader.readUInt16();
}

void EnumElementValue::dump(DataOutputStream &os) {
    ElementValue::dump(os);
    os.writeUInt16(type_name_index);
    os.writeUInt16(const_name_index);
}
