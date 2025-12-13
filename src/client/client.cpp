#include "client.h"
#include <iostream>

ChatClient::ChatClient(boost::asio::io_context& io, const std::string& host, unsigned short port)
    : io_(io), resolver_(io), socket_(io), host_(host), port_(port) {}

void ChatClient::start() {
    do_connect();
}

void ChatClient::do_connect() {
    auto endpoints = resolver_.resolve(host_, std::to_string(port_));

    boost::asio::async_connect(
        socket_,
        endpoints,
        [this](boost::system::error_code ec, const tcp::endpoint&) {
            if (!ec) {
                std::cout << "Connected to server\n";
            } else {
                std::cerr << "Connection failed: " << ec.message() << "\n";
            }
            socket_.close();    
        }
    );
}
