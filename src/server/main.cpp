#include "server.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    try {
        unsigned short port = 12345;

        if (argc > 1) {
            port = static_cast<unsigned short>(std::atoi(argv[1]));
        }

        boost::asio::io_context io;
        ChatServer server(io, port);

        std::cout << "Server started on port " << port << std::endl;
        server.start();
        io.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
