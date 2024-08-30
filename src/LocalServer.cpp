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







#define PRINT_LOGS






#if defined(PRINT_LOGS)

static void LOGS(const std::string &val) {
    std::cout << "Local server: " << val << std::endl;
}
static void LOGS(const StringLcl& val) {
    std::cout << "Local server: " << val.toRawString() << std::endl; // val.get() requires utf 16 support in console
}

#else

static void LOGS(const std::string &val) {}
static void LOGS(const std::wstring &val) {}

#endif











static void PROCESS_SENDING(sf::TcpSocket &socket, std::queue<sf::Packet>& toSend) {
    if (toSend.empty()) {
        return;
    }
    if (socket.send(toSend.front()) == sf::Socket::Status::Done) {
        toSend.pop();
    }
}
static void PROCESS_RECEIVING(sf::TcpSocket& socket, std::tuple<bool, sf::Packet>& received) {
    if (std::get<bool>(received)) {
        return;
    }
    if (socket.receive(std::get<sf::Packet>(received)) == sf::Socket::Status::Done) {
        std::get<bool>(received) = true;
    }
}
static void THREAD(std::atomic<bool>& stop, std::atomic<bool>& ready, std::atomic<uint16_t> &port) {
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




    UUID id;
    std::queue<sf::Packet> toSend;
    std::tuple<bool, sf::Packet> received = std::make_tuple(false, sf::Packet());
    socket.setBlocking(false);
    std::atomic<bool> stopTcpThread;
    stopTcpThread.store(false);
    std::atomic<bool> error;
    error.store(false);




	std::unique_ptr<Room> room = nullptr;
	RemotePlayers players;
    LOGS("Creating room...");
	while (room == nullptr or room->playersNumber() != players.size()) {
		for (; ; sf::sleep(sf::milliseconds(5))) {
            if (stop) {
                return;
            }
            PROCESS_RECEIVING(socket, received);
            if (std::get<bool>(received)) {
                break;
            }
        }
        std::get<bool>(received) = false;
        sf::Packet receivedPacket = std::get<sf::Packet>(received);
        LOGS("Got pkg!");
        std::string roomIdVal;
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
                players.set(RemotePlayer(players.size() + 1, id));
                playersAtHost = playersAtHost - 1;
            }
        }
	}
    LOGS("Created.");


    LOGS("Processing...");
	for (; ;) {
        if (stop) {
            return;
        }

        Clock clock;

        PROCESS_SENDING(socket, toSend);
        PROCESS_RECEIVING(socket, received);

		boost::optional<std::tuple<sf::Packet, UUID>> tuple = boost::none;
		if (std::get<bool>(received)) {
            tuple = std::make_tuple(std::get<sf::Packet>(received), id);
            std::get<bool>(received) = false;
        }

        std::vector<std::tuple<sf::Packet, UUID>> toSendGlobal;
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
            LOGS(var);
        }

        sf::sleep(sf::milliseconds(bfdlib::math::subu<uint32_t>(1000 / 60, clock.getMS())));
	}
}






static void THREAD_EXCEPTION_SAFE(std::atomic_bool& stop, std::atomic_bool& running, std::atomic_bool &ready, std::atomic<uint16_t> &port) {
	running = true;
    LOGS("Local server was started in new thread");

	try {
		THREAD(std::ref(stop), std::ref(ready), std::ref(port));
	}
	catch (std::exception& e) {
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

    LOGS("Local server was destroyed.");
}
void LocalServer::finish() {
    this->stop = true;
    if (this->thread != nullptr) {
        try {
            this->thread->join();
        }
        catch (std::exception&) {

        }
    }
}
uint16_t LocalServer::launch() {
	if (this->running) {
		throw LocalServerAlreadyLaunched();
	}
    std::atomic<bool> ready;
    ready.store(false);
    std::atomic<uint16_t> port;
    port.store(0);
    LOGS("Launching local sever thread...");
    this->stop = false;
	this->thread = std::make_unique<std::thread>(std::bind(&THREAD_EXCEPTION_SAFE, std::ref(this->stop), std::ref(this->running), std::ref(ready), std::ref(port)));
    this->thread->detach();
    while (!ready) {
        sf::sleep(sf::milliseconds(5));
    }
    LOGS("Local server thread reported that it is ready to detach");
    return port;
}