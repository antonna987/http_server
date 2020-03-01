#pragma once

#include "request.h"
#include "response.h"
#include <memory>

struct Context {
    Request request;
    Response response;
};

using ContextPtr = std::shared_ptr<Context>;

inline ContextPtr make_context()
{
    return std::make_shared<Context>();
}
