#pragma once

#include <string>
#include "types.h"


namespace ujvm {
    namespace strings {
        using String = std::wstring;

        std::string toStdString(const String &str);

        String fromStdString(const std::string &str);

        String replaceAll(const String &str, const String &from, const String &to);
    }

//    using String = strings::String;
}
