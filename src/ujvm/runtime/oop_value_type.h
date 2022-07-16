#pragma once

#include "shared/tiny_jvm.h"

enum OopType {
    INSTANCE_OOP,
    PRIMITIVE_OOP,
    OBJECT_ARRAY_OOP,
    TYPE_ARRAY_OOP,
};
enum ValueType {
    VOID,
    BYTE,
    BOOLEAN,
    CHAR,
    SHORT,
    INT,
    FLOAT,
    LONG,
    DOUBLE,
    OBJECT,
    ARRAY,
};


static ValueType primitiveTypeToValueTypeNoWrap(wchar_t c) {
    switch (c) {
        case L'B':    // byte
            return ValueType::BYTE;
        case L'Z':    // boolean
            return ValueType::BOOLEAN;
        case L'S':    // short
            return ValueType::SHORT;
        case L'C':    // char
            return ValueType::CHAR;
        case L'I':    // int
            return ValueType::INT;
        case L'J':    // long
            return ValueType::LONG;
        case L'F':    // float
            return ValueType::FLOAT;
        case L'D':    // double
            return ValueType::DOUBLE;
        case L'V':    // void
            return ValueType::VOID;
        default:
            PANIC("primitive type required");
            break;
    }
}