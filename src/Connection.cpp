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


Connection::Connection() {
    this->traffic = 0;
    this->error = false;
    this->socket.setBlocking(false);
    this->received = std::make_tuple(false, sf::Packet());
}
uint64_t Connection::getCurrentTraffic() const {
    return this->traffic;
}
sf::TcpSocket& Connection::getSocketRef() {
    return this->socket;
}
std::optional<sf::Packet> Connection::getReceivedPacket() {
    if (std::get<bool>(this->received)) {
        std::get<bool>(this->received) = false;
        return std::get<sf::Packet>(this->received);
    }
    return std::nullopt;
}
bool Connection::hasError() const {
    return this->error;
}
void Connection::send(const sf::Packet &packet) {
    this->toSend.push(packet);
}
void Connection::update() {
    this->processSending();
    this->processReceiving();
}
void Connection::processSending() {
    if (this->toSend.empty()) {
        return;
    }
    sf::Socket::Status status = this->socket.send(this->toSend.front());
    if (status == sf::Socket::Status::Error or status == sf::Socket::Status::Disconnected) {
        this->error = true;
    }
    else if (status == sf::Socket::Status::Done) {
        this->traffic = this->traffic + this->toSend.back().getDataSize();
        this->toSend.pop();
    }
}
void Connection::processReceiving() {
    if (std::get<bool>(this->received)) {
        return;
    }
    if (this->socket.receive(std::get<sf::Packet>(received)) == sf::Socket::Status::Done) {
        std::get<bool>(received) = true;
        this->traffic = this->traffic + std::get<sf::Packet>(received).getDataSize();
    }
}