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


#include <memory>
#include <unordered_map>
#include "Room.hpp"
#include "Connection.hpp"


#pragma once


class ServerRoom {
public:
    ServerRoom();

    bool tryToCreate(const RoomID &id, const std::string &data);
    uint32_t addPlayers(const Connection &connection, uint32_t n);

    typedef enum Status {
        OK,
        DeleteMe
    } Status;

    Status removeConnection(const Connection &connection, std::vector<StringLcl> *toLogs);

    Status update(std::vector<StringLcl> *toLogs);
private:
    std::unique_ptr<Room> room;
    RemotePlayers players;
    std::unordered_map<uint32_t, Connection> connections;

    Status update(boost::optional<std::tuple<sf::Packet, sf::IpAddress>> &received, RoomOutputProtocol p);
};