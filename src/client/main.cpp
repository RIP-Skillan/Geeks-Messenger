#include <iostream>
#include <boost/asio.hpp>
#include "client.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {

    try {
        std::string host = "127.0.0.1";
        unsigned short port = 12345;

        if (argc > 1) host = argv[1];
        if (argc > 2) port = static_cast<unsigned short>(std::stoi(argv[2]));

        boost::asio::io_context io;
        ChatClient client(io, host, port);

        std::cout << "🚀 Connecting to " << host << ":" << port << std::endl;
        client.start();
        io.run();
        
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
