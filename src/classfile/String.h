#pragma once

#include <string>
#include "types.hpp"

namespace ujvm {
    namespace strings {
        using String = std::wstring;
        std::string toStdString(const String &str);
    }
}
