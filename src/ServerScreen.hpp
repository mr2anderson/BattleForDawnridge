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


#include <SFML/Network.hpp>
#include "Logs.hpp"
#include "ServerScreenResponse.hpp"
#include "MenuBg.hpp"
#include "ServerRooms.hpp"


#pragma once


class ServerScreen {
public:
	ServerScreen(sf::RenderWindow& window);
	ServerScreen(const ServerScreen& copy) = delete;
	ServerScreenResponse run(sf::RenderWindow& window);
private:
	bool alreadyFinished;
	MenuBg bg;
	Logs logs;
    sf::UdpSocket sendSocket;
    sf::UdpSocket receiveSocket;
	ServerRooms rooms;
	std::unordered_map<uint64_t, bool> initSignalBlocklist;

	void checkRoomInitSignal(sf::Packet& packet, sf::IpAddress ip);

	void drawEverything(sf::RenderWindow& window);
};