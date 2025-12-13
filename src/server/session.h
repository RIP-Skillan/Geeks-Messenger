#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <deque>
#include "protocol.pb.h"

using boost::asio::ip::tcp;

class RoomManager;
class Room;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, RoomManager& room_manager);
    void start();

    void deliver(const geeks::ChatMessage& msg);

private:
    void read_header();
    void read_body(std::size_t length);
    void do_write();

    tcp::socket socket_;
    RoomManager& room_manager_;
    std::shared_ptr<Room> room_;
    std::string username_;
    std::array<char, 4> header_;
    std::vector<char> body_;
    std::deque<std::vector<uint8_t>> write_queue_;

public:
    std::string username() const { return username_; }
};
