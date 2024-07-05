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


#include "GO.hpp"


GO::GO() = default;
GO::GO(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHp) {
	this->bar = HPBar(currentHp, maxHp, x, y, sx, sy);
}
void GO::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!this->exist()) {
		return;
	}
	this->drawTexture(target, states);
	target.draw(this->bar, states);
}
void GO::addHp(uint32_t value) {
	this->bar = this->bar + value;
}
void GO::subHp(uint32_t value) {
	this->bar = this->bar - value;
}
void GO::setMaxHp() {
	this->bar.setMax();
}
void GO::changeMaxHp(uint32_t value) {
	this->bar.changeMax(value);
}
uint32_t GO::getHP() const {
	return this->bar.getCurrent();
}
uint32_t GO::getMaxHP() const {
	return this->bar.getMax();
}
uint32_t GO::getX() const {
	return this->bar.getX();
}
uint32_t GO::getY() const {
	return this->bar.getY();
}
uint32_t GO::getSX() const {
	return this->bar.getSX();
}
uint32_t GO::getSY() const {
	return this->bar.getSY();
}
void GO::setX(uint32_t newX) {
	this->bar.setX(newX);
}
void GO::setY(uint32_t newY) {
	this->bar.setY(newY);
}
bool GO::exist() const {
	return (this->getHP() > 0);
}
Event GO::click(const Player &currentPlayer, uint32_t mouseX, uint32_t mouseY) {
	if (mouseX >= 32 * this->getX() and
		mouseY >= 32 * this->getY() and
		mouseX < 32 * (this->getX() + this->getSX()) and
		mouseY < 32 * (this->getY() + this->getSY())) {
		return this->getGameObjectResponse(currentPlayer);
	}
	return Event();
}
std::wstring GO::getReadableHpInfo() const {
	return this->bar.getReadableInfo();
}
void GO::drawTexture(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	sprite.setPosition(sf::Vector2f(32 * this->getX(), 32 * this->getY()));
	sprite.setTexture(*Textures::get()->get(this->getTextureName()));
	target.draw(sprite, states);
}