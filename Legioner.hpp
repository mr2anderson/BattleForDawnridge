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


#pragma once


class Legioner : public Warrior {
public:
	Legioner();
	Legioner(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH);
	Warrior* cloneWarrior() const override;

	Resources getCost() const override;
	uint32_t getTimeToProduce() const override;
	std::string getSoundName() const override;
	std::wstring getDescription() const override;
private:
	uint32_t getTalkingAnimationsNumberInSet() const override;
	uint32_t getWalkingAnimationsNumberInSet() const override;
	uint32_t getAttackAnimationsNumberInSet() const override;
	uint32_t getBeenHitAnimationsNumberInSet() const override;
	uint32_t getTippingOverAnimationsNumberInSet() const override;
	std::string getBaseTextureName() const override;
	uint32_t getMovementPoints() const override;
};