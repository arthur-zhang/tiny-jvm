//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_CODE_ATTRIBUTE_H
#define TINY_JVM_CODE_ATTRIBUTE_H

#include "attribute_info.h"

class CodeAttribute : public AttributeInfo {
public:
    u2 max_stack;
    u2 max_locals;

    u4 code_length;
    u1 *code;

    u2 exception_table_length;
    ExceptionTable **exception_table;

    u2 attributes_count;
    AttributeInfo **attributes;

    CodeAttribute(ClassReader &reader, ConstantPool *constantPool);

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(max_stack);
        os.writeUInt16(max_locals);
        os.writeUInt32(code_length);
        os.writeBytes(code, code_length);
        os.writeUInt16(exception_table_length);
        for (int i = 0; i < exception_table_length; i++) {
            exception_table[i]->dump(os);
        }
        os.writeUInt16(attributes_count);
        for (int i = 0; i < attributes_count; i++) {
            attributes[i]->dump(os);
        }
    }

    virtual ~CodeAttribute();
};


#endif //TINY_JVM_CODE_ATTRIBUTE_H
