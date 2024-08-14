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


#include <SFML/System/Thread.hpp>
#include <atomic>
#include "Room.hpp"


#pragma once


class LocalServer {
public:
	LocalServer();
	LocalServer(const LocalServer& copy) = delete;
	~LocalServer();

	void launch(std::shared_ptr<Room> room);
	void fine();
private:
	sf::UdpSocket sendSocket, receiveSocket;
	std::exception_ptr threadError;
	std::atomic<bool> stopThread;
	std::unique_ptr<sf::Thread> thread;
};