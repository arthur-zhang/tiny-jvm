//
// Created by ya on 2022/3/6.
//

#ifndef TINY_JVM_LINE_NUMBER_TABLE_ATTRIBUTE_H
#define TINY_JVM_LINE_NUMBER_TABLE_ATTRIBUTE_H

#include "attribute_info.h"

class LineNumber {
public:
    u2 start_pc;
    u2 line_number;

    LineNumber(ClassReader &reader) {
        start_pc = reader.readUInt16();
        line_number = reader.readUInt16();
    }

    void dump(DataOutputStream &os) {
        os.writeUInt16(start_pc);
        os.writeUInt16(line_number);
    };
};

class LineNumberTableAttribute : public AttributeInfo {
public:
    u2 line_number_table_length;

    LineNumber **line_number_table = nullptr;        // [line_number_table_length]

    LineNumberTableAttribute(ClassReader &reader) : AttributeInfo(reader) {
        line_number_table_length = reader.readUInt16();
        if (line_number_table_length != 0)
            line_number_table = new LineNumber *[line_number_table_length];
        for (int pos = 0; pos < line_number_table_length; pos++) {
            line_number_table[pos] = new LineNumber(reader);
        }
    }

    void dump(DataOutputStream &os) override {
        AttributeInfo::dump(os);
        os.writeUInt16(line_number_table_length);
        for (int i = 0; i < line_number_table_length; i++) {
            line_number_table[i]->dump(os);
        }
    }
};


#endif //TINY_JVM_LINE_NUMBER_TABLE_ATTRIBUTE_H
