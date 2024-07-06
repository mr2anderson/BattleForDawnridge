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
	this->currentHp = this->currentHp - n;
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
	uint32_t size = (32 / 4 * this->getSY() - 32 * this->getSY() / 8) * this->currentHp / this->maxHp;

	uint32_t posX = 32 * this->getX() + 4;
	uint32_t posY = 32 * this->getY() + 4;

	uint32_t red = 255 - 255 * this->currentHp / this->maxHp;
	uint32_t green = 255 * this->currentHp / this->maxHp;
	uint32_t blue = 0;

	sf::RectangleShape rect1;
	rect1.setPosition(posX, posY);
	rect1.setFillColor(sf::Color::Black);
	rect1.setOutlineColor(sf::Color::Black);
	rect1.setOutlineThickness(1);
	rect1.setSize(sf::Vector2f(2, 32 / 4 * this->getSY() - 32 * this->getSY() / 8));
	target.draw(rect1);

	sf::RectangleShape rect2;
	rect2.setPosition(sf::Vector2f(posX, posY) + sf::Vector2f(0, 32 / 4 * this->getSY() - 32 * this->getSY() / 8 - size));
	rect2.setFillColor(sf::Color(red, green, blue));
	rect2.setSize(sf::Vector2f(2, size));
	target.draw(rect2, states);
}