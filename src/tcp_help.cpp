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


#include "tcp_help.hpp"


static const sf::Time DELTA = sf::milliseconds(5);


void bfdlib::tcp_help::process_sending(sf::TcpSocket *socket, packet_queue *q, const std::atomic<bool> *flag) {
    for (; ; sf::sleep(DELTA)) {
        if (*flag) {
            break;
        }
        if (q->empty()) {
            continue;
        }
        sf::Packet packet = q->pop();
        if (socket->send(packet) != sf::Socket::Status::Done) {
            break;
        }
    }
}
void bfdlib::tcp_help::process_receiving(sf::TcpSocket *socket, packet_queue *q, const std::atomic<bool> *flag) {
    for (; ; sf::sleep(DELTA)) {
        if (*flag) {
            break;
        }
        sf::Packet packet;
        if (socket->receive(packet) != sf::Socket::Status::Done) {
            break;
        }
        q->push(packet);
    }
}