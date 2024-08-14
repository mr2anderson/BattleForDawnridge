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


#include "LocalServer.hpp"
#include "Ports.hpp"
#include "ClientNetSpecs.hpp"
#include "math.hpp"
#include "RoomWasClosed.hpp"
#include "PortIsBusy.hpp"
#include "LocalServerAlreadyLaunched.hpp"


static void F(std::shared_ptr<Room> room, sf::UdpSocket *sendSocket, sf::UdpSocket *receiveSocket, std::exception_ptr *error, std::atomic_bool* p) {
	boost::optional<std::tuple<sf::Packet, sf::IpAddress>> received;
	std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSend;

	RemotePlayers players;
	for (uint32_t i = 1; i <= room->playersNumber(); i = i + 1) {
		players.add(RemotePlayer(i, sf::IpAddress::getLocalAddress()));
	}

	for (; ;) {
		Clock clock;

		while (!toSend.empty()) {
			sendSocket->send(std::get<0>(toSend.back()), std::get<1>(toSend.back()), Ports::get()->getClientReceivePort());
			toSend.pop_back();
		}

		received = boost::none;
		sf::Packet receivedPacket;
		sf::IpAddress senderIP;
		uint16_t senderPort;
		if (receiveSocket->receive(receivedPacket, senderIP, senderPort) == sf::Socket::Status::Done and senderIP == sf::IpAddress::getLocalAddress() and senderPort == Ports::get()->getClientSendPort()) {
			received = std::make_tuple(receivedPacket, senderIP);
		}

		try {
			room->update(received, &toSend, players);
		}
		catch (RoomWasClosed&) {
			break;
		}
		catch (std::exception& e) {
			*error = std::current_exception();
			break;
		}

		if (*p) {
			break;
		}

		sf::sleep(sf::milliseconds(bfdlib::math::subu<uint32_t>(1000 / 60, clock.getMS())));
	}
}


LocalServer::LocalServer() {
	this->stopThread = false;
	this->thread = nullptr;

	this->sendSocket.setBlocking(false);
	if (this->sendSocket.bind(Ports::get()->getLocalServerSendPort()) != sf::Socket::Done) {
		throw PortIsBusy(Ports::get()->getLocalServerSendPort());
	}
	this->receiveSocket.setBlocking(false);
	if (this->receiveSocket.bind(Ports::get()->getLocalServerReceivePort()) != sf::Socket::Done) {
		throw PortIsBusy(Ports::get()->getLocalServerReceivePort());
	}
}
LocalServer::~LocalServer() {
	if (this->thread != nullptr) {
		this->stopThread = true;
		this->thread->wait();
	}
}
void LocalServer::launch(std::shared_ptr<Room> room) {
	if (this->thread != nullptr) {
		throw LocalServerAlreadyLaunched();
	}
	this->thread = std::make_unique<sf::Thread>(std::bind(&F, room, &this->sendSocket, &this->receiveSocket, &this->threadError, &this->stopThread));
	this->thread->launch();
}
void LocalServer::fine() {
	if (this->threadError != nullptr) {
		std::rethrow_exception(this->threadError);
	}
}