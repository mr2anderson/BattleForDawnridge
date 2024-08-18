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







#define PRINT_LOGS






static void LOGS(const std::string &val) {
    #ifdef PRINT_LOGS
    std::cout << val << std::endl;
    #endif
}











static void CLOSE_THREADS(std::unique_ptr<sf::Thread> &sendThread, std::unique_ptr<sf::Thread> &receiveThread, std::atomic<bool> *stop) {
    *stop = true;
    sendThread->wait();
    receiveThread->wait();
}
static void THREAD(const std::atomic<bool>* stop, std::atomic<bool>* ready, std::atomic<uint64_t> *sendTraffic, std::atomic<uint64_t> *receiveTraffic) {
	sf::TcpListener listener;
    LOGS("Listening port...");
    if (listener.listen(SERVER_NET_SPECS::PORTS::TCP) != sf::Socket::Status::Done) {
        throw PortIsBusy(SERVER_NET_SPECS::PORTS::TCP);
    }
    LOGS("OK");




    sf::TcpSocket socket;
    listener.setBlocking(false);
    LOGS("Waiting for client...");
    for (; ; sf::sleep(sf::milliseconds(5))) {
        if (*stop) {
            return;
        }
        if (listener.accept(socket) == sf::Socket::Status::Done) {
            break;
        }
        *ready = true;
    }
    LOGS("OK");




    bfdlib::tcp_help::queue_w toSend;
    bfdlib::tcp_help::queue_r received;
    socket.setBlocking(false);
    std::atomic<bool> stopTcpThread = false;
    std::unique_ptr<sf::Thread> sendThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_help::process_sending, &socket, &toSend, &stopTcpThread, sendTraffic));
    std::unique_ptr<sf::Thread> receiveThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_help::process_receiving, &socket, &received, &stopTcpThread, receiveTraffic));
    sendThread->launch();
    receiveThread->launch();




	std::unique_ptr<Room> room = nullptr;
	RemotePlayers players;
    LOGS("Creating room...");
	while (room == nullptr or room->playersNumber() != players.size()) {
		for (; ; sf::sleep(sf::milliseconds(5))) {
            if (*stop) {
                CLOSE_THREADS(sendThread, receiveThread, &stopTcpThread);
                return;
            }
            if (!received.empty()) {
                break;
            }
        }
        LOGS("Got pkg!");
        sf::Packet receivedPacket = received.pop();
        sf::Uint64 roomIdVal;
        receivedPacket >> roomIdVal;
        uint8_t code;
        receivedPacket >> code;
        if (code == CLIENT_NET_SPECS::CODES::CREATE) {
            std::string data;
            receivedPacket >> data;
            room = std::make_unique<Room>(RoomID(roomIdVal), data, Room::Restrictions::Disable);
            uint32_t playersAtHost;
            receivedPacket >> playersAtHost;
            while (playersAtHost and players.size() != room->playersNumber()) {
                players.add(RemotePlayer(players.size() + 1, sf::IpAddress::getLocalAddress()));
                playersAtHost = playersAtHost - 1;
            }
        }
	}
    LOGS("OK");


    LOGS("Processing...");
	for (; ;) {
        if (*stop) {
            CLOSE_THREADS(sendThread, receiveThread, &stopTcpThread);
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






static void THREAD_EXCEPTION_SAFE(std::exception_ptr *unexpectedError, std::atomic_bool* stop, std::atomic_bool* running, std::atomic_bool *ready, std::atomic<uint64_t> *sendTraffic, std::atomic<uint64_t> *receiveTraffic) {
	*running = true;
    LOGS("Local server was started");

	try {
		THREAD(stop, ready, sendTraffic, receiveTraffic);
	}
	catch (std::exception& e) {
		*unexpectedError = std::current_exception();
        LOGS("Local server got an unexpected error: ");
	}

    LOGS("Local server was closed");
    *ready = true;
	*running = false;
}






LocalServer::LocalServer() {
	this->stop = false;
	this->running = false;
	this->thread = nullptr;
}
LocalServer::~LocalServer() {
    LOGS("Destroying local server...");

	this->finish();

    LOGS("Local server was destroyed. Send: " + std::to_string((float)this->sendTraffic / 1024 / 1024) +
         " MB. Received: " + std::to_string((float)this->receiveTraffic / 1024 / 1024) + " MB");
}
void LocalServer::finish() {
    LOGS("Finishing local server...");
	if (this->running) {
		this->stop = true;
		this->thread->wait();
		this->stop = false;
        LOGS("OK");
	}
    else {
        LOGS("Already finished.");
    }
}
void LocalServer::launch() {
	if (this->running) {
		throw LocalServerAlreadyLaunched();
	}
    std::atomic<bool> ready = false;
    LOGS("Launching local sever thread...");
	this->thread = std::make_unique<sf::Thread>(std::bind(&THREAD_EXCEPTION_SAFE, &this->unexpectedError, &this->stop, &this->running, &ready, &this->sendTraffic, &this->receiveTraffic));
	this->thread->launch();
    while (!ready) {
        sf::sleep(sf::milliseconds(5));
    }
    LOGS("Local server thread reported that it is ready to detach");
}
void LocalServer::fine() const {
	if (this->unexpectedError != nullptr) {
		std::rethrow_exception(this->unexpectedError);
	}
}