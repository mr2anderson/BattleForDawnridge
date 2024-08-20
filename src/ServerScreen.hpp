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
#include <atomic>
#include <forward_list>
#include "Logs.hpp"
#include "ServerScreenResponse.hpp"
#include "MenuBg.hpp"
#include "ServerRooms.hpp"
#include "tcp_helper.hpp"


#pragma once


class ServerScreen {
public:
	ServerScreen(sf::RenderWindow& window);
	ServerScreen(const ServerScreen& copy) = delete;
    ~ServerScreen();

	ServerScreenResponse run(sf::RenderWindow& window);
private:
	bool alreadyFinished;

	MenuBg bg;
	Logs logs;

	ServerRooms rooms;

    sf::TcpListener listener;
    std::atomic<bool> stop;
    std::atomic<uint64_t> traffic;
	std::forward_list<sf::TcpSocket> sockets;
    std::forward_list<bfdlib::tcp_helper::queue_r> received;
    std::forward_list<bfdlib::tcp_helper::queue_w> toSend;
    std::forward_list<sf::Thread> sendThreads;
	std::forward_list<sf::Thread> receiveThreads;

	void checkRoomInitSignal(sf::Packet& packet, sf::IpAddress ip);

	void drawEverything(sf::RenderWindow& window);
};