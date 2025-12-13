#include "session.h"
#include <iostream>
#include "protocol.pb.h"

Session::Session(tcp::socket socket)
    : socket_(std::move(socket)) {}

void Session::start() {
    std::cout << "Waiting for message...\n";
    read_header();
}

void Session::read_header() {
    auto self = shared_from_this();

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(header_),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint32_t length = 0;
                std::memcpy(&length, header_.data(), 4);
                length = ntohl(length);
                read_body(length);
            } else {
                std::cout << "Client disconnected: " << ec.message() << "\n";
            }
        });
}

void Session::read_body(std::size_t length) {
    body_.resize(length);
    auto self = shared_from_this();

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(body_),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::cout << "Received message of size: " << body_.size() << std::endl;

                geeks::ChatMessage msg;
                if (msg.ParseFromArray(body_.data(), body_.size())) {
                    std::cout << msg.from_user() << ": " << msg.text() << std::endl;
                }

                read_header();
            } else {
                std::cout << "Error reading body: " << ec.message() << "\n";
            }
        });
}


