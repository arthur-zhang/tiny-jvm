//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_SOURCE_FILE_ATTRIBUTE_H
#define TINY_JVM_SOURCE_FILE_ATTRIBUTE_H

#include "attribute_info.h"

class SourceFileAttribute : public AttributeInfo {
public:
    u2 source_file_index;

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(source_file_index);
    }

    SourceFileAttribute(ClassReader &reader);
};


#endif //TINY_JVM_SOURCE_FILE_ATTRIBUTE_H
