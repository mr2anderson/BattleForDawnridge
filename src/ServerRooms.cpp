/*
 *  Battle for Dawnridge
 *  Copyright (C) 2024 mr2anderson
 *
 *  Battle for Dawnridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Battle for Dawnridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battle for Dawnridge.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "ServerRooms.hpp"
#include "PlayerLimitReached.hpp"
#include "RoomDoesNotExist.hpp"


ServerRooms::ServerRooms() = default;
void ServerRooms::update(std::vector<StringLcl> *toLogs) {
    this->updateRooms(toLogs);
}

bool ServerRooms::exist(const RoomID &id) const {
    return (this->data.find(id.value()) != this->data.end());
}
void ServerRooms::createIfValid(const RoomID &id, const std::string &rd) {
    if (!this->data[id.value()].tryToCreate(id, rd)) {
        this->data.erase(id.value());
    }
}
uint32_t ServerRooms::addPlayers(const RoomID &id, const Connection &connection, uint32_t n) {
    auto it = this->data.find(id.value());
    if (it == this->data.end()) {
        return 0;
    }
    return it->second.addPlayers(connection, n);
}

void ServerRooms::removeConnection(const Connection &connection, std::vector<StringLcl> *toLogs) {
    std::vector<std::string> toDelete;
    for (auto &room : this->data) {
        if (room.second.removeConnection(connection, toLogs) == ServerRoom::Status::DeleteMe) {
            toDelete.push_back(room.first);
        }
    }
    for (const std::string &k : toDelete) {
        this->data.erase(k);
    }
}

void ServerRooms::updateRooms(std::vector<StringLcl> *toLogs) {
    std::vector<std::string> toDelete;
    for (auto &room : this->data) {
        if (room.second.update(toLogs) == ServerRoom::Status::DeleteMe) {
            toDelete.push_back(room.first);
        }
    }
    for (const std::string &k : toDelete) {
        this->data.erase(k);
    }
}