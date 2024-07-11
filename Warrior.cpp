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


#include "Warrior.hpp"
#include "SelectWarriorEvent.hpp"
#include "StartWarriorClickAnimationEvent.hpp"


Warrior::Warrior() {
	this->startClickAnimation();
}
Warrior::Warrior(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go) :
	Unit(x, y, sx, sy, maxHp, maxHp, playerId, units) {
	this->go = go;
	this->startClickAnimation();
}
Events Warrior::newMove(uint32_t currentPlayerId) {
	if (!this->exist() or !this->belongTo(currentPlayerId)) {
		return Events();
	}
	return Events();
}
void Warrior::startClickAnimation() {
	this->currentDirection = "e";
	this->startAnimation("talking");
}
std::string Warrior::getTextureName() const {
	uint32_t ms = this->animationClock.getElapsedTime().asMilliseconds();
	uint32_t animationNumber = this->getAnimationNumber(this->currentAnimation, this->currentDirection);
	uint32_t msForFrame = 750 / animationNumber;
	uint32_t currentFrame = std::min(ms / msForFrame, animationNumber - 1);

	std::string name = this->getBaseTextureName();
	name = name + " " + this->currentAnimation;
	name = name + " " + this->currentDirection;
	name = name + std::to_string(currentFrame);

	return name;
}
uint32_t Warrior::getAnimationNumber(const std::string &type, const std::string &direction) const {
	if (type == "talking") {
		if (direction == "e") {
			return this->getTalkingAnimationsNumberInSet();
		}
		return 0;
	}
	if (type == "walking") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e") {
			return this->getWalkingAnimationsNumberInSet();
		}
		return 0;
	}
	if (type == "attack") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e") {
			return this->getAttackAnimationsNumberInSet();
		}
		return 0;
	}
	if (type == "been hit") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e" or direction == "nw" or direction == "ne" or direction == "sw" or direction == "se") {
			return this->getBeenHitAnimationsNumberInSet();
		}
		return 0;
	}
	if (type == "tipping over") {
		if (direction == "n" or direction == "s" or direction == "w" or direction == "e" or direction == "nw" or direction == "ne" or direction == "sw" or direction == "se") {
			return this->getTippingOverAnimationsNumberInSet();
		}
		return 0;
	}
	return 0;
}
bool Warrior::canMoveTo(uint32_t newX, uint32_t newY) const {
	sf::IntRect thisRect;
	thisRect.left = newX;
	thisRect.top = newY;
	thisRect.width = this->getSX();
	thisRect.height = this->getSY();

	for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
		GO* o = this->go->at(i);
		if (o->exist() and o->blockForWarrior(this->getPlayerId())) {
			sf::IntRect goRect;
			goRect.left = o->getX();
			goRect.top = o->getY();
			goRect.width = o->getSX();
			goRect.height = o->getSY();
			if (thisRect.intersects(goRect)) {
				return false;
			}
		}
	}

	return true;
}
void Warrior::startAnimation(const std::string &type) {
	this->animationClock.restart();
	this->currentAnimation = type;
}
Events Warrior::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (!this->belongTo(playerId)) {
		return this->getUnitOfEnemyResponse();
	}
	Events response;
	response.add(std::make_shared<SelectWarriorEvent>(this));
	response.add(std::make_shared<StartWarriorClickAnimationEvent>(this));
	return response;
}