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


#include <atomic>
#include <cstdint>
#include <SFML/Network.hpp>
#include <optional>
#include <queue>


#pragma once


class Connection {
public:
    Connection();

    uint64_t getCurrentTraffic() const;
    sf::TcpSocket& getSocketRef();
    std::optional<sf::Packet> getReceivedPacket();
    bool hasError() const;
    void send(const sf::Packet &packet);
    void update();
private:
    uint64_t traffic;
    bool error;
    sf::TcpSocket socket;
    std::queue<sf::Packet> toSend;
    std::tuple<bool, sf::Packet> received;

    void processSending();
    void processReceiving();
};