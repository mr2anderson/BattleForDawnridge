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


#include "Dragon.hpp"
#include "Unit.hpp"
#include "GOCollection.hpp"


#pragma once


class RedDragon : public Dragon {
public:
	RedDragon();
	RedDragon(std::shared_ptr<GOCollection<Unit>> units, uint32_t playerId);
	Dragon* clone() const override;

	std::wstring getDescriptionText() const override;
	std::wstring getRestText() const override;
	std::wstring getRaiseText() const override;
	std::wstring getAttackText() const override;
	std::string getTextureName() const override;
	Resources getCost() const override;
	uint32_t getRecoverTime() const override;
	std::string getInfoSoundName() const override;
private:
	std::shared_ptr<GOCollection<Unit>> units;

	std::string getAttackSoundName() const override;
	std::shared_ptr<DragonFlame> getFlameType() const override;
	Events doAttack(uint32_t centerX, uint32_t centerY) override;
};