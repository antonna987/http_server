#pragma once

#include "method.h"
#include <string>

const size_t REQUEST_BUFFER_SIZE = 4096;

struct Request {
    Request()
        : buffer(REQUEST_BUFFER_SIZE, ' ')
    {
    }

    void shrink(size_t bytes_transferred)
    {
        buffer.resize(bytes_transferred);
    }

    void parse()
    {
        if (buffer.empty()) {
            return;
        }

        auto s1 = buffer.find_first_of(' ');
        s1 = s1 == std::string::npos ? buffer.size() : s1;
        std::string m_str(buffer.data(), s1 - 0);
        method = method_from_string(m_str);

        if (s1 + 1 >= buffer.size() - 1) {
            return;
        }
        auto s2 = buffer.find_first_of(" ?", s1 + 1);
        s2 = s2 == std::string::npos ? buffer.size() : s2;
        path = std::string(buffer.data() + s1 + 1, buffer.data() + s2);
        path = path == "/" ? "/index.html" : path;
    }

    std::string buffer;
    Method method;
    std::string path;
};
