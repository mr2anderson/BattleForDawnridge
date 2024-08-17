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

	void finish();
	void launch();
	void fine() const;
private:
	std::exception_ptr unexpectedError;
	std::atomic<bool> stop;
	std::atomic<bool> running;
    std::atomic<uint64_t> sendTraffic;
    std::atomic<uint64_t> receiveTraffic;
	std::unique_ptr<sf::Thread> thread;
};