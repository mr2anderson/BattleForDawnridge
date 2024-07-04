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


#include "ResourceB.hpp"


#pragma once


class Mine : public ResourceB {
public:
	Mine();
	Mine(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr);

	Resources getCost() const override;
private:
	uint32_t getRegenerationSpeed() const override;
	std::string getTextureName() const override;
	std::string getNewWindowSoundName() const override;
	std::string getResourceType() const override;
	uint32_t getCollectionSpeed(uint32_t level) const override;
	uint32_t getRadius(uint32_t level) const override;
	std::wstring getDescription() const override;
	Resources getUpgradeCost() const override;
	uint32_t getUpgradeTime() const override;
	std::wstring getReadableName() const override;
};