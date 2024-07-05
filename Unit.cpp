/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Unit.hpp"


Unit::Unit() = default;
Unit::Unit(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHp, const Player* playerPtr) : 
	GO(x, y, sx, sy, currentHp, maxHp) {
	this->playerPtr = playerPtr;
}
bool Unit::belongTo(const Player* player) const {
	return (*this->playerPtr == *player);
}
Event Unit::getUnitOfEnemyResponse() {
	Event gEvent;
	gEvent.tryToAttack.push_back(this);
	return gEvent;
}
const Player* Unit::getPlayer() const {
	return this->playerPtr;
}