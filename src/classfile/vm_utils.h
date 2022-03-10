#pragma once

#include "types.hpp"
#include "tiny_jvm.h"
#include "String.h"

using namespace ujvm;

strings::String decodeMUTF8(const u1 *inBytes, size_t len);
