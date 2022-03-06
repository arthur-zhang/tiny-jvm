//
// Created by ya on 2022/3/6.
//

#include "constant_value_attribute.h"


ConstantValueAttribute::ConstantValueAttribute(ClassReader &reader) : AttributeInfo(reader) {
    constant_value_index = reader.readUInt16();
}

void ConstantValueAttribute::dump(DataOutputStream &os) {
    AttributeInfo::dump(os);
    os.writeUInt16(constant_value_index);
}
