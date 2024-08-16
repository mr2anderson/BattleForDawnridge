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
#include "Room.hpp"
#include "RoomWasClosed.hpp"


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
    if (this->receiveSocket.bind(SERVER_NET_SPECS::PORTS::RECEIVE) != sf::Socket::Done) {
        this->logs.add(StringLcl("{couldnt_bind_receive_port}"));
    }
}
ServerScreenResponse ServerScreen::run(sf::RenderWindow& window) {
	if (this->alreadyFinished) {
		throw ScreenAlreadyFinished();
	}
	this->alreadyFinished = true;

	window.setMouseCursorVisible(false);

    boost::optional<std::tuple<sf::Packet, sf::IpAddress>> received;
    std::vector<std::tuple<sf::Packet, sf::IpAddress>> toSend;

    std::vector<boost::optional<std::tuple<Room, RemotePlayers>>> rooms;

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

        received = boost::none;
        sf::Packet receivedPacket;
        sf::IpAddress senderIP;
        uint16_t senderPort;
        if (receiveSocket.receive(receivedPacket, senderIP, senderPort) == sf::Socket::Status::Done and senderIP == sf::IpAddress::getLocalAddress() and senderPort == CLIENT_NET_SPECS::PORTS::SEND) {
            received = std::make_tuple(receivedPacket, senderIP);
        }

        for (uint32_t i = 0; i < rooms.size(); i = i + 1) {
            if (rooms.at(i).has_value()) {
                try {
                    std::get<0>(rooms.at(i).value()).update(received, &toSend, std::get<1>(rooms.at(i).value()));
                }
                catch (RoomWasClosed &) {

                }
                catch (std::exception&) {

                }

                while (!toSend.empty()) {
                    sendSocket.send(std::get<0>(toSend.back()), std::get<1>(toSend.back()), CLIENT_NET_SPECS::PORTS::RECEIVE);
                    toSend.pop_back();
                }
            }
        }
	}
}
void ServerScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.draw(this->bg);
	window.draw(Label(10, 10, window.getSize().x - 20, window.getSize().y - 20, this->logs.get()));
	window.display();
}