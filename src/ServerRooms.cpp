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


void ServerRooms::createIfValid(RoomID id, const std::string& saveData) {
	try {
		std::unique_ptr<Room> roomPtr = std::make_unique<Room>(id, saveData, Room::Restrictions::Enable);
		std::tuple<std::unique_ptr<Room>, RemotePlayers> val = std::make_tuple(std::move(roomPtr), RemotePlayers());
		this->data[id.value()] = std::move(val);
	}
	catch (std::exception&) {

	}
}
void ServerRooms::close(RoomID id) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	this->data.erase(it);
}
bool ServerRooms::exist(RoomID id) const {
	return (this->data.find(id.value()) != this->data.end());
}


void ServerRooms::update(RoomID id, const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	try {
		std::get<std::unique_ptr<Room>>(it->second)->update(received, toSend, std::get<RemotePlayers>(it->second));
	}
	catch (std::exception&) {
		this->data.erase(it);
	}
}
void ServerRooms::updateAll(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend) {
	for (auto it = this->data.begin(); it != this->data.end();) {
		try {
			std::get<std::unique_ptr<Room>>(it->second)->update(received, toSend, std::get<RemotePlayers>(it->second));
			it++;
		}
		catch (std::exception&) {
			this->data.erase(it);
		}
	}
}

uint32_t ServerRooms::playersToAdd(RoomID id) const {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	return std::get<std::unique_ptr<Room>>(it->second)->playersNumber() - std::get<RemotePlayers>(it->second).size();
}
uint32_t ServerRooms::getCurrentPlayerNumber(RoomID id) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	return std::get<RemotePlayers>(it->second).size();
}
uint32_t ServerRooms::getPlayerLimit(RoomID id) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	return std::get<std::unique_ptr<Room>>(it->second)->playersNumber();
}

void ServerRooms::addPlayer(RoomID id, sf::IpAddress playerIP) {
	this->addPlayers(id, playerIP, 1);
}
void ServerRooms::addPlayers(RoomID id, sf::IpAddress playerIP, uint32_t number) {
	auto it = this->data.find(id.value());
	if (it == this->data.end()) {
		throw RoomDoesNotExist();
	}
	if (std::get<RemotePlayers>(it->second).size() + number > std::get<std::unique_ptr<Room>>(it->second)->playersNumber()) {
		throw PlayerLimitReached();
	}
	for (uint32_t i = 0; i < number; i = i + 1) {
		std::get<RemotePlayers>(it->second).add(playerIP);
	}
    if (number) {
        std::get<std::unique_ptr<Room>>(it->second)->needInit();
    }
}

bool ServerRooms::addPlayerSafe(RoomID id, sf::IpAddress playerIP) {
	return this->addPlayersSafe(id, playerIP, 1);
}
uint32_t ServerRooms::addPlayersSafe(RoomID id, sf::IpAddress playerIP, uint32_t number) {
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