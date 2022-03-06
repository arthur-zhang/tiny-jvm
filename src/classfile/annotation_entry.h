#pragma once

#include "element_value_pair.h"

/**
 * represents one annotation in the annotation table
 */
class ElementValuePair;

class AnnotationEntry {
public:
    u2 type_index;
    u2 num_element_value_pairs;
    ElementValuePair **element_value_pairs = nullptr;        // [num_element_value_pairs]
    AnnotationEntry(ClassReader &reader);

    void dump(DataOutputStream &os);
};


