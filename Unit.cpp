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


#include "Unit.hpp"



Unit::Unit() = default;
Unit::Unit(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHp, std::shared_ptr<Player> playerPtr) : 
	HPGO(x, y, sx, sy, currentHp, maxHp) {
	this->playerPtr = playerPtr;

}
bool Unit::belongTo(std::shared_ptr<Player> player) const {
	return (*this->playerPtr == *player);
}
void Unit::changePlayer(std::shared_ptr<Player> newPlayerPtr) {
	this->playerPtr = newPlayerPtr;
}
std::shared_ptr<Player> Unit::getPlayer() const {
	return this->playerPtr;
}
Events Unit::getUnitOfEnemyResponse() {
	Events gEvent;
	gEvent.add(std::make_shared<TryToAttackEvent>(this));
	return gEvent;
}