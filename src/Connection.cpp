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


#include "Connection.hpp"


/*Connection::Connection() {
    this->traffic.store(0);
    this->stop.store(false);
    this->error.store(false);
    this->socket = std::make_shared<sf::TcpSocket>();
    this->socket->setBlocking(false);
    this->sendThread = nullptr;
    this->receiveThread = nullptr;
}
Connection::~Connection() {
    this->stop = true;
}
uint64_t Connection::getCurrentTraffic() const {
    return this->traffic;
}
std::shared_ptr<sf::TcpSocket>& Connection::getSocketRef() {
    return this->socket;
}
std::optional<sf::Packet> Connection::getReceivedPacket() {
    return this->received.pop();
}
bool Connection::hasError() const {
    return this->error;
}
void Connection::send(const sf::Packet &packet) {
    this->toSend.push(packet);
}
void Connection::run() {
    this->sendThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_helper::process_sending, std::ref(this->socket), std::ref(this->toSend), std::ref(this->stop), std::ref(this->traffic), std::ref(this->error)));
    this->receiveThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_helper::process_receiving, std::ref(this->socket), std::ref(this->received), std::ref(this->stop), std::ref(this->traffic)));
    this->sendThread->launch();
    this->receiveThread->launch();
}*/