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
#include "Room.hpp"
#include "RemotePlayers.hpp"


#pragma once


class ServerRooms {
public:
	ServerRooms();

	void createIfValid(RoomID id, const std::string& saveData);
	void close(RoomID id);
	bool exist(RoomID id) const;

	void update(RoomID id, const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<std::tuple<sf::Packet, sf::IpAddress>> *toSend);
	void updateAll(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend);

	uint32_t playersToAdd(RoomID id) const;
	uint32_t getCurrentPlayerNumber(RoomID id);
	uint32_t getPlayerLimit(RoomID id);

	void addPlayer(RoomID id, sf::IpAddress playerIP);
	void addPlayers(RoomID id, sf::IpAddress playerIP, uint32_t number);

	bool addPlayerSafe(RoomID id, sf::IpAddress playerIP);
	uint32_t addPlayersSafe(RoomID id, sf::IpAddress playerIP, uint32_t number);
private:
	std::unordered_map<uint64_t, std::tuple<std::unique_ptr<Room>, RemotePlayers>> data;
};