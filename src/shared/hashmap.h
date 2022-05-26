#pragma once

#include <unordered_map>

namespace ujvm {
    template<typename K, typename V>
    using HashMap = std::unordered_map<K, V>;
}
