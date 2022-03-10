#pragma once

#include "element_value_pair.h"
#include "annotation_entry.h"

/**
 * represents one annotation in the annotation table
 */
class ElementValuePair;

/**
 annotation {
     u2 type_index;
     u2 num_element_value_pairs;
     {
       u2 element_name_index;
       element_value value;
     } element_value_pairs[num_element_value_pairs];
  }
 */
class AnnotationEntry {
public:
    u2 type_index;
    u2 num_element_value_pairs;
    ElementValuePair **element_value_pairs = nullptr;        // [num_element_value_pairs]
    AnnotationEntry(ClassReader &reader);

    void dump(DataOutputStream &os);
};


