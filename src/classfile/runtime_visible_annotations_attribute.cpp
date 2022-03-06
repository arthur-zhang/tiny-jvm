#include "runtime_visible_annotations_attribute.h"

RuntimeVisibleAnnotationsAttribute::RuntimeVisibleAnnotationsAttribute(ClassReader &reader) : AttributeInfo(reader) {
    parameter_annotations = new parameter_annotations_t(reader);
}

void RuntimeVisibleAnnotationsAttribute::dump(DataOutputStream &os) {
    AttributeInfo::dump(os);
    parameter_annotations->dump(os);
}