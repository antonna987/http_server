#pragma once

#include "context.h"
#include "options.h"
#include <fstream>
#include <streambuf>
#include <string>

inline void http_impl(const Options& options, ContextPtr ctx, size_t bytes_received)
{
    ctx->request.shrink(bytes_received);
    ctx->request.parse();

    if (ctx->request.method != Method::GET) {
        ctx->response.compile("", Status::NOT_IMPLEMENTED);
        return;
    }

    auto filename = options.dir() + ctx->request.path;
    std::ifstream fstream(filename);
    if (!fstream.good()) {
        ctx->response.compile("", Status::NOT_FOUND);
        return;
    }

    std::string content(
        (std::istreambuf_iterator<char>(fstream)),
        std::istreambuf_iterator<char>());
    ctx->response.compile(content, Status::OK);
}
