//
// Created by ya on 2022/3/1.
//

#ifndef TINY_JVM_STRING_H
#define TINY_JVM_STRING_H
#include "types.hpp"
using String  = std::wstring;

String fromBytes(u1 *bytes, size_t length);
std::string toStdString(const String &str);

#endif //TINY_JVM_STRING_H
