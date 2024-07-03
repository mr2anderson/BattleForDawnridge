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


#include "GameObject.hpp"


GameObject::GameObject() = default;
GameObject::GameObject(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHp) {
	this->bar = HPBar(currentHp, maxHp, x, y, sx, sy);
}
void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!this->exist()) {
		return;
	}

	sf::Sprite sprite;
	sprite.setPosition(sf::Vector2f(64 * this->getX(), 64 * this->getY()));
	sprite.setTexture(*TextureStorage::get()->get(this->getTextureName()));
	
	target.draw(sprite, states);
	target.draw(this->bar, states);
}
void GameObject::addHp(uint32_t value) {
	this->bar = this->bar + value;
}
void GameObject::subHp(uint32_t value) {
	this->bar = this->bar - value;
}
void GameObject::changeMaxHp(uint32_t value) {
	this->bar.changeMax(value);
}
uint32_t GameObject::getHP() const {
	return this->bar.getCurrent();
}
uint32_t GameObject::getMaxHP() const {
	return this->bar.getMax();
}
uint32_t GameObject::getX() const {
	return this->bar.getX();
}
uint32_t GameObject::getY() const {
	return this->bar.getY();
}
uint32_t GameObject::getSX() const {
	return this->bar.getSX();
}
uint32_t GameObject::getSY() const {
	return this->bar.getSY();
}
bool GameObject::exist() const {
	return (this->getHP() > 0);
}
GameObjectResponse GameObject::click(const Player &currentPlayer, uint32_t mouseX, uint32_t mouseY) {
	if (mouseX >= 64 * this->getX() and
		mouseY >= 64 * this->getY() and
		mouseX < 64 * (this->getX() + this->getSX()) and
		mouseY < 64 * (this->getY() + this->getSY())) {
		return this->getGameObjectResponse(currentPlayer);
	}
	return GameObjectResponse();
}
GameObjectResponse GameObject::newMove(const Player& currentPlayer) {
	return GameObjectResponse();
}