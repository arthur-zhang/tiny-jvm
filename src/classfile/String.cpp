#include <codecvt>
#include <locale>
#include <utility>
#include "String.h"

namespace ujvm {
    namespace strings {
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> CONVERT;
        std::string toStdString(const String &str) {
            return CONVERT.to_bytes(str);
        }
    }
}