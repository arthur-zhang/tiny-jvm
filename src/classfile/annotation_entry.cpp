//
// Created by ya on 2022/3/5.
//

#include "annotation_entry.h"

AnnotationEntry::AnnotationEntry(ClassReader &reader) {
    type_index = reader.readUInt16();
    num_element_value_pairs = reader.readUInt16();
    if (num_element_value_pairs == 0)return;
    element_value_pairs = new ElementValuePair *[num_element_value_pairs];
    for (int pos = 0; pos < num_element_value_pairs; pos++) {
        element_value_pairs[pos] = new ElementValuePair(reader);
    }
}

void AnnotationEntry::dump(DataOutputStream &os) {
    os.writeUInt16(type_index);
    os.writeUInt16(num_element_value_pairs);
    for (int pos = 0; pos < num_element_value_pairs; pos++) {
        element_value_pairs[pos]->dump(os);
    }
}
