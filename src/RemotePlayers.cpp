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


#include "RemotePlayers.hpp"


RemotePlayers::RemotePlayers() = default;
void RemotePlayers::set(RemotePlayer player) {
	if (this->data.size() < player.getId()) {
		this->data.resize(player.getId());
	}
	this->data.at(player.getId() - 1) = player;
}
void RemotePlayers::add(UUID uuid) {
    this->set(RemotePlayer(this->size() + 1, uuid));
}
RemotePlayer RemotePlayers::get(uint32_t id) const {
    if (id - 1 >= this->data.size()) {
        return {id, INVALID_UUID};
    }
	return this->data.at(id - 1);
}
uint32_t RemotePlayers::size() const {
	return this->data.size();
}