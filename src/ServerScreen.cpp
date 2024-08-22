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
#include "ServerNetSpecs.hpp"
#include "ClientNetSpecs.hpp"
#include "MenuBg.hpp"
#include "MainServerPosition.hpp"
#include "InvalidRoomIDFormat.hpp"


ServerScreen::ServerScreen(sf::RenderWindow& window) {
    this->alreadyFinished = false;

    this->logs.setEntryLimit(50);
    this->logs.add(StringLcl("{server_mode_welcome}"));
    this->logs.add(StringLcl("{entry_limit_set}" + std::to_string(this->logs.getEntryLimit())));
    this->logs.add(StringLcl());

    if (this->listener.listen(MainServerPosition::get()->getPort()) != sf::Socket::Status::Done) {
        this->logs.add(StringLcl("{couldnt_listen_port}"));
    }
    this->listener.setBlocking(false);
}
void ServerScreen::run(sf::RenderWindow& window) {
    if (this->alreadyFinished) {
        throw ScreenAlreadyFinished();
    }
    this->alreadyFinished = true;

    window.setMouseCursorVisible(false);

    sf::Event event;
    bool r = true;
    while (r) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    r = false;
                }
                if (event.key.code == sf::Keyboard::A) {
                    this->addTrafficInfo();
                }
            }
        }

        Playlist::get()->update();

        this->drawEverything(window);

        this->connections.emplace_front();
        if (this->listener.accept(this->connections.front().getSocketRef()) == sf::Socket::Status::Done) {
            this->logs.add(StringLcl("{new_connection}" + this->connections.front().getSocketRef().getRemoteAddress().toString()));
            auto iterator = this->connections.begin();
            auto prevIterator = this->connections.begin();
            iterator++;
            while (iterator != this->connections.end()) {
                if (iterator->getSocketRef().getRemoteAddress() == this->connections.front().getSocketRef().getRemoteAddress()) {
                    this->logs.add(StringLcl("{removed_old_connection}"));
                    this->connections.erase_after(prevIterator);
                    break;
                }
                prevIterator++;
                iterator++;
            }
        }
        else {
            this->connections.pop_front();
        }

        for (; ;) {
            for (auto& connection : this->connections) {
                connection.update();
            }

            boost::optional<std::tuple<sf::Packet, sf::IpAddress>> receivedPacket = boost::none;
            auto iterator = this->connections.begin();
            while (iterator != this->connections.end()) {
                std::optional<sf::Packet> opt = iterator->getReceivedPacket();
                if (opt.has_value()) {
                    receivedPacket = std::make_tuple(opt.value(), iterator->getSocketRef().getRemoteAddress());
                    break;
                }
                iterator++;
            }

            if (receivedPacket.has_value()) {
                std::tuple<sf::Packet, sf::IpAddress> receivedPacketCopy = receivedPacket.value();
                this->checkRoomInitSignal(std::get<0>(receivedPacketCopy), std::get<1>(receivedPacketCopy));
            }

            // TODO: optimize
            std::vector<std::tuple<sf::Packet, sf::IpAddress>> toRoomClients;
            std::vector<StringLcl> toLogs;
            this->rooms.updateAll(receivedPacket, &toLogs, &toRoomClients);
            for (uint32_t i = 0; i < toLogs.size(); i = i + 1) {
                this->logs.add(toLogs.at(i));
            }
            for (uint32_t i = 0; i < toRoomClients.size(); i = i + 1) {
                iterator = this->connections.begin();
                while (iterator != this->connections.end()) {
                    if (iterator->getSocketRef().getRemoteAddress() == std::get<sf::IpAddress>(toRoomClients.at(i))) {
                        iterator->send(std::get<sf::Packet>(toRoomClients.at(i)));
                    }
                    iterator++;
                }
            }

            if (!receivedPacket.has_value()) {
                break;
            }
        }
    }
}
void ServerScreen::checkRoomInitSignal(sf::Packet& packet, sf::IpAddress ip) {
    std::string roomIdVal;
    packet >> roomIdVal;

    RoomID roomId;
    try {
        roomId = RoomID(roomIdVal);
    }
    catch (InvalidRoomIDFormat&) {
        this->logs.add(StringLcl("{invalid_room_id}" + ip.toString()));
    }

    uint8_t code;
    packet >> code;

    if (code == CLIENT_NET_SPECS::CODES::CREATE) {
        if (this->rooms.exist(roomId)) {
            this->logs.add(StringLcl("{attempt_to_create_room_with_existing_id}" + roomId.value() + " " + ip.toString()));
        }
        if (!this->rooms.exist(roomId)) {
            std::string data;
            packet >> data;
            this->rooms.createIfValid(roomId, data);
            if (this->rooms.exist(roomId)) {
                uint32_t playersAtHost;
                packet >> playersAtHost;
                uint32_t added = this->rooms.addPlayersSafe(roomId, ip, playersAtHost);
                this->logs.add(StringLcl("{new_room}" + roomId.value() + " " + ip.toString() + " " + std::to_string(added)));
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
            this->logs.add(StringLcl("{attempt_to_connect_to_room}" + roomId.value() + " " + ip.toString() + " " + std::to_string(added)));
        }
        else {
            this->logs.add(StringLcl("{attempt_to_connect_to_room_with_unknown_id}" + roomId.value() + " " + ip.toString()));
        }
    }
}
void ServerScreen::drawEverything(sf::RenderWindow& window) {
    window.clear();
    window.draw(MenuBg());
    window.draw(Label(10, 10, window.getSize().x - 20, window.getSize().y - 20, this->logs.get()));
    window.display();
}
void ServerScreen::addTrafficInfo() {
    uint64_t sum = 0;
    for (const auto & connection : this->connections) {
        sum = sum + connection.getCurrentTraffic();
    }

    uint32_t gb = sum / 1024 / 1024 / 1024;
    sum = sum % (1024ull * 1024ull * 1024ull);

    uint32_t mb = sum / 1024 / 1024;
    sum = sum % (1024ull * 1024ull);

    uint32_t kb = sum / 1024;
    sum = sum % 1024;

    uint32_t b = sum;

    this->logs.add(StringLcl("{traffic}" + std::to_string(gb) + "GB, " + std::to_string(mb) + "MB, "  + std::to_string(kb) + "KB, " + std::to_string(b) + "b"));
}