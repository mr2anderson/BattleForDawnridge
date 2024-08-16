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


#include "Players.hpp"


Players::Players() = default;
void Players::addPlayer(const Player& player) {
	uint32_t index = player.getId() - 1;
	if (this->v.size() <= index) {
		this->v.resize(index + 1);
	}
	this->v.at(index) = player;
}
Player* Players::getPlayerPtr(uint32_t id) {
	return &this->v.at(id - 1);
}
uint32_t Players::total() const {
	return this->v.size();
}