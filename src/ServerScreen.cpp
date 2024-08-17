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
#include "ServerScreen.hpp"
#include "Textures.hpp"
#include "Playlist.hpp"
#include "SoundQueue.hpp"
#include "Label.hpp"
#include "ScreenAlreadyFinished.hpp"
#include "PublicIP.hpp"
#include "ServerNetSpecs.hpp"
#include "ClientNetSpecs.hpp"


ServerScreen::ServerScreen(sf::RenderWindow& window) {
	this->alreadyFinished = false;

	this->logs.setEntryLimit(10);
	this->logs.add(StringLcl("{server_mode_welcome}"));
	if (PublicIP::get()->getIp().toString() == "0.0.0.0") {
		this->logs.add(StringLcl("{couldnt_get_public_ip}"));
	}
	else {
		this->logs.add(StringLcl("{public_ip}") + StringLcl(PublicIP::get()->getIp().toString()));
	}
	this->logs.add(StringLcl("{entry_limit_set}" + std::to_string(this->logs.getEntryLimit())));

    if (this->sendSocket.bind(SERVER_NET_SPECS::PORTS::SEND) != sf::Socket::Done) {
        this->logs.add(StringLcl("{couldnt_bind_send_socket}"));
    }
	this->sendSocket.setBlocking(false);
    if (this->receiveSocket.bind(SERVER_NET_SPECS::PORTS::RECEIVE) != sf::Socket::Done) {
        this->logs.add(StringLcl("{couldnt_bind_receive_port}"));
    }
	this->receiveSocket.setBlocking(false);
}
ServerScreenResponse ServerScreen::run(sf::RenderWindow& window) {
	if (this->alreadyFinished) {
		throw ScreenAlreadyFinished();
	}
	this->alreadyFinished = true;

	window.setMouseCursorVisible(false);

    boost::optional<std::tuple<sf::Packet, sf::IpAddress>> received;
    std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSend;

	sf::Event event;
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					return ServerScreenResponse(ServerScreenResponse::TYPE::EXIT);
				}
			}
		}

		Playlist::get()->update();

		this->drawEverything(window);

        while (!toSend.empty()) {
            sendSocket.send(std::get<0>(toSend.back()), std::get<1>(toSend.back()), CLIENT_NET_SPECS::PORTS::RECEIVE);
            toSend.pop_back();
        }

        received = boost::none;
        sf::Packet receivedPacket;
        sf::IpAddress senderIP;
        uint16_t senderPort;
        if (receiveSocket.receive(receivedPacket, senderIP, senderPort) == sf::Socket::Status::Done and senderPort == CLIENT_NET_SPECS::PORTS::SEND) {
            received = std::make_tuple(receivedPacket, senderIP);
            this->checkRoomInitSignal(receivedPacket, senderIP);
        }

        this->rooms.updateAll(received, &toSend);
	}
}
void ServerScreen::checkRoomInitSignal(sf::Packet& packet, sf::IpAddress ip) {
    sf::Uint64 packageId;
    packet >> packageId;

    sf::Uint64 roomIdVal;
    packet >> roomIdVal;
    RoomID roomId(roomIdVal);

    uint8_t code;
    packet >> code;

    if (code == CLIENT_NET_SPECS::CODES::CREATE) {
        if (this->rooms.exist(roomId)) {
            this->logs.add(StringLcl("{attempt_to_create_room_with_existing_id}" + roomId.readableValue() + " " + ip.toString()));
        }
        if (!this->rooms.exist(roomId)) {
            std::string data;
            packet >> data;
            this->rooms.createIfValid(roomId, data);
            if (this->rooms.exist(roomId)) {
                uint32_t playersAtHost;
                packet >> playersAtHost;
                uint32_t added = this->rooms.addPlayersSafe(roomId, ip, playersAtHost);
                this->logs.add(StringLcl("{room_was_created}" + roomId.readableValue() + " " + ip.toString() + " " + std::to_string(added)));
            }
            else {
                this->logs.add(StringLcl("{couldnt_create_room}"));
            }
        }
    }
    else if (code == CLIENT_NET_SPECS::CODES::CONNECT) {
        if (this->rooms.exist(roomId)) {
            uint32_t playersAtHost;
            packet >> playersAtHost;
            uint32_t added = this->rooms.addPlayersSafe(roomId, ip, playersAtHost);
            this->logs.add(StringLcl("{attempt_to_connect_to_room}" + roomId.readableValue() + " " + ip.toString() + " " + std::to_string(added)));
        }
        else {
            this->logs.add(StringLcl("{attempt_to_connect_to_room_with_unknown_id}" + roomId.readableValue() + " " + ip.toString()));
        }
    }
}
void ServerScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.draw(this->bg);
	window.draw(Label(10, 10, window.getSize().x - 20, window.getSize().y - 20, this->logs.get()));
	window.display();
}