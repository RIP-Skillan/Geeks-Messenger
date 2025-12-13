#include "server.hpp"
#include "session.h"

ChatServer::ChatServer(boost::asio::io_context& io, unsigned short port) : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
    // TODO: implement constructor
}

void ChatServer::start() {
    std::cout << "[Server] listening on port " << acceptor_.local_endpoint().port() << std::endl;
    do_accept();
}

void ChatServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::cout << "[Server] Client connected: " << socket.remote_endpoint() << std::endl;
                auto session = std::make_shared<Session>(std::move(socket));
                session->start();

            } else {
                std::cerr << "[Server] Accept error:" << ec.message() << std::endl;
            }
            do_accept(); // re-call to continue accepting new client
        }
    );
}