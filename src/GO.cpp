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
#include "Locales.hpp"
#include "CreateEEvent.hpp"
#include "ResetHighlightEvent.hpp"


#include "PlaySoundEvent.hpp"


const DrawingPriority GO::DRAWING_PRIORITY_WARRIOR_FLYING = {3};
const DrawingPriority GO::DRAWING_PRIORITY_WARRIOR_DEFAULT = {2};
const DrawingPriority GO::DRAWING_PRIORITY_WARRIOR_VEHICLE = {1};
const DrawingPriority GO::DRAWING_PRIORITY_LOWEST = {0};

const ClickPriority GO::CLICK_PRIORITY_WARRIOR {1};
const ClickPriority GO::CLICK_PRIORITY_LOWEST = {0};

const NewMoveMainPriority GO::NEW_MOVE_MAIN_PRIORITY_VICTORY_CONDITION = {6};
const NewMoveMainPriority GO::NEW_MOVE_MAIN_PRIORITY_WARRIOR = { 5 };
const NewMoveMainPriority GO::NEW_MOVE_MAIN_PRIORITY_SHOOTING = {4};
const NewMoveMainPriority GO::NEW_MOVE_MAIN_PRIORITY_TERRITORY_CONDUCTOR = {3};
const NewMoveMainPriority GO::NEW_MOVE_MAIN_PRIORITY_HOUSE = {2};
const NewMoveMainPriority GO::NEW_MOVE_MAIN_PRIORITY_WAREHOUSE = {1};
const NewMoveMainPriority GO::NEW_MOVE_MAIN_PRIORITY_LOWEST = {0};


GO::GO() = default;
GO::GO(uint32_t x, uint32_t y) {
	this->x = x;
	this->y = y;
}
bool GO::inView(const sf::View& view) const {
	sf::FloatRect thisRect;
	thisRect.left = 64 * this->x;
	thisRect.top = 64 * this->y;
	thisRect.width = 64 * this->getSX();
	thisRect.height = 64 * this->getSY();

	sf::FloatRect viewRect;
	viewRect.left = view.getCenter().x - view.getSize().x / 2;
	viewRect.top = view.getCenter().y - view.getSize().y / 2;
	viewRect.width = viewRect.left + view.getSize().x;
	viewRect.height = viewRect.top + view.getSize().y;

	return thisRect.intersects(viewRect);
}
void GO::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!this->exist()) {
		return;
	}
	this->drawTexture(target, states);
}
float GO::getXInPixels() const {
    return 64 * this->x + this->getOffsetX();
}
float GO::getYInPixels() const {
    return 64 * this->y + this->getOffsetY();
}
float GO::getCenterX() const {
	return this->getXInPixels() + 64 * this->getSX() / 2;
}
float GO::getCenterY() const {
	return this->getYInPixels() + 64 * this->getSY() / 2;
}
uint32_t GO::getX() const {
	return this->x;
}
uint32_t GO::getY() const {
	return this->y;
}
UUID GO::getUUID() const {
    return this->uuid;
}
void GO::setX(uint32_t newX) {
	this->x = newX;
}
void GO::setY(uint32_t newY) {
	this->y = newY;
}
bool GO::warriorCanStay(const Warrior *w) const {
	return true;
}
uint32_t GO::getWarriorMovementCost(const Warrior *w) const {
	return 1;
}
bool GO::isUltraHighObstacle(uint32_t playerId) const {
    return false;
}
bool GO::isHighObstacle(uint32_t playerId) const {
    return false;
}
bool GO::isLowObstacle(uint32_t playerId) const {
    return false;
}
bool GO::exist() const {
	return true;
}
Events GO::click(MapState *state, uint32_t currentPlayerId, uint8_t button, uint32_t mouseX, uint32_t mouseY) {
	if (mouseX >= 64 * this->getX() and
		mouseY >= 64 * this->getY() and
		mouseX < 64 * (this->getX() + this->getSX()) and
		mouseY < 64 * (this->getY() + this->getSY())) {
		return this->getResponse(state, currentPlayerId, button);
	}
	return Events();
}
void GO::update(MapState *state, uint32_t playerId) {

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
Events GO::newMove(MapState* state, uint32_t currentPlayerId) {
	return Events();
}
DrawingPriority GO::getDrawingPriority() const {
	return GO::DRAWING_PRIORITY_LOWEST;
}
ClickPriority GO::getClickPriority() const {
	return GO::CLICK_PRIORITY_LOWEST;
}
NewMoveMainPriority GO::getNewMoveMainPriority() const {
    return GO::NEW_MOVE_MAIN_PRIORITY_LOWEST;
}
NewMovePriority GO::getNewMovePriority() const {
	return { this->getNewMoveMainPriority(), {this->getTypeUUID()}};
}
sf::IntRect GO::getTextureRect() const {
	return sf::IntRect(0, 0, 64 * this->getSX(), 64 * this->getSY());
}
sf::Color GO::getTextureColor() const {
	return sf::Color::White;
}
float GO::getOffsetX() const {
    return 0;
}
float GO::getOffsetY() const {
    return 0;
}
HorizontalSelectionWindowComponent GO::getExitComponent() const {
    Events exitEvent;
    exitEvent.add(std::make_shared<PlaySoundEvent>("click"));
	exitEvent.add(std::make_shared<ResetHighlightEvent>());

	HorizontalSelectionWindowComponent component = {
		  "exit_icon",
		StringLcl("{leave}"),
		true,
		exitEvent
	};
	return component;
}
HorizontalSelectionWindowComponent GO::getDescriptionComponent() const {
	HorizontalSelectionWindowComponent component = {
		this->getTextureName(),
		this->getDescription(),
		false,
		Events(),
		IntRectSerializable(this->getTextureRect())
	};
	return component;
}
void GO::drawTexture(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	sprite.setPosition(sf::Vector2f(this->getXInPixels() , this->getYInPixels()));
	sprite.setTextureRect(this->getTextureRect());
	sprite.setTexture(*Textures::get()->get(this->getTextureName()));
	sprite.setColor(this->getTextureColor());
	target.draw(sprite, states);
}


BOOST_CLASS_EXPORT_IMPLEMENT(GO)