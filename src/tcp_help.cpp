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


void bfdlib::tcp_help::process_sending(sf::TcpSocket *socket, queue_w *q, const std::atomic<bool> *flag, std::atomic<uint64_t> *bytes) {
    *bytes = 0;
    for (; ; sf::sleep(DELTA)) {
        if (*flag) {
            break;
        }
        if (q->empty()) {
            continue;
        }
        sf::Packet packet = q->pop();
        *bytes = *bytes + packet.getDataSize();
        while (socket->send(packet) != sf::Socket::Status::Done) {
            sf::sleep(DELTA);
            if (*flag) {
                break;
            }
        }
    }
}
void bfdlib::tcp_help::process_receiving(sf::TcpSocket *socket, queue_r *q, const std::atomic<bool> *flag, std::atomic<uint64_t> *bytes) {
    *bytes = 0;
    for (; ; sf::sleep(DELTA)) {
        if (*flag) {
            break;
        }
        sf::Packet packet;
        while (socket->receive(packet) != sf::Socket::Status::Done) {
            sf::sleep(DELTA);
            if (*flag) {
                break;
            }
        }
        *bytes = *bytes + packet.getDataSize();
        q->push(packet);
    }
}