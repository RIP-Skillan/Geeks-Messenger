#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <vector>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket);
    void start();

private:
    void read_header();
    void read_body(std::size_t length);
    /*void do_read();*/

    tcp::socket socket_;
    std::array<char, 4> header_;
    std::vector<char> body_;
};
