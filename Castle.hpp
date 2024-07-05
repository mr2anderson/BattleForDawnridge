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


#include "TerritoryOriginB.hpp"
#include "UpgradeableB.hpp"
#include "SelectionW.hpp"


#pragma once


class Castle : public TerritoryOriginB, public UpgradeableB {
public:
	Castle();
	Castle(uint32_t x, uint32_t y, const Player* playerPtr);
	Events newMove(const Player& player) override;

	bool works() const override;
	Resources getCost() const override;
	std::string getTextureName() const override;
	std::string getSoundName() const override;
	std::wstring getDescription() const override;
private:
	static const uint32_t LEVEL_HP[];

	static uint32_t GET_REGENERATION_SPEED(uint32_t level);
	uint32_t getRegenerationSpeed() const override;
	std::wstring getReadableName() const override;
	Resources getUpgradeCost() const override;
	uint32_t getUpgradeTime() const override;
	uint32_t getRadius() const override;
	Events getSelectionW();
	Events getGameObjectResponse(const Player& player) override;
};