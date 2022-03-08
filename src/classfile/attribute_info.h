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

    AttributeInfo(ClassReader &reader) {
        attribute_name_index = reader.readUInt16();
        attribute_length = reader.readUInt32();
    }

    virtual ~AttributeInfo() = 0;

    virtual void dump(DataOutputStream &os) {
        os.writeUInt16(attribute_name_index);
        os.writeUInt32(attribute_length);
    }

    static AttributeInfo *readAttributeInfo(ClassReader &reader, ConstantPool *constant_pool);

    static u2 attributeName2Tag(u2 attribute_name_index,
                                ConstantPool *constant_pool);
};

inline AttributeInfo::~AttributeInfo() {}