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
#include "SoundQueue.hpp"
#include "Music.hpp"
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

    if (this->listener.listen(MainServerPosition::get().getPort()) != sf::Socket::Status::Done) {
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
    Music::get().stop();

    sf::Event event;
    bool r = true;
    while (r) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    r = false;
                }
            }
        }
        this->checkNewConnection();
        this->updateSimpleConnections();
        this->checkRoomInitSignals();
        this->rooms.update(&this->toLogs);
        this->handleLogs();
        this->drawEverything(window);
    }
}
void ServerScreen::checkNewConnection() {
    Connection tempConnection;
    if (this->listener.accept(*tempConnection.getSocketRef()) == sf::Socket::Status::Done) {
        this->logs.add(StringLcl("{new_connection}" + tempConnection.getSocketRef()->getRemoteAddress().toString() + " " + tempConnection.getUUID().toString()));
        this->simpleConnections[tempConnection.getUUID()] = tempConnection;
    }
}
void ServerScreen::updateSimpleConnections() {
    std::vector<UUID> toDelete;
    for (auto &connection : this->simpleConnections) {
        connection.second.update();
        if (connection.second.hasError()) {
            this->logs.add(StringLcl("{connection_erased_reason_disconnect}" + connection.second.getUUID().toString()));
            toDelete.push_back(connection.first);
        }
    }
    for (auto a : toDelete) {
        this->simpleConnections.erase(a);
    }
}
static void SEND_ERROR_PACKAGE(Connection& connection, const RoomID &roomID, uint8_t errorCode) {
    sf::Packet packet;
    packet << roomID.value();
    packet << SERVER_NET_SPECS::CODES::ERROR;
    packet << errorCode;
    connection.send(packet);
}
void ServerScreen::checkRoomInitSignals() {
    std::vector<UUID> toDelete;
    for (auto &connection : this->simpleConnections) {
        std::optional<sf::Packet> packetOpt = connection.second.getReceivedPacket();
        if (packetOpt == std::nullopt) {
            continue;
        }
        sf::Packet packet = packetOpt.value();

        this->logs.add(StringLcl("{got_init_signal_from}" + connection.second.getUUID().toString()));

        std::string roomIdVal;
        packet >> roomIdVal;

        RoomID roomId;
        try {
            roomId = RoomID(roomIdVal);
        }
        catch (InvalidRoomIDFormat&) {
            this->logs.add(StringLcl("{invalid_room_id}" + connection.second.getUUID().toString()));
        }

        uint8_t code;
        packet >> code;

        if (code == CLIENT_NET_SPECS::CODES::CREATE) {
            if (this->rooms.exist(roomId)) {
                this->logs.add(StringLcl("{attempt_to_create_room_with_existing_id}" + roomId.value() + " " + connection.second.getUUID().toString()));
                SEND_ERROR_PACKAGE(connection.second, roomId, SERVER_NET_SPECS::CODES::ERROR_CODES::ROOM_ALREADY_EXIST);
            }
            if (!this->rooms.exist(roomId)) {
                std::string data;
                packet >> data;
                this->rooms.createIfValid(roomId, data);
                if (this->rooms.exist(roomId)) {
                    uint32_t playersAtHost;
                    packet >> playersAtHost;
                    uint32_t added = this->rooms.addPlayers(roomId, connection.second, playersAtHost);
                    this->logs.add(StringLcl("{new_room}" + roomId.value() + " " + connection.second.getUUID().toString() + " " + std::to_string(added)));
                    toDelete.push_back(connection.first);
                }
                else {
                    this->logs.add(StringLcl("{couldnt_create_room}" + roomId.value() + " " + connection.second.getUUID().toString()));
                    SEND_ERROR_PACKAGE(connection.second, roomId, SERVER_NET_SPECS::CODES::ERROR_CODES::INVALID_DATA);
                }
            }
        }
        else if (code == CLIENT_NET_SPECS::CODES::CONNECT) {
            if (this->rooms.exist(roomId)) {
                uint32_t playersAtHost;
                packet >> playersAtHost;
                uint32_t added = this->rooms.addPlayers(roomId, connection.second, playersAtHost);
                if (added == 0) {
                    this->logs.add(StringLcl("{attempt_to_connect_to_full_room}" + roomId.value() + " " + connection.second.getUUID().toString()));
                    SEND_ERROR_PACKAGE(connection.second, roomId, SERVER_NET_SPECS::CODES::ERROR_CODES::FULL_ROOM);
                }
                else {
                    this->logs.add(StringLcl("{attempt_to_connect_to_room}" + roomId.value() + " " + connection.second.getUUID().toString() + " " + std::to_string(added)));
                    toDelete.push_back(connection.first);
                }
            }
            else {
                this->logs.add(StringLcl("{attempt_to_connect_to_room_with_unknown_id}" + roomId.value() + " " + connection.second.getUUID().toString()));
                SEND_ERROR_PACKAGE(connection.second, roomId, SERVER_NET_SPECS::CODES::ERROR_CODES::UNKNOWN_ROOM_ID);
            }
        }
    }
    for (auto a : toDelete) {
        this->simpleConnections.erase(a);
    }
}
void ServerScreen::handleLogs() {
    for (uint32_t i = 0; i < this->toLogs.size(); i = i + 1) {
        this->logs.add(this->toLogs.at(i));
    }
    this->toLogs.clear();
}
void ServerScreen::drawEverything(sf::RenderWindow& window) {
    window.clear();
    window.draw(MenuBg());
    window.draw(this->logs.toLabel(window.getSize().x, window.getSize().y));
    window.display();
}