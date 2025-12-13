#include <iostream>
#include <boost/asio.hpp>
#include "client.h"

using boost::asio::ip::tcp;

// TODO: Replace with your GCP Server Public IP
const std::string DEFAULT_SERVER_IP = "34.45.120.67"; 

int main(int argc, char* argv[]) {

    try {
        std::string host = DEFAULT_SERVER_IP;
        unsigned short port = 12345;

        if (argc > 1) host = argv[1];
        if (argc > 2) port = static_cast<unsigned short>(std::stoi(argv[2]));

        std::string username;
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
        if (username.empty()) username = "user";

        boost::asio::io_context io;
        auto work = boost::asio::make_work_guard(io);
        ChatClient client(io, host, port);

        std::cout << "🚀 Connecting to " << host << ":" << port << std::endl;
        client.start();

        std::thread t([&io]() { io.run(); });

        std::string line;
        std::string current_room;

        while (std::getline(std::cin, line)) {
            if (line.empty()) continue;

            geeks::ChatMessage msg;
            msg.set_from_user(username);

            if (line.rfind("/join ", 0) == 0) {
                std::string rest = line.substr(6);
                if (rest.empty()) {
                    std::cout << "Usage: /join <room_id> [password]\n";
                    continue;
                }
                
                std::string room_id;
                std::string password;
                
                size_t space_pos = rest.find(' ');
                if (space_pos != std::string::npos) {
                    room_id = rest.substr(0, space_pos);
                    password = rest.substr(space_pos + 1);
                } else {
                    room_id = rest;
                }

                current_room = room_id;
                msg.set_type(geeks::ChatMessage::JOIN);
                msg.set_room_id(current_room);
                msg.set_password(password);
                msg.set_text("Joined room " + current_room); // Optional text
                std::cout << "Joining room: " << current_room << "...\n";
            } else if (line == "/leave") {
                if (current_room.empty()) {
                    std::cout << "You are not in a room.\n";
                    continue;
                }
                msg.set_type(geeks::ChatMessage::LEAVE);
                msg.set_room_id(current_room);
                std::cout << "Leaving room: " << current_room << "...\n";
                current_room.clear();
            } else if (line == "/quit" || line == "/exit") {
                std::cout << "Exiting application...\n";
                client.close();
                break;
            } else {
                if (current_room.empty()) {
                    std::cout << "You must join a room first! Use /join <room_id>\n";
                    continue;
                }
                msg.set_type(geeks::ChatMessage::MESSAGE);
                msg.set_room_id(current_room);
                msg.set_text(line);
            }
            
            client.write(msg);
        }

        work.reset();
        t.join();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

}
