#pragma once

#include "types.hpp"
#include "class_reader.h"
#include "String.h"
#include "constant_pool.hpp"
#include "exception_table.h"
#include "element_value.h"
#include "method_parameter.h"

class AttributeInfo {
public:
    u2 attribute_name_index;
    u4 attribute_length;

    AttributeInfo(ClassReader &reader);

    ~AttributeInfo() {}//todo

    virtual void dump(DataOutputStream &os) = 0;

    static AttributeInfo *readAttribute(ClassReader &reader, ConstantPool *constant_pool);

    static u2 attributeName2Tag(u2 attribute_name_index,
                                ConstantPool *constant_pool);
};


class Annotations {
public:
    Annotations(ClassReader &reader) {
        num_annotations = reader.readUInt16();
        if (num_annotations != 0)
            annotations = new AnnotationEntry *[num_annotations];
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos] = new AnnotationEntry(reader); // todo
        }
    }

    u2 num_annotations;
    AnnotationEntry **annotations = nullptr;    // [num_annotations]

    void dump(DataOutputStream &os) {
        os.writeUInt16(num_annotations);
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos]->dump(os);
        }
    }
};


