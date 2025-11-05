#include "server.hpp"

int main() {
    try {
        boost::asio::io_context io;
        ChatServer server(io, 9000);
        server.start();
        io.run();
    } catch (std::exception& e) {
        std::cerr << "[Fatal] " << e.what() << std::endl;
    }
    return 0;
}