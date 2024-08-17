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
#include "PortIsBusy.hpp"
#include "LocalServerAlreadyLaunched.hpp"
#include "ServerNetSpecs.hpp"
#include "tcp_help.hpp"








static void THREAD(std::exception_ptr* unexpectedError, const std::atomic<bool>* stop, std::atomic<bool>* ready) {
	sf::TcpListener listener;
    if (listener.listen(SERVER_NET_SPECS::PORTS::TCP) != sf::Socket::Status::Done) {
        throw PortIsBusy(SERVER_NET_SPECS::PORTS::TCP);
    }




    sf::TcpSocket socket;
    socket.setBlocking(false);
    for (; ; sf::sleep(sf::milliseconds(5))) {
        if (*stop) {
            return;
        }
        if (listener.accept(socket) == sf::Socket::Status::Done) {
            break;
        }
        *ready = true;
    }
    socket.setBlocking(true);
    std::cout << "Got connection" << std::endl;




    bfdlib::tcp_help::packet_queue toSend;
    bfdlib::tcp_help::packet_queue received;
    std::unique_ptr<sf::Thread> sendThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_help::process_sending, &socket, &toSend, stop));
    std::unique_ptr<sf::Thread> receiveThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_help::process_receiving, &socket, &toSend, stop));
    sendThread->launch();
    receiveThread->launch();




	std::unique_ptr<Room> room = nullptr;
	RemotePlayers players;
	while (room == nullptr or room->playersNumber() != players.size()) { // Block list is not used cuz server run locally
		for (; ; sf::sleep(sf::milliseconds(5))) {
            if (*stop) {
                return;
            }
            if (!received.empty()) {
                break;
            }
        }
        sf::Packet receivedPacket = received.pop();
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
	}
	std::cout << "Room launched!" << std::endl;




	for (; ;) {
        if (*stop) {
            return;
        }

        Clock clock;

		boost::optional<std::tuple<sf::Packet, sf::IpAddress>> tuple = boost::none;
		if (!received.empty()) {
            tuple = std::make_tuple(received.pop(), sf::IpAddress::getLocalAddress());
        }

        std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSendGlobal;

        room->update(tuple, &toSendGlobal, players);

        for (const auto &val : toSendGlobal) {
            toSend.push(std::get<sf::Packet>(val));
        }

        sf::sleep(sf::milliseconds(bfdlib::math::subu<uint32_t>(1000 / 60, clock.getMS())));
	}
}






static void THREAD_EXCEPTION_SAFE(std::exception_ptr *unexpectedError, std::atomic_bool* stop, std::atomic_bool* running, std::atomic_bool *ready) {
	*running = true;
    std::cout << "Local server was started" << std::endl;

	try {
		THREAD(unexpectedError, stop, ready);
	}
	catch (std::exception& e) {
		*unexpectedError = std::current_exception();
		std::cout << "Local server got an unexpected error: " << e.what() << std::endl;
	}

    std::cout << "Local server was closed" << std::endl;
    *ready = true;
	*running = false;
}






LocalServer::LocalServer() {
	this->stop = false;
	this->running = false;
	this->thread = nullptr;
}
LocalServer::~LocalServer() {
	this->finish();
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
    std::atomic<bool> ready = false;
	this->thread = std::make_unique<sf::Thread>(std::bind(&THREAD_EXCEPTION_SAFE, &this->unexpectedError, &this->stop, &this->running, &ready));
	this->thread->launch();
    while (!ready) {
        sf::sleep(sf::milliseconds(5));
    }
}
void LocalServer::fine() const {
	if (this->unexpectedError != nullptr) {
		std::rethrow_exception(this->unexpectedError);
	}
}