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


#include "Building.hpp"
#include "SelectWindow.hpp"


#pragma once


class Castle : public Building {
public:
	Castle(uint32_t x, uint32_t y, const Player* playerPtr);
	GameObjectResponse newMove(const Player& player) override;
	GameObjectResponse upgrade() override;
	
	Resources getCost() const override;
private:
	static constexpr uint32_t TOTAL_LEVELS = 3;
	static const Resources UPGRADE_COSTS[];
	static const uint32_t UPGRADE_MOVES[];
	static const uint32_t UPGRADE_HP[];

	GameObjectResponse getSelectWindow();

	uint32_t getRegenerationSpeed() const override;
	std::string getTextureName() const override;
	std::string getNewWindowSoundName() const override;
	std::wstring getUpgradeStartDescription() const override;
	std::wstring getUpgradeFinishDescription() const override;
	std::wstring getBusyWithUpgradingDescription() const override;
	GameObjectResponse decreaseUpgradeMovesLeft() override;
	GameObjectResponse getGameObjectResponse(const Player& player) override;
};