#pragma once

#include "shared/types.h"
#include "tiny_jvm.h"
#include "shared/jstring.h"

using namespace ujvm;

strings::String* decodeMUTF8(const u1 *inBytes, size_t len);
