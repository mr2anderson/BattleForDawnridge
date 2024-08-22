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


#include "RemotePlayer.hpp"


RemotePlayer::RemotePlayer() = default;
RemotePlayer::RemotePlayer(uint32_t id, sf::IpAddress ip) {
	this->id = id;
	this->ip = ip;
}
uint32_t RemotePlayer::getId() const {
	return this->id;
}
sf::IpAddress RemotePlayer::getIp() const {
	return this->ip;
}
void RemotePlayer::disconnect() {
    this->ip = sf::IpAddress::None;
}
bool RemotePlayer::connected() const {
    return (this->ip != sf::IpAddress::None);
}