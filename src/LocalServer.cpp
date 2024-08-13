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


#include <SFML/System/Sleep.hpp>
#include <iostream>
#include "LocalServer.hpp"
#include "Ports.hpp"


static void F(std::shared_ptr<Room> room) {
	sf::UdpSocket socket;
	socket.setBlocking(false);
	if (socket.bind(Ports::get()->getLocalServerPort()) != sf::Socket::Done) {
		std::cerr << "LocalServer: warning: unable to bind port" << std::endl;
	}

	std::tuple<sf::Packet, sf::IpAddress> received;
	std::vector<sf::Packet> toSend;

	RemotePlayers players;
	for (uint32_t i = 1; i <= room->playersNumber(); i = i + 1) {
		players.add(RemotePlayer(i, sf::IpAddress::getLocalAddress()));
	}

	for (; ;) {
		Clock clock;

		room->update(received, &toSend, players);

		while (!toSend.empty()) {
			socket.send(toSend.back(), sf::IpAddress::getLocalAddress(), Ports::get()->getClientPort());
			toSend.pop_back();
		}

		sf::sleep(sf::milliseconds(1000 / 60 - clock.getMS()));
	}
}


LocalServer::LocalServer() = default;
void LocalServer::launch(std::shared_ptr<Room> room) {
	this->thread = std::make_unique<sf::Thread>(&F, room);
	this->thread->launch();
}