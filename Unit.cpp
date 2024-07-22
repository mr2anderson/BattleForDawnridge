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
Unit::Unit(uint32_t x, uint32_t y, std::optional<uint32_t> currentHp, uint32_t playerId) :
	HPGO(x, y, currentHp) {
	this->playerId = playerId;
}
Events Unit::hit(Damage d) {
	uint32_t dPoints = d.getHpLoss(this->getDefence());
	return this->hit(dPoints);
}
void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (this->exist() and this->getPlayerId() != 0) {
		this->drawPlayerPointer(target, states);
	}
	this->HPGO::draw(target, states);
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
void Unit::drawPlayerPointer(sf::RenderTarget& target, sf::RenderStates states) const {
	std::shared_ptr<PlayerPointer> ptr = this->getPlayerPointer();
    ptr->setSide(this->getPlayerId());
	target.draw(*ptr, states);
}