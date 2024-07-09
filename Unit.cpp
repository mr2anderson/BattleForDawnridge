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
Unit::Unit(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHp, uint32_t playerId) : 
	HPGO(x, y, sx, sy, currentHp, maxHp) {
	this->playerId = playerId;

}
void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->HPGO::draw(target, states);
	if (this->exist() and this->getPlayerId() != 0) {
		this->drawPlayerPointer(target, states);
	}
}
bool Unit::belongTo(uint32_t playerId) const {
	return (this->playerId == playerId);
}
void Unit::changePlayer(uint32_t newPlayerId) {
	this->playerId = newPlayerId;
}
uint32_t Unit::getPlayerId() const {
	return this->playerId;
}
Events Unit::getUnitOfEnemyResponse() {
	Events gEvent;
	gEvent.add(std::make_shared<TryToAttackEvent>(this));
	return gEvent;
}
void Unit::drawPlayerPointer(sf::RenderTarget& target, sf::RenderStates states) const {
	PlayerPointer pointer(this->getPlayerId(), this->getX(), this->getY(), this->getSX(), this->getSY());
	target.draw(pointer, states);
}