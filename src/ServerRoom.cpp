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


#include "ServerRoom.hpp"
#include "NoActivePlayers.hpp"


ServerRoom::ServerRoom() = default;

bool ServerRoom::tryToCreate(const RoomID &id, const std::string &data) {
    try {
        this->room = std::make_unique<Room>(id, data, Room::Restrictions::Enable);
        return true;
    }
    catch (std::exception&) {
        return false;
    }
}
uint32_t ServerRoom::addPlayers(const Connection &connection, uint32_t n) {
    uint32_t added = 0;
    for (uint32_t i = 1; i <= this->room->playersNumber(); i = i + 1) {
        if (added == n) {
            break;
        }
        bool free = (this->connections.find(this->players.get(i).getIp().toInteger()) == this->connections.end());
        if (free) {
            if (this->connections.find(connection.getIP().toInteger()) == this->connections.end()) {
                this->connections[connection.getIP().toInteger()] = connection;
            }
            this->players.set(RemotePlayer(i, connection.getIP()));
            added = added + 1;
        }
    }
    if (added) {
        this->room->mustSendInit();
    }
    return added;
}

ServerRoom::Status ServerRoom::removeConnection(const Connection &connection, std::vector<StringLcl> *toLogs) {
    toLogs->emplace_back("{connection_erased}" + connection.getIP().toString());
    this->connections.erase(connection.getIP().toInteger());
    if (this->connections.empty()) {
        toLogs->emplace_back("{room_will_be_removed_reason_no_connected_players}" + room->getID().value());
        return Status::DeleteMe;
    }
    return Status ::OK;
}

ServerRoom::Status ServerRoom::update(std::vector<StringLcl> *toLogs) {
    std::vector<uint32_t> toDelete;
    for (auto &connection : this->connections) {
        connection.second.update();
        if (connection.second.hasError()) {
            toLogs->emplace_back("{connection_erased_reason_disconnect}" + connection.second.getIP().toString());
            toDelete.push_back(connection.first);
        }
    }
    for (auto a : toDelete) {
        connections.erase(a);
    }
    if (this->connections.empty()) {
        toLogs->emplace_back("{room_will_be_removed_reason_no_connected_players}" + room->getID().value());
        return Status::DeleteMe;
    }

    std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSend;
    RoomOutputProtocol protocol;
    protocol.logs = toLogs;
    protocol.remotePlayers = &this->players;
    protocol.toSend = &toSend;
    for (auto &connection : this->connections) {
        std::optional<sf::Packet> receivedPacket = connection.second.getReceivedPacket();
        if (receivedPacket != std::nullopt) {
            boost::optional<std::tuple<sf::Packet, sf::IpAddress>> received = std::make_tuple(receivedPacket.value(), connection.second.getIP());
            if (this->update(received, protocol) == Status::DeleteMe) {
                return Status::DeleteMe;
            }
        }
    }
    boost::optional<std::tuple<sf::Packet, sf::IpAddress>> received = boost::none;
    if (this->update(received, protocol) == Status::DeleteMe) {
        return Status::DeleteMe;
    }
    for (const auto &t : toSend) {
        auto it = this->connections.find(std::get<sf::IpAddress>(t).toInteger());
        if (it != this->connections.end()) {
            it->second.send(std::get<sf::Packet>(t));
        }
    }

    return Status::OK;
}
ServerRoom::Status ServerRoom::update(boost::optional<std::tuple<sf::Packet, sf::IpAddress>> &received, RoomOutputProtocol p) {
    try {
        std::vector<StringLcl>* originalLogsPtr = p.logs;
        std::vector<StringLcl> logs;
        p.logs = &logs;
        room->update(received, p);
        for (const auto &s : logs) {
            originalLogsPtr->emplace_back("{room} " + this->room->getID().value() + " " + s.toRawString());
        }
        return Status::OK;
    }
    catch (NoActivePlayers&) {
        p.logs->emplace_back("{room_will_be_removed_reason_no_active_players}" + room->getID().value());
        return Status::DeleteMe;
    }
    catch (std::exception& e) {
        p.logs->emplace_back("{room_will_be_removed_reason_unknown_error}" + room->getID().value() + " " + std::string(e.what()));
        return Status::DeleteMe;
    }
}