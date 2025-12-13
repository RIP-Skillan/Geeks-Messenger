#pragma once
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

class ChatClient {
public:
    ChatClient(boost::asio::io_context& io,
               const std::string& host,
               unsigned short port);

    void start();

private:
    void do_connect();

    boost::asio::io_context& io_;
    tcp::resolver resolver_;
    tcp::socket socket_;
    std::string host_;
    unsigned short port_;
};
