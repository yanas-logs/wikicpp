#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace wikicpp {
using json = nlohmann::json;
inline bool has_key(const json& j, const std::string& key) {
    return j.contains(key) && !j[key].is_null();
}

inline std::string get_string(const json& j, const std::string& key,
                               const std::string& default_val = "") {
    if (has_key(j, key) && j[key].is_string()) {
        return j[key].get<std::string>();
    }
    return default_val;
}
}
