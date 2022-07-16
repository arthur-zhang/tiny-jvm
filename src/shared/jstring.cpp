#include "jstring.h"
#include <codecvt>
#include <locale>

namespace ujvm {
    namespace strings {
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> CONVERT;

        std::string toStdString(const String &str) {
            return CONVERT.to_bytes(str);
        }

        String fromStdString(const std::string &str) {
            return CONVERT.from_bytes(str);
        }

        String replaceAll(const String &str, const String &from, const String &to) {
            String newString = str;
            size_t fromSize = from.size();
            size_t toSize = to.size();
            int pos = 0;
            while ((pos = newString.find(from, pos)) != String::npos) {
                newString.replace(pos, fromSize, to);
                pos += toSize;
            }
            return newString;
        }
    }
}