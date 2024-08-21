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
#include <memory>
#include "tcp_helper.hpp"


#pragma once


class Connection {
public:
    Connection();
    ~Connection();

    uint64_t getCurrentTraffic() const;
    sf::TcpSocket& getSocketRef();
    std::optional<sf::Packet> getReceivedPacket();
    void send(const sf::Packet &packet);
    void run();
private:
    std::atomic<uint64_t> traffic;
    std::atomic<bool> stop;
    sf::TcpSocket socket;
    bfdlib::tcp_helper::queue_r received;
    bfdlib::tcp_helper::queue_w toSend;
    std::unique_ptr<sf::Thread> sendThread;
    std::unique_ptr<sf::Thread> receiveThread;
};