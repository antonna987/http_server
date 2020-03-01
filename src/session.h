#pragma once

#include <boost/asio.hpp>

class Session {
    boost::asio::io_service& m_io_service;
    boost::asio::ip::tcp::socket m_socket;

public:
    Session(boost::asio::io_service& io_service)
        : m_io_service(io_service)
        , m_socket(m_io_service)
    {
    }
    boost::asio::ip::tcp::socket& socket() { return m_socket; }
};
