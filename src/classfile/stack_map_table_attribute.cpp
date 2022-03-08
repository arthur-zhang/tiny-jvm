//
// Created by ya on 2022/3/6.
//

#include "stack_map_table_attribute.h"


StackMapTableAttribute::StackMapTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
    // todo later
    bytes = reader.readBytes(attribute_length);
}

void StackMapTableAttribute::dump(DataOutputStream &os) {
    AttributeInfo::dump(os);
    os.writeBytes(bytes, attribute_length);
}


StackMapTableAttribute::~StackMapTableAttribute() {
    if (attribute_length <= 0) return;
    delete[] bytes;
}