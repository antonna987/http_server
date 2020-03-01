#pragma once

#include "method.h"
#include "status.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

const std::string PROTOCOL = "HTTP/1.0";
const std::string SERVER = "MyServer";
const std::string CONTENT_TYPE = "text/html";

struct Response {
    void compile(const std::string& content, Status st)
    {
        status = st;
        content_length = content.size();

        buffer.clear();
        buffer += PROTOCOL + " " + status_to_string(status) + "\r\n";
        // buffer += "Date: " + date_time_now_as_string() + "\r\n";
        buffer += "Server: " + SERVER + "\r\n";
        buffer += "Connection: close\r\n";
        if (status != Status::OK) {
            return;
        }
        buffer += "Content-Type: " + CONTENT_TYPE + "\r\n";
        buffer += "Content-Length: " + std::to_string(content.size()) + "\r\n";
        buffer += "\r\n";
        buffer += content;
    }

    Status status = Status::NOT_IMPLEMENTED;
    size_t content_length = 0;
    std::string buffer;

private:
    // std::string date_time_now_as_string() const
    // {
    //     auto tp = std::chrono::system_clock::now();
    //     auto t = std::chrono::system_clock::to_time_t(tp);
    //     auto tm = *std::gmtime(&t);
    //     std::stringstream ss;
    //     ss << std::put_time(&tm, "%a, %d %b %Y %H:%M:%S %Z");
    //     return ss.str();
    // }
};
