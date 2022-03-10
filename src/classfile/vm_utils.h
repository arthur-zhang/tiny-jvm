#pragma once

#include "types.hpp"
#include "tiny_jvm.h"
#include "String.h"

using namespace ujvm;

strings::String decodeMUTF8(u1 *inBytes, const size_t len);
