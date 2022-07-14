#pragma once

#include <cstdint>
#include <cstdlib>
typedef std::uint8_t u1;
typedef std::uint16_t u2;
typedef std::uint32_t u4;

typedef int jint;
typedef long long jlong; // long long: 64bit, long: 32bit
typedef void *jobject;
typedef jobject jclass;
typedef float jfloat;
typedef short jshort;
typedef double jdouble;
typedef unsigned char jboolean;
typedef unsigned short jchar;

#define JAVA_NATIVE extern "C"