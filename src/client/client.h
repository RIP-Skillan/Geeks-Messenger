#pragma once
#include <boost/asio.hpp>
#include <string>
#include <deque>
#include <vector>
#include "protocol.pb.h"

using boost::asio::ip::tcp;

class ChatClient {
public:
    ChatClient(boost::asio::io_context& io,
               const std::string& host,
               unsigned short port);

    void start();
    void write(const geeks::ChatMessage& msg); 
    void close(); 

private:
    void do_write();
    void do_connect();
    void read_header();
    void read_body(std::size_t length);

    boost::asio::io_context& io_;
    tcp::resolver resolver_;
    tcp::socket socket_;
    std::string host_;
    unsigned short port_;
    std::array<char, 4> header_;
    std::vector<char> body_;
    std::deque<std::vector<uint8_t>> write_queue_;
};
