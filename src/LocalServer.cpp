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
#include "ClientNetSpecs.hpp"
#include "math.hpp"
#include "RoomWasClosed.hpp"
#include "PortIsBusy.hpp"
#include "LocalServerAlreadyLaunched.hpp"
#include "ServerNetSpecs.hpp"








static void THREAD(std::exception_ptr* unexpectedError, std::atomic_bool* stop) {
	sf::UdpSocket sendSocket, receiveSocket;
	sendSocket.setBlocking(false);
	if (sendSocket.bind(SERVER_NET_SPECS::PORTS::SEND) != sf::Socket::Done) {
		throw PortIsBusy(SERVER_NET_SPECS::PORTS::SEND);
	}
	receiveSocket.setBlocking(false);
	if (receiveSocket.bind(SERVER_NET_SPECS::PORTS::RECEIVE) != sf::Socket::Done) {
		throw PortIsBusy(SERVER_NET_SPECS::PORTS::RECEIVE);
	}



	std::unique_ptr<Room> room = nullptr;
	RemotePlayers players;



	while (room == nullptr or room->playersNumber() != players.size()) { // Block list is not used cuz server run locally
		sf::Packet receivedPacket;
		sf::IpAddress senderIP;
		uint16_t senderPort;
		while (receiveSocket.receive(receivedPacket, senderIP, senderPort) != sf::Socket::Status::Done) {
			if (*stop) {
				return;
			}
			sf::sleep(sf::milliseconds(50));
		}
		if (senderIP != sf::IpAddress::getLocalAddress() or senderPort != CLIENT_NET_SPECS::PORTS::SEND) {
			continue;
		}
        sf::Uint64 packageId;
        receivedPacket >> packageId;
        sf::Uint64 roomIdVal;
        receivedPacket >> roomIdVal;
        uint8_t code;
        receivedPacket >> code;
        if (code == CLIENT_NET_SPECS::CODES::CREATE) {
            if (room == nullptr) {
                std::string data;
                receivedPacket >> data;
                room = std::make_unique<Room>(RoomID(roomIdVal), data, Room::Restrictions::Disable);
                std::cout << "Room was created" << std::endl;

                uint32_t playersAtHost;
                receivedPacket >> playersAtHost;
                while (playersAtHost and players.size() != room->playersNumber()) {
                    players.add(RemotePlayer(players.size() + 1, sf::IpAddress::getLocalAddress()));
                    playersAtHost = playersAtHost - 1;
                    std::cout << "Added player to room. Total players: " << players.size() << std::endl;
                }
            }
        }
        else if (code == CLIENT_NET_SPECS::CODES::CONNECT) {
            if (room != nullptr) {
                if (room->getID().value() == roomIdVal) {
                    uint32_t playersAtHost;
                    receivedPacket >> playersAtHost;
                    while (playersAtHost and players.size() != room->playersNumber()) {
                        players.add(RemotePlayer(players.size() + 1, sf::IpAddress::getLocalAddress()));
                        playersAtHost = playersAtHost - 1;
                        std::cout << "Added player to room. Total players: " << players.size() << std::endl;
                    }
                }
                else {
                    std::cout << "Player used incorrect room id." << std::endl;
                }
            }
        }
	}



	std::cout << "Room launched!" << std::endl;

	boost::optional<std::tuple<sf::Packet, sf::IpAddress>> received;
	std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSend;
    uint32_t sum = 0;

	for (; ;) {
		Clock clock;

		while (!toSend.empty()) {
			sendSocket.send(std::get<0>(toSend.back()), std::get<1>(toSend.back()), CLIENT_NET_SPECS::PORTS::RECEIVE);
            sum = sum + std::get<0>(toSend.back()).getDataSize();
            std::cout << "Packet was sent. Size: " << std::get<0>(toSend.back()).getDataSize() << " bytes. Total: " << (float)sum / 1024 / 1024 << " mb" << std::endl;
			toSend.pop_back();
		}

		received = boost::none;
		sf::Packet receivedPacket;
		sf::IpAddress senderIP;
		uint16_t senderPort;
		if (receiveSocket.receive(receivedPacket, senderIP, senderPort) == sf::Socket::Status::Done and senderIP == sf::IpAddress::getLocalAddress() and senderPort == CLIENT_NET_SPECS::PORTS::SEND) {
			received = std::make_tuple(receivedPacket, senderIP);
            sum = sum + receivedPacket.getDataSize();
            std::cout << "Packet was received. Size: " << receivedPacket.getDataSize() << " bytes. Total: " << (float)sum / 1024 / 1024 << " mb" << std::endl;
		}

		try {
			room->update(received, &toSend, players);
		}
		catch (RoomWasClosed&) {
			break;
		}

		if (*stop) {
			return;
		}

		sf::sleep(sf::milliseconds(bfdlib::math::subu<uint32_t>(1000 / 60, clock.getMS())));
	}
}






static void THREAD_EXCEPTION_SAFE(std::exception_ptr *unexpectedError, std::atomic_bool* stop, std::atomic_bool* running) {
	std::cout << "Started new local server" << std::endl;
	*running = true;

	try {
		THREAD(unexpectedError, stop);
	}
	catch (std::exception& e) {
		*unexpectedError = std::current_exception();
		std::cout << "Local server got an unexpected error. Thread was stopped. Exception ptr was saved" << std::endl; // It is ok cuz server runned locally. No safety problems.
		std::cout << e.what() << std::endl;
	}

	*running = false;
}






LocalServer::LocalServer() {
	this->stop = false;
	this->running = false;
	this->thread = nullptr;
}
LocalServer::~LocalServer() {
	this->finish();
	std::cout << "Destroyed local server" << std::endl;
}
void LocalServer::finish() {
	if (this->running) {
		this->stop = true;
		this->thread->wait();
		this->stop = false;
	}
}
void LocalServer::launch() {
	if (this->running) {
		throw LocalServerAlreadyLaunched();
	}
	this->thread = std::make_unique<sf::Thread>(std::bind(&THREAD_EXCEPTION_SAFE, &this->unexpectedError, &this->stop, &this->running));
	this->thread->launch();
}
void LocalServer::fine() const {
	if (this->unexpectedError != nullptr) {
		std::rethrow_exception(this->unexpectedError);
	}
}