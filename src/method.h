#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

enum class Method {
    GET,
    UNIMPLEMENTED
};

struct MethodHash {
    size_t operator()(const Method& method) const noexcept
    {
        return static_cast<size_t>(method);
    }
};

inline Method method_from_string(const std::string& str)
{
    static const std::unordered_map<std::string, Method> STRING_TO_METHOD{
        { "GET", Method::GET },
    };
    if (STRING_TO_METHOD.count(str)) {
        return STRING_TO_METHOD.at(str);
    }
    return Method::UNIMPLEMENTED;
}

inline const std::string& method_to_string(Method method)
{
    static const std::unordered_map<Method, std::string, MethodHash> METHOD_TO_STRING{
        { Method::GET, "GET" },
        { Method::UNIMPLEMENTED, "UNIMPLEMENTED" },
    };
    return METHOD_TO_STRING.at(method);
}

inline std::ostream& operator<<(std::ostream& os, const Method& method)
{
    return os << method_to_string(method);
}
