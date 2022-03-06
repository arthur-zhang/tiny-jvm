//
// Created by ya on 2022/3/6.
//

#include "source_file_attribute.h"


SourceFileAttribute::SourceFileAttribute(ClassReader &reader) : AttributeInfo(reader) {
    source_file_index = reader.readUInt16();
}
