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
#include "GOCollection.hpp"
#include "SelectWarriorEvent.hpp"
#include "StartWarriorClickAnimationEvent.hpp"


#pragma once


class Warrior : public Unit {
public:
	Warrior();
	Warrior(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<GO>> go);
	virtual Warrior* cloneWarrior() const = 0;

	Events newMove(uint32_t playerId);
	void startClickAnimation();
	std::string getTextureName() const override;
	uint32_t getAnimationNumber(const std::string& type, const std::string& direction) const;
	bool canMoveTo(uint32_t newX, uint32_t newY) const;

	virtual uint32_t getTimeToProduce() const = 0;
	virtual std::string getBaseTextureName() const = 0;
	virtual uint32_t getTalkingAnimationsNumberInSet() const = 0;
	virtual uint32_t getWalkingAnimationsNumberInSet() const = 0;
	virtual uint32_t getAttackAnimationsNumberInSet() const = 0;
	virtual uint32_t getBeenHitAnimationsNumberInSet() const = 0;
	virtual uint32_t getTippingOverAnimationsNumberInSet() const = 0;
private:
	std::shared_ptr<GOCollection<GO>> go;
	std::string currentDirection;
	std::string currentAnimation;
	sf::Clock animationClock;

	void startAnimation(const std::string &type);
	Events getGameObjectResponse(uint32_t playerId) override;
};