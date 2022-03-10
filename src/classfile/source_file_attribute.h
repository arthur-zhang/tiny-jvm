#pragma once

#include "attribute_info.h"

class SourceFileAttribute : public AttributeInfo {
public:
    u2 source_file_index;

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(source_file_index);
    }

    SourceFileAttribute(ClassReader &reader) : AttributeInfo(reader) {
        source_file_index = reader.readUInt16();
    }

    virtual ~SourceFileAttribute() = default;
};