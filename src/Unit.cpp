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
#include "ColorTheme.hpp"


Unit::Unit() = default;
Unit::Unit(uint32_t x, uint32_t y, boost::optional<uint32_t> currentHp, uint32_t playerId) :
	HPGO(x, y, currentHp) {
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
StringLcl Unit::getDetailedDescription(MapState *state) const {
    return GO::getDetailedDescription(state) + "\n" +
    StringLcl("{hp}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR(this->getHP(), this->getMaxHP())) + std::to_string(this->getHP()) + "\n" +
    StringLcl("{max_hp}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_BEST) + std::to_string(this->getMaxHP()) + "\n" +
    this->getDefence().getReadable() + "\n";
}
std::shared_ptr<ILightSource> Unit::getLightSource() const {
	std::shared_ptr<PlayerPointer> ptr = this->getPlayerPointer();
	ptr->setSide(this->getPlayerId());
	return ptr->getLightSource();
}
void Unit::drawPlayerPointer(sf::RenderTarget& target, sf::RenderStates states) const {
	std::shared_ptr<PlayerPointer> ptr = this->getPlayerPointer();
    ptr->setSide(this->getPlayerId());
	target.draw(*ptr, states);
}


BOOST_CLASS_EXPORT_IMPLEMENT(Unit)