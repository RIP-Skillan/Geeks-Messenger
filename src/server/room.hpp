#pragma once
#include <set>
#include <memory>
#include <iostream>
#include "protocol.pb.h"
#include "session.h"

class Session;

class Room {
public:
    Room(const std::string& password = "") : password_(password) {}

    bool check_password(const std::string& pwd) const {
        return password_ == pwd;
    }

    void join(std::shared_ptr<Session> participant) {
        participants_.insert(participant);
        std::cout << "User joined room. Total: " << participants_.size() << "\n";
        
        geeks::ChatMessage join_msg;
        join_msg.set_type(geeks::ChatMessage::MESSAGE); // Use MESSAGE type for display
        join_msg.set_from_user("System");
        join_msg.set_text(participant->username() + " has joined the room.");
        broadcast(join_msg);
    }

    void leave(std::shared_ptr<Session> participant) {
        participants_.erase(participant);
        std::cout << "User left room. Total: " << participants_.size() << "\n";
        
        if (!participant->username().empty()) {
            geeks::ChatMessage leave_msg;
            leave_msg.set_type(geeks::ChatMessage::MESSAGE);
            leave_msg.set_from_user("System");
            leave_msg.set_text(participant->username() + " has left the room.");
            broadcast(leave_msg);
        }
    }

    void broadcast(const geeks::ChatMessage& msg) {
        for (auto& participant : participants_) {
            participant->deliver(msg);
        }
    }

private:
    std::set<std::shared_ptr<Session>> participants_;
    std::string password_;
};

#include <map>
#include <mutex>

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
