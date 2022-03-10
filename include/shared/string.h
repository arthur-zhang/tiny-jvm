#pragma once

#include <string>
#include "types.h"

namespace ujvm::strings {
    using String = std::wstring;

    std::string toStdString(const String &str);
}
