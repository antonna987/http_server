#pragma once

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>

namespace po = boost::program_options;

const size_t NTHREADS = 16;

struct Options {
    Options(int argc, char* argv[])
        : desc("Allowed options")
    {
        desc.add_options()("help", "produce help message")("ip,h", po::value<std::string>(), "ip address")("port,p", po::value<int>(), "listening port")("dir,d", po::value<std::string>(), "working directory");
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }

    bool help_requested() const
    {
        return vm.count("help");
    }

    const std::string& help() const
    {
        static const std::string help = [&]() {
            std::stringstream ss;
            desc.print(ss);
            return ss.str();
        }();
        return help;
    }

    int port() const
    {
        if (!vm.count("port")) {
            throw std::runtime_error("missing param ip");
        }
        return vm["port"].as<int>();
    }

    const std::string& dir() const
    {
        if (!vm.count("dir")) {
            throw std::runtime_error("missing param ip");
        }
        return vm["dir"].as<std::string>();
    }

    boost::asio::ip::address ip() const
    {
        if (!vm.count("ip")) {
            throw std::runtime_error("missing param ip");
        }
        const auto& ip_str = vm["ip"].as<std::string>();
        boost::system::error_code error;
        auto addr = boost::asio::ip::address::from_string(ip_str, error);
        if (error) {
            throw std::runtime_error("failed to parse ip address");
        }
        return addr;
    }

    size_t nthreads() const
    {
        assert(NTHREADS);
        return NTHREADS;
    }

private:
    po::options_description desc;
    po::variables_map vm;
};
