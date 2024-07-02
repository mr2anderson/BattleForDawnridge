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


#include "ResourceBuilding.hpp"


#pragma once


class Quarry : public ResourceBuilding {
public:
	Quarry(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr);

	Resources getCost() const override;
private:
	uint32_t getRegenerationSpeed() const override;
	std::string getTextureName() const override;
	std::wstring getIsNotBuiltYetStr() const override;
	std::wstring getBuildingFinishedStr() const override;
	std::string getNewWindowSoundName() const override;
	std::string getResourceType() const override;
	uint32_t getResourceNPerMove() const override;
	uint32_t getRadius() const override;
	std::wstring getDescription() const override;
	std::wstring getResourcesOverStr() const override;
	std::wstring getUpgradeStartDescription() const override;
	std::wstring getUpgradeFinishDescription() const override;
	std::wstring getBusyWithUpgradingDescription() const override;
	Resources getUpgradeCost(uint32_t i) const override;
	uint32_t getUpgradeMoves(uint32_t i) const override;
	uint32_t getMaxLevel() const override;

	static constexpr uint32_t TOTAL_LEVELS = 3;
	static const Resources UPGRADE_COSTS[];
	static const uint32_t UPGRADE_MOVES[];
	static const float LEVEL_BONUS[];
	static const uint32_t LEVEL_RADIUS_BONUS[];
};