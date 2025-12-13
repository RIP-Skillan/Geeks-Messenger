#include "session.h"
#include "room.hpp"
#include <iostream>
#include "protocol.pb.h"

Session::Session(tcp::socket socket, RoomManager& room_manager)
    : socket_(std::move(socket)), room_manager_(room_manager) {}

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
                if (room_) {
                    room_->leave(shared_from_this());
                    if (room_->empty()) {
                        room_manager_.remove_room(room_->id());
                    }
                }
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
                geeks::ChatMessage msg;
                if (msg.ParseFromArray(body_.data(), body_.size())) {
                    if (msg.type() == geeks::ChatMessage::JOIN) {
                        username_ = msg.from_user();
                        std::cout << "User " << username_ << " joining room " << msg.room_id() << "\n";
                        if (room_) {
                            room_->leave(shared_from_this());
                            if (room_->empty()) {
                                room_manager_.remove_room(room_->id());
                            }
                        }
                        auto new_room = room_manager_.get_or_create_room(msg.room_id(), msg.password());
                        if (new_room) {
                            room_ = new_room;
                            room_->join(shared_from_this());
                        } else {
                            // Send error message back to client
                            geeks::ChatMessage error_msg;
                            error_msg.set_type(geeks::ChatMessage::ERROR);
                            error_msg.set_text("Incorrect password or room error");
                            deliver(error_msg);
                        }
                    } else if (msg.type() == geeks::ChatMessage::MESSAGE) {
                        if (room_) {
                            room_->broadcast(msg);
                        } else {
                            std::cout << "User " << msg.from_user() << " tried to send message without joining a room.\n";
                        }
                    } else if (msg.type() == geeks::ChatMessage::LEAVE) {
                        if (room_) {
                            std::cout << "User " << username_ << " leaving room " << msg.room_id() << "\n";
                            room_->leave(shared_from_this());
                            if (room_->empty()) {
                                room_manager_.remove_room(room_->id());
                            }
                            room_.reset();
                        }
                    }
                }
                read_header();
            } else {
                std::cout << "Error reading body: " << ec.message() << "\n";
                if (room_) {
                    room_->leave(shared_from_this());
                    if (room_->empty()) {
                        room_manager_.remove_room(room_->id());
                    }
                }
            }
        });
}

void Session::deliver(const geeks::ChatMessage& msg) {
    std::string payload;
    msg.SerializeToString(&payload);

    uint32_t len = static_cast<uint32_t>(payload.size());
    uint32_t net_len = htonl(len);

    std::vector<uint8_t> buffer(4 + len);
    std::memcpy(buffer.data(), &net_len, 4);
    std::memcpy(buffer.data() + 4, payload.data(), len);

    bool write_in_progress = !write_queue_.empty();
    write_queue_.push_back(std::move(buffer));

    if (!write_in_progress) {
        do_write();
    }
}

void Session::do_write() {
    auto self = shared_from_this();
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(write_queue_.front()),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                write_queue_.pop_front();
                if (!write_queue_.empty()) {
                    do_write();
                }
            } else {
                std::cout << "Write error: " << ec.message() << "\n";
                if (room_) {
                    room_->leave(shared_from_this());
                    if (room_->empty()) {
                        room_manager_.remove_room(room_->id());
                    }
                }
            }
        });
}
