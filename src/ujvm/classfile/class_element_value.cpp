//
// Created by ya on 2022/3/6.
//

#include "class_element_value.h"

ClassElementValue::ClassElementValue(ClassReader &reader, u1 tag) : ElementValue(reader, tag) {
    class_info_index = reader.readUInt16();
}

void ClassElementValue::dump(DataOutputStream &os) {
    ElementValue::dump(os);
    os.writeUInt16(class_info_index);
}
