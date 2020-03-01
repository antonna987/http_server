#pragma once

#include "context.h"
#include "http.h"
#include "options.h"
#include "session.h"

#include <boost/asio.hpp>

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Server {
    using tcp = boost::asio::ip::tcp;
    using error_code = boost::system::error_code;

public:
    Server(const Options& opt)
        : options(opt)
        , endpoint(options.ip(), options.port())
        , acceptor(io_service, endpoint)
    {
        io_service.post([this]() { accept(); });

        auto nthreads = options.nthreads();
        while (nthreads--) {
            threads.push_back(
                std::thread([this]() { io_service.run(); }));
        }

        std::cout << "Server started at "
                  << options.ip() << ":"
                  << options.port() << std::endl;
    }
    ~Server()
    {
        io_service.stop();
        for (auto& thread : threads) {
            thread.join();
        }
        std::cout << "Server stopped" << std::endl;
    }

private:
    const Options& options;
    boost::asio::io_service io_service;
    tcp::endpoint endpoint;
    tcp::acceptor acceptor;

    std::vector<std::thread> threads;

    using Sessions = std::list<Session>;
    Sessions sessions;
    std::mutex lock;

    void accept()
    {
        std::lock_guard<std::mutex> guard(lock);
        auto session_it = sessions.insert(
            sessions.end(),
            Session(io_service));
        acceptor.async_accept(
            session_it->socket(),
            [this, session_it](const error_code& error) {
                accept_cb(session_it, error);
            });
    }

    void accept_cb(Sessions::iterator session_it,
        const error_code& error)
    {
        if (!error) {
            std::lock_guard<std::mutex> guard(lock);
            std::cout << "Session accepted"
                      << " (now " << sessions.size() << " sessions)"
                      << std::endl;
            io_service.post(
                [this, session_it]() {
                    read(session_it);
                });
        } else {
            io_service.post(
                [this, session_it]() {
                    close(session_it);
                });
        }
        io_service.post([this]() { accept(); });
    }

    void close(Sessions::iterator session_it)
    {
        std::lock_guard<std::mutex> guard(lock);
        sessions.erase(session_it);
        std::cout << "Session closed"
                  << " (now " << sessions.size() - 1 << " sessions)"
                  << std::endl;
    }

    void read(Sessions::iterator session_it)
    {
        auto& socket = session_it->socket();
        auto ctx = make_context();
        auto buf = boost::asio::buffer(&(ctx->request.buffer[0]), ctx->request.buffer.size());
        socket.async_read_some(
            buf,
            [this, session_it, ctx](const error_code& error,
                size_t bytes_transferred) {
                read_cb(session_it, ctx, error, bytes_transferred);
            });
    }

    void read_cb(Sessions::iterator session_it,
        ContextPtr ctx,
        const error_code& error,
        size_t bytes_received)
    {
        if (!error) {
            http_impl(options, ctx, bytes_received);

            std::cout << ctx->request.method << ", "
                      << ctx->request.path
                      << std::endl;

            std::cout << ctx->response.status << ", "
                      << ctx->response.content_length
                      << std::endl;

            // std::cout << "Data received" << std::endl
            //           << ctx->request.view() << std::endl
            //           << " (" << bytes_received << " bytes)"
            //           << std::endl;

            //std::reverse(ctx->request.buffer.begin(), ctx->request.buffer.end());

            auto& socket = session_it->socket();
            auto buf = boost::asio::buffer(&(ctx->response.buffer[0]), ctx->response.buffer.size());
            socket.async_write_some(
                buf,
                [this, session_it, ctx](const error_code& error,
                    size_t bytes_transferred) {
                    write_cb(session_it, ctx, error, bytes_transferred);
                });
        } else {
            io_service.post(
                [this, session_it]() {
                    close(session_it);
                });
        }
    }

    void write_cb(Sessions::iterator session_it,
        ContextPtr /*ctx*/,
        const error_code& error,
        size_t /*bytes_transferred*/)
    {
        if (!error) {
            //std::cout << "Response sended" << std::endl;
            io_service.post(
                [this, session_it]() {
                    //read(session_it);
                    close(session_it);
                });
        } else {
            io_service.post(
                [this, session_it]() {
                    close(session_it);
                });
        }
    }
};
