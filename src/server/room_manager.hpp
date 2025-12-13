#pragma once
#include <map>
#include <memory>
#include <string>
#include <mutex>
#include "room.hpp"

class RoomManager {
public:
    std::shared_ptr<Room> get_or_create_room(const std::string& room_id, const std::string& password) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = rooms_.find(room_id);
        if (it != rooms_.end()) {
            if (it->second->check_password(password)) {
                return it->second;
            } else {
                std::cout << "Incorrect password for room: " << room_id << "\n";
                return nullptr;
            }
        }
        
        auto new_room = std::make_shared<Room>(password);
        rooms_[room_id] = new_room;
        std::cout << "Created new room: " << room_id << " with password: " << password << "\n";
        return new_room;
    }

private:
    std::map<std::string, std::shared_ptr<Room>> rooms_;
    std::mutex mutex_;
};
