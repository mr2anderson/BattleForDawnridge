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


#include "ServerScreen.hpp"
#include "Textures.hpp"
#include "Playlist.hpp"
#include "SoundQueue.hpp"
#include "Label.hpp"
#include "ScreenAlreadyFinished.hpp"
#include "PublicIP.hpp"
#include "ServerNetSpecs.hpp"
#include "ClientNetSpecs.hpp"
#include "MainServerPosition.hpp"


/*ServerScreen::ServerScreen(sf::RenderWindow& window) {
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

    if (this->listener.listen(MainServerPosition::get()->getPort()) != sf::Socket::Status::Done) {
        this->logs.add(StringLcl("{couldnt_listen_port}"));
    }
    this->listener.setBlocking(false);
    this->stop = false;
    this->traffic = 0;
}
ServerScreen::~ServerScreen() {
    this->stop = true;
    for (uint32_t i = 0; i < this->sendThreads.size(); i = i + 1) {
        this->sendThreads.at(i)->wait();
        this->receiveThreads.at(i)->wait();
        delete this->received.at(i);
        delete this->toSend.at(i);
        delete this->sockets.at(i);
    }
}
ServerScreenResponse ServerScreen::run(sf::RenderWindow& window) {
    if (this->alreadyFinished) {
        throw ScreenAlreadyFinished();
    }
    this->alreadyFinished = true;

    window.setMouseCursorVisible(false);

    sf::Event event;
    sf::TcpSocket *tempSocket = new sf::TcpSocket();
    for (; ;) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    delete tempSocket;
                    return ServerScreenResponse(ServerScreenResponse::TYPE::EXIT);
                }
                if (event.key.code == sf::Keyboard::A) {
                    this->logs.add(StringLcl("{traffic}" + std::to_string((float)this->traffic / 1024 / 1024)) + " MB");
                }
            }
        }

        Playlist::get()->update();

        this->drawEverything(window);

        if (this->listener.accept(*tempSocket) == sf::Socket::Status::Done) {
            this->sockets.push_back(tempSocket);
            tempSocket = new sf::TcpSocket();
            this->logs.add(StringLcl("{new_connection}" + this->sockets.back()->getRemoteAddress().toString()));
            this->toSend.push_back(new bfdlib::tcp_helper::queue_w());
            this->received.push_back(new bfdlib::tcp_helper::queue_r());
            this->sendThreads.push_back(std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_helper::process_sending, this->sockets.back(), toSend.back(), &this->stop, &this->traffic)));
            this->receiveThreads.push_back(std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_helper::process_receiving, this->sockets.back(), received.back(), &this->stop, &this->traffic)));
            this->sendThreads.back()->launch();
            this->receiveThreads.back()->launch();
        }

        for (; ;) {
            boost::optional<std::tuple<sf::Packet, sf::IpAddress>> receivedPacket = boost::none;
            for (uint32_t i = 0; i < this->received.size(); i = i + 1) {
                std::optional<sf::Packet> opt = this->received.at(i)->pop();
                if (opt.has_value()) {
                    receivedPacket = std::make_tuple(opt.value(), this->sockets.at(i)->getRemoteAddress());
                    break;
                }
            }

            if (receivedPacket.has_value()) {
                std::tuple<sf::Packet, sf::IpAddress> receivedPacketCopy = receivedPacket.value();
                this->checkRoomInitSignal(std::get<0>(receivedPacketCopy), std::get<1>(receivedPacketCopy));
            }

            std::vector<std::tuple<sf::Packet, sf::IpAddress>> toRoomClients; // TODO: optimize
            this->rooms.updateAll(receivedPacket, &toRoomClients);
            for (uint32_t i = 0; i < toRoomClients.size(); i = i + 1) {
                for (uint32_t j = 0; j < this->sockets.size(); j = j + 1) {
                    if (this->sockets.at(j)->getRemoteAddress() == std::get<sf::IpAddress>(toRoomClients.at(i))) {
                        this->toSend.at(j)->push(std::get<sf::Packet>(toRoomClients.at(i)));
                    }
                }
            }

            if (!receivedPacket.has_value()) {
                break;
            }
        }
    }
}
void ServerScreen::checkRoomInitSignal(sf::Packet& packet, sf::IpAddress ip) {
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
                this->logs.add(StringLcl("{new_room}" + roomId.readableValue() + " " + ip.toString() + " " + std::to_string(added)));
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
}*/