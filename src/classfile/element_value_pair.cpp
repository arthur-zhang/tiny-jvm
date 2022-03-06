//
// Created by ya on 2022/3/5.
//

#include "element_value_pair.h"

ElementValuePair::ElementValuePair(ClassReader &reader) {
    element_name_index = reader.readUInt16();
    value = ElementValue::readElementValue(reader);
}

void ElementValuePair::dump(DataOutputStream &os) {
    os.writeUInt16(element_name_index);
    value->dump(os);
}
