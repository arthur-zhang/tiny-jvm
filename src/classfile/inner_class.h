//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_INNER_CLASS_H
#define TINY_JVM_INNER_CLASS_H

#include "attribute_info.h"

class InnerClass {
public:
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;

    InnerClass(ClassReader &reader) {
        inner_class_info_index = reader.readUInt16();
        outer_class_info_index = reader.readUInt16();
        inner_name_index = reader.readUInt16();
        inner_class_access_flags = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(inner_class_info_index);
        os.writeUInt16(outer_class_info_index);
        os.writeUInt16(inner_name_index);
        os.writeUInt16(inner_class_access_flags);
    }
};


#endif //TINY_JVM_INNER_CLASS_H
