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


#include "HPGO.hpp"
#include "HPPointer.hpp"


HPGO::HPGO() = default;
HPGO::HPGO(uint32_t x, uint32_t y, std::optional<uint32_t> currentHp) :
	GO(x, y) {
	this->currentHp = currentHp;
}
void HPGO::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->GO::draw(target, states);
	if (this->exist()) {
		this->drawHpBar(target, states);
	}
}
void HPGO::addHp(uint32_t n) {
	this->currentHp = std::min(this->getHP() + n, this->getMaxHP());
}
void HPGO::subHp(uint32_t n) {
	if (n >= this->getHP()) {
		this->currentHp = 0;
	}
	else {
		this->currentHp = this->getHP() - n;
	}
}
uint32_t HPGO::getHP() const {
	return this->currentHp.value_or(this->getMaxHP());
}
void HPGO::setMaxHp() {
	this->currentHp = this->getMaxHP();
}
bool HPGO::exist() const {
	return (this->getHP() != 0);
}
void HPGO::drawHpBar(sf::RenderTarget& target, sf::RenderStates states) const {
	HPPointer p(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), this->getHPPointerOrientation());

	p.setCurrent(this->getHP());
	p.setMax(this->getMaxHP());

	target.draw(p, states);
}