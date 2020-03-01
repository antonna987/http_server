#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

enum class Status {
    OK = 200,
    NOT_FOUND = 404,
    NOT_IMPLEMENTED = 501
};

struct StatusHash {
    size_t operator()(const Status& status) const noexcept
    {
        return static_cast<size_t>(status);
    }
};

std::string status_to_string(Status status)
{
    static const std::unordered_map<Status, std::string, StatusHash> STATUS_TO_STRING{
        { Status::OK, "OK" },
        { Status::NOT_FOUND, "Not Found" },
        { Status::NOT_IMPLEMENTED, "Not Implemented" },
    };
    std::string str;
    str += std::to_string(static_cast<int>(status)) + " ";
    str += STATUS_TO_STRING.at(status);
    return str;
}

inline std::ostream& operator<<(std::ostream& os, Status status)
{
    return os << status_to_string(status);
}
