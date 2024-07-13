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


#include "GO.hpp"
#include "Textures.hpp"
#include "Texts.hpp"
#include "CreateEEvent.hpp"


GO::GO() = default;
GO::GO(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
}
void GO::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!this->exist()) {
		return;
	}
	this->drawTexture(target, states);
}
uint32_t GO::getX() const {
	return this->x;
}
uint32_t GO::getY() const {
	return this->y;
}
uint32_t GO::getSX() const {
	return this->sx;
}
uint32_t GO::getSY() const {
	return this->sy;
}
void GO::setX(uint32_t newX) {
	this->x = newX;
}
void GO::setY(uint32_t newY) {
	this->y = newY;
}
bool GO::isHighObstacle() const {
    return false;
}
bool GO::isLowObstacle() const {
    return false;
}
Events GO::click(uint32_t currentPlayerId, uint32_t mouseX, uint32_t mouseY) {
	if (mouseX >= 64 * this->getX() and
		mouseY >= 64 * this->getY() and
		mouseX < 64 * (this->getX() + this->getSX()) and
		mouseY < 64 * (this->getY() + this->getSY())) {
		return this->getGameObjectResponse(currentPlayerId);
	}
	return Events();
}
bool GO::intersects(GO* go) const {
	sf::IntRect rect1;
	rect1.left = this->getX();
	rect1.top = this->getY();
	rect1.width = this->getSX();
	rect1.height = this->getSY();

	sf::IntRect rect2;
	rect2.left = go->getX();
	rect2.top = go->getY();
	rect2.width = go->getSX();
	rect2.height = go->getSY();

	return rect1.intersects(rect2);
}
bool GO::highDrawingPriority() const {
	return false;
}
bool GO::highClickPriority() const {
	return false;
}
HorizontalSelectionWindowComponent GO::getExitComponent() {
	HorizontalSelectionWindowComponent component = {
		"exit_icon",
		*Texts::get()->get("leave"),
		true,
		true,
		Events()
	};
	return component;
}
HorizontalSelectionWindowComponent GO::getDescriptionComponent() const {
	HorizontalSelectionWindowComponent component = {
		this->getTextureName(),
		this->getDescription(),
		false,
		false,
		Events()
	};
	return component;
}
void GO::drawTexture(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	sprite.setPosition(sf::Vector2f(64 * this->getX(), 64 * this->getY()));
	sprite.setTexture(*Textures::get()->get(this->getTextureName()));
	target.draw(sprite, states);
}