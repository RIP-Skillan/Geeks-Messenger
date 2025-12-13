#include "client.h"
#include <iostream>
#include "protocol.pb.h"

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
                read_header();
            } else {
                std::cerr << "Connection failed: " << ec.message() << "\n";
            }
        }
    );
}

void ChatClient::read_header() {
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(header_),
        [this](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                uint32_t length = 0;
                std::memcpy(&length, header_.data(), 4);
                length = ntohl(length);
                read_body(length);
            } else {
                std::cerr << "Disconnected from server: " << ec.message() << "\n";
                socket_.close();
            }
        });
}

void ChatClient::read_body(std::size_t length) {
    body_.resize(length);
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(body_),
        [this](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                geeks::ChatMessage msg;
                if (msg.ParseFromArray(body_.data(), body_.size())) {
                    if (msg.type() == geeks::ChatMessage::MESSAGE) {
                        std::cout << "\n[" << msg.from_user() << "] " << msg.text() << "\n";
                    } else if (msg.type() == geeks::ChatMessage::JOIN) {
                         std::cout << "\n[System] " << msg.from_user() << " joined the room.\n";
                    } else if (msg.type() == geeks::ChatMessage::ERROR) {
                         std::cout << "\n[Error] " << msg.text() << "\n";
                    }
                }
                read_header();
            } else {
                std::cerr << "Error reading body: " << ec.message() << "\n";
                socket_.close();
            }
        });
}

void ChatClient::write(const geeks::ChatMessage& msg) {
    std::string payload;
    if (!msg.SerializeToString(&payload)) {
        std::cerr << "Failed to serialize message\n";
        return;
    }

    uint32_t len = static_cast<uint32_t>(payload.size());
    uint32_t net_len = htonl(len); // Convert to Network Byte Order

    std::vector<uint8_t> buffer(4 + len);
    std::memcpy(buffer.data(), &net_len, 4);
    std::memcpy(buffer.data() + 4, payload.data(), len);

    boost::asio::post(io_, [this, buffer = std::move(buffer)]() mutable {
        bool write_in_progress = !write_queue_.empty();
        write_queue_.push_back(std::move(buffer));

        if (!write_in_progress) {
            do_write();
        }
    });
}

void ChatClient::do_write() {
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(write_queue_.front()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                write_queue_.pop_front();
                if (!write_queue_.empty()) {
                    do_write();
                }
            } else {
                std::cerr << "Write failed: " << ec.message() << "\n";
                socket_.close();
            }
        });
}
