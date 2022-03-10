//
// Created by ya on 2022/3/6.
//

#include "code_attribute.h"

CodeAttribute::CodeAttribute(ClassReader &reader, ConstantPool *constantPool) : AttributeInfo(reader) {
    max_stack = reader.readUInt16();
    max_locals = reader.readUInt16();
    code_length = reader.readUInt32();
    if (code_length != 0) {
        code = new u1[code_length];
        code = reader.readBytes(code_length);
    }
    exception_table_length = reader.readUInt16();
    if (exception_table_length != 0) {
        exception_table = new ExceptionTable *[exception_table_length];
        for (int pos = 0; pos < exception_table_length; pos++) {
            exception_table[pos] = new ExceptionTable(reader);
        }
    }
    attributes_count = reader.readUInt16();
    if (attributes_count != 0)
        attributes = new AttributeInfo *[attributes_count];
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos] = readAttributeInfo(reader, constantPool);
    }
}

CodeAttribute::~CodeAttribute() {
    if (code_length > 0) delete[] code;
    if (attributes_count > 0) {
        for (int i = 0; i < attributes_count; ++i) {
            delete attributes[i];
        }
        delete[] attributes;
    }
    if (exception_table_length > 0) {
        for (int i = 0; i < exception_table_length; ++i) {
            delete exception_table[i];
        }
        delete[]exception_table;
    }
}
