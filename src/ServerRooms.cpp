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


void ServerRooms::createIfValid(const RoomID & id, const std::string& saveData) {
	try {
		std::unique_ptr<Room> roomPtr = std::make_unique<Room>(id, saveData, Room::Restrictions::Enable);
		std::tuple<std::unique_ptr<Room>, RemotePlayers> val = std::make_tuple(std::move(roomPtr), RemotePlayers());
		this->data[id.value()] = std::move(val);
	}
	catch (std::exception&) {

	}
}
bool ServerRooms::exist(const RoomID& id) const {
	return (this->data.find(id.value()) != this->data.end());
}

void ServerRooms::removePlayer(sf::IpAddress ip, std::vector<StringLcl>* toLogs) {
    std::vector<std::string> toRemove;

    for (auto &p : this->data) {
        bool hasValid = false;
        for (uint32_t i = 1; i <= std::get<RemotePlayers>(p.second).size(); i = i + 1) {
            if (std::get<RemotePlayers>(p.second).get(i).getIp() == ip) {
                std::get<RemotePlayers>(p.second).disconnect(i);
                toLogs->emplace_back("{removing_player_from_room} " + ip.toString() + " " + std::get<std::unique_ptr<Room>>(p.second)->getID().value());
            }
            hasValid = hasValid or std::get<RemotePlayers>(p.second).connected(i);
        }
        if (!hasValid) {
            toLogs->emplace_back("{removing_room_cuz_all_players_were_removed} " + std::get<std::unique_ptr<Room>>(p.second)->getID().value());
            toRemove.push_back(p.first);
        }
    }

    for (const auto &k : toRemove) {
        this->data.erase(k);
    }
}

std::set<sf::IpAddress> ServerRooms::updateAll(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<StringLcl>* toLogs, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend) {
	std::set<sf::IpAddress> toClose;
	
	auto it = this->data.begin();
	while (it != this->data.end()) {
		std::vector<StringLcl> logs;
		RoomID id = std::get<std::unique_ptr<Room>>(it->second)->getID();
		try {
			RoomOutputProtocol p;
			p.logs = &logs;
			p.toSend = toSend;
			p.remotePlayers = &std::get<RemotePlayers>(it->second);
			std::get<std::unique_ptr<Room>>(it->second)->update(received, p);
			it++;
		}
		catch (std::exception& e) {
			RemotePlayers players = std::get<RemotePlayers>(it->second);
			for (uint32_t i = 1; i <= players.size(); i = i + 1) {
				toClose.insert(players.get(i).getIp());
			}
			it = this->data.erase(it);
			logs.emplace_back("{closed_on_exception}" + std::string(e.what()));
		}
		for (const auto& a : logs) {
			toLogs->emplace_back("{room} " + id.value() + ": " + a.toRawString());
		}
	}

	return toClose;
}

uint32_t ServerRooms::playersToAdd(const RoomID& id) const {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	return std::get<std::unique_ptr<Room>>(it->second)->playersNumber() - std::get<RemotePlayers>(it->second).size();
}
uint32_t ServerRooms::getCurrentPlayerNumber(const RoomID& id) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	return std::get<RemotePlayers>(it->second).size();
}
uint32_t ServerRooms::getPlayerLimit(const RoomID& id) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	return std::get<std::unique_ptr<Room>>(it->second)->playersNumber();
}

bool ServerRooms::addPlayerSafe(const RoomID& id, sf::IpAddress playerIP) {
	return this->addPlayersSafe(id, playerIP, 1);
}
uint32_t ServerRooms::addPlayersSafe(const RoomID& id, sf::IpAddress playerIP, uint32_t number) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}

	uint32_t ctr = 0;

	while (number and std::get<std::unique_ptr<Room>>(it->second)->playersNumber() > std::get<RemotePlayers>(it->second).size()) {
		std::get<RemotePlayers>(it->second).add(playerIP);
		number = number - 1;
		ctr = ctr + 1;
	}

    if (ctr) {
        std::get<std::unique_ptr<Room>>(it->second)->needInit();
    }

	return ctr;
}