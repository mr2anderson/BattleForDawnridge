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


#include <iostream>
#include "LocalServer.hpp"
#include "Ports.hpp"
#include "ClientNetSpecs.hpp"
#include "math.hpp"
#include "RoomWasClosed.hpp"


static void F(std::tuple<std::shared_ptr<Room>, std::atomic_bool*> p) {
	uint16_t sendPort = Ports::get()->getLocalServerSendPort();
	uint16_t receivePort = Ports::get()->getLocalServerReceivePort();
	uint16_t clientSendPort = Ports::get()->getClientSendPort();
	uint16_t clientReceivePort = Ports::get()->getClientReceivePort();

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress clientIP = sf::IpAddress::getLocalAddress();

	sf::UdpSocket sendSocket;
	sendSocket.setBlocking(false);
	if (sendSocket.bind(sendPort) != sf::Socket::Done) {
		std::cerr << "LocalServer: warning: unable to bind send port" << std::endl;
		return;
	}
	sf::UdpSocket receiveSocket;
	receiveSocket.setBlocking(false);
	if (receiveSocket.bind(receivePort) != sf::Socket::Done) {
		std::cerr << "LocalServer: warning: unable to bind receive port" << std::endl;
		return;
	}

	boost::optional<std::tuple<sf::Packet, sf::IpAddress>> received;
	std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSend;

	RemotePlayers players;
	for (uint32_t i = 1; i <= std::get<0>(p)->playersNumber(); i = i + 1) {
		players.add(RemotePlayer(i, clientIP));
	}

	for (; ;) {
		Clock clock;

		while (!toSend.empty()) {
			sendSocket.send(std::get<0>(toSend.back()), std::get<1>(toSend.back()), clientReceivePort);
			toSend.pop_back();
		}

		received = boost::none;
		sf::Packet receivedPacket;
		sf::IpAddress senderIP;
		uint16_t senderPort;
		if (receiveSocket.receive(receivedPacket, senderIP, senderPort) == sf::Socket::Status::Done and senderIP == clientIP and senderPort == clientSendPort) {
			received = std::make_tuple(receivedPacket, senderIP);
		}

		try {
			std::get<0>(p)->update(received, &toSend, players);
		}
		catch (RoomWasClosed&) {
			break;
		}

		if (*std::get<1>(p)) {
			break;
		}

		sf::sleep(sf::milliseconds(bfdlib::math::subu<uint32_t>(1000 / 60, clock.getMS())));
	}
}


LocalServer::LocalServer() {
	this->thread = nullptr;
}
LocalServer::~LocalServer() {
	if (this->thread != nullptr) {
		this->stopThread = true;
		this->thread->wait();
	}
}
void LocalServer::launch(std::shared_ptr<Room> room) {
	this->stopThread = false;
	this->thread = std::make_unique<sf::Thread>(&F, std::make_tuple(room, &this->stopThread));
	this->thread->launch();
}