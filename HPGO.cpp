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


HPGO::HPGO() = default;
HPGO::HPGO(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHP) :
	GO(x, y, sx, sy) {
	this->currentHp = currentHp;
	this->maxHp = maxHP;
}
void HPGO::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->GO::draw(target, states);
	if (this->exist()) {
		this->drawHpBar(target, states);
	}
}
void HPGO::addHp(uint32_t n) {
	this->currentHp = std::min(this->currentHp + n, this->maxHp);
}
void HPGO::subHp(uint32_t n) {
	if (n >= this->currentHp) {
		this->currentHp = 0;
	}
	else {
		this->currentHp = this->currentHp - n;
	}
}
uint32_t HPGO::getHP() const {
	return this->currentHp;
}
uint32_t HPGO::getMaxHP() const {
	return this->maxHp;
}
void HPGO::changeCurrentHp(uint32_t newCurrentHp) {
	this->currentHp = std::min(this->maxHp, newCurrentHp);
}
void HPGO::changeMaxHp(uint32_t newMaxHp) {
	this->maxHp = newMaxHp;
	this->currentHp = std::min(this->currentHp, this->maxHp);
}
void HPGO::setMaxHp() {
	this->currentHp = this->maxHp;
}
std::wstring HPGO::getReadableHpInfo() const {
	return *Texts::get()->get("hp") + std::to_wstring(this->getHP()) + *Texts::get()->get("slash") + std::to_wstring(this->getMaxHP());
}
bool HPGO::exist() const {
	return (this->currentHp != 0);
}
void HPGO::drawHpBar(sf::RenderTarget& target, sf::RenderStates states) const {
	HPBar bar(this->currentHp, this->maxHp, this->getX(), this->getY(), this->getSX(), this->getSY());
	target.draw(bar, states);
}