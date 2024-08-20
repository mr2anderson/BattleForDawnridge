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
#include "LocalServerAlreadyLaunched.hpp"
#include "ServerNetSpecs.hpp"
#include "tcp_helper.hpp"







#define PRINT_LOGS






#if defined(PRINT_LOGS)
static void LOGS(const std::string &val) {
    std::cout << "Local server: " << val << std::endl;
}
static void LOGS(const std::wstring& val) {

}
#else
static void LOGS(const std::string &val) {}
static void LOGS(const std::wstring &val) {}
#endif











static void CLOSE_THREADS(std::unique_ptr<sf::Thread> &sendThread, std::unique_ptr<sf::Thread> &receiveThread, std::atomic<bool> &stop) {
    stop = true;
    sendThread->wait();
    receiveThread->wait();
}
static void THREAD(std::atomic<bool>& stop, std::atomic<bool>& ready, std::atomic<uint16_t> &port, std::atomic<uint64_t> &sendTraffic, std::atomic<uint64_t> &receiveTraffic) {
	sf::TcpListener listener;
    LOGS("Looking for port...");
    for (uint32_t p = 1024; p < 49151; p = p + 1) {
        if (listener.listen(p) == sf::Socket::Status::Done) {
            port = p;
            LOGS("Port " + std::to_string(p) + " is available!");
            break;
        }
        else {
            LOGS("Unable to listen port " + std::to_string(p));
        }
    }




    sf::TcpSocket socket;
    listener.setBlocking(false);
    LOGS("Waiting for client...");
    for (; ; sf::sleep(sf::milliseconds(5))) {
        if (stop) {
            return;
        }
        if (listener.accept(socket) == sf::Socket::Status::Done) {
            break;
        }
        ready = true;
    }
    LOGS("Client accepted.");




    bfdlib::tcp_helper::queue_w toSend;
    bfdlib::tcp_helper::queue_r received;
    socket.setBlocking(false);
    std::atomic<bool> stopTcpThread = false;
    std::unique_ptr<sf::Thread> sendThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_helper::process_sending, std::ref(socket), std::ref(toSend), std::ref(stopTcpThread), std::ref(sendTraffic)));
    std::unique_ptr<sf::Thread> receiveThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_helper::process_receiving, std::ref(socket), std::ref(received), std::ref(stopTcpThread), std::ref(receiveTraffic)));
    sendThread->launch();
    receiveThread->launch();




	std::unique_ptr<Room> room = nullptr;
	RemotePlayers players;
    LOGS("Creating room...");
	while (room == nullptr or room->playersNumber() != players.size()) {
        std::optional<sf::Packet> receivedPacketOpt;
		for (; ; sf::sleep(sf::milliseconds(5))) {
            if (stop) {
                CLOSE_THREADS(std::ref(sendThread), std::ref(receiveThread), std::ref(stopTcpThread));
                return;
            }
            receivedPacketOpt = received.pop();
            if (receivedPacketOpt != std::nullopt) {
                break;
            }
        }
        sf::Packet receivedPacket = receivedPacketOpt.value();
        LOGS("Got pkg!");
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
    LOGS("Created.");


    LOGS("Processing...");
	for (; ;) {
        if (stop) {
            CLOSE_THREADS(std::ref(sendThread), std::ref(receiveThread), std::ref(stopTcpThread));
            return;
        }

        Clock clock;

		boost::optional<std::tuple<sf::Packet, sf::IpAddress>> tuple = boost::none;
        std::optional<sf::Packet> packet = received.pop();
		if (packet != std::nullopt) {
            tuple = std::make_tuple(packet.value(), sf::IpAddress::getLocalAddress());
        }

        std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSendGlobal;
        std::vector<StringLcl> logs;
        RoomOutputProtocol protocol;
        protocol.logs = &logs;
        protocol.toSend = &toSendGlobal;
        protocol.remotePlayers = &players;
        room->update(tuple, protocol);
        for (const auto &val : toSendGlobal) {
            toSend.push(std::get<sf::Packet>(val));
        }
        for (const auto& var : logs) {
            LOGS(var.get());
        }

        sf::sleep(sf::milliseconds(bfdlib::math::subu<uint32_t>(1000 / 60, clock.getMS())));
	}
}






static void THREAD_EXCEPTION_SAFE(std::exception_ptr &unexpectedError, std::atomic_bool& stop, std::atomic_bool& running, std::atomic_bool &ready, std::atomic<uint16_t> &port, std::atomic<uint64_t> &sendTraffic, std::atomic<uint64_t> &receiveTraffic) {
	running = true;
    LOGS("Local server was started in new thread");

	try {
		THREAD(std::ref(stop), std::ref(ready), std::ref(port), std::ref(sendTraffic), std::ref(receiveTraffic));
	}
	catch (std::exception& e) {
		unexpectedError = std::current_exception();
        LOGS("Local server thread got an unexpected error: " + std::string(e.what()));
	}

    LOGS("Local server thread was closed");
    ready = true;
	running = false;
}






LocalServer::LocalServer() {
	this->stop.store(false);
	this->running.store(false);
	this->thread = nullptr;
}
LocalServer::~LocalServer() {
    LOGS("Destroying local server...");

	this->finish();

    LOGS("Local server was destroyed. Sent: " + std::to_string((float)this->sendTraffic / 1024 / 1024) +
         " MB. Received: " + std::to_string((float)this->receiveTraffic / 1024 / 1024) + " MB");
}
void LocalServer::finish() {
    LOGS("Finishing local server...");
	if (this->running) {
		this->stop = true;
		this->thread->wait();
		this->stop = false;
        LOGS("Finished.");
	}
    else {
        LOGS("Already finished.");
    }
}
uint16_t LocalServer::launch() {
	if (this->running) {
		throw LocalServerAlreadyLaunched();
	}
    this->sendTraffic = 0;
    this->receiveTraffic = 0;
    std::atomic<bool> ready;
    ready.store(false);
    std::atomic<uint16_t> port;
    port.store(0);
    LOGS("Launching local sever thread...");
	this->thread = std::make_unique<sf::Thread>(std::bind(&THREAD_EXCEPTION_SAFE, std::ref(this->unexpectedError), std::ref(this->stop), std::ref(this->running), std::ref(ready), std::ref(port), std::ref(this->sendTraffic), std::ref(this->receiveTraffic)));
	this->thread->launch();
    while (!ready) {
        sf::sleep(sf::milliseconds(5));
    }
    LOGS("Local server thread reported that it is ready to detach");
    return port;
}
void LocalServer::fine() const {
	if (this->unexpectedError != nullptr) {
		std::rethrow_exception(this->unexpectedError);
	}
}