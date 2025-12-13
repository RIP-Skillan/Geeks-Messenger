#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include "room.hpp"

using boost::asio::ip::tcp;

class ChatServer {
public: 
    ChatServer(boost::asio::io_context& io, uint16_t port);
    void start();

private:
    void do_accept();

    tcp::acceptor acceptor_;
    RoomManager room_manager_;
};