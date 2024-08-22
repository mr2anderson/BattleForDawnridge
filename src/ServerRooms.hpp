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


#include <unordered_map>
#include <cstdint>
#include <set>
#include "Room.hpp"
#include "RemotePlayers.hpp"


#pragma once


class ServerRooms {
public: 
	ServerRooms();

	void createIfValid(const RoomID & id, const std::string& saveData);
	void close(const RoomID& id);
	bool exist(const RoomID& id) const;
	std::set<sf::IpAddress> updateAll(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<StringLcl> *toLogs, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend);

	uint32_t playersToAdd(const RoomID& id) const;
	uint32_t getCurrentPlayerNumber(const RoomID& id);
	uint32_t getPlayerLimit(const RoomID& id);

	bool addPlayerSafe(const RoomID& id, sf::IpAddress playerIP);
	uint32_t addPlayersSafe(const RoomID& id, sf::IpAddress playerIP, uint32_t number);
private:
	std::unordered_map<std::string, std::tuple<std::unique_ptr<Room>, RemotePlayers>> data;
};