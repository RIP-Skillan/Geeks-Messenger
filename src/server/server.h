#pragma once
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket);
    void start();

private:
    void do_read();

    tcp::socket socket_;
    std::array<char, 1024> buffer_;
};

