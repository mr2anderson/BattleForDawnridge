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



#include "Sawmill.hpp"


Sawmill::Sawmill() = default;
Sawmill::Sawmill(uint32_t x, uint32_t y, Player* playerPtr, std::vector<ResourcePoint*>* resourcePointsPtr) : 
	ResourceB(x, y, 3, 3, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 3, 3, 20000, playerPtr) {}
Resources Sawmill::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Sawmill::getRegenerationSpeed() const {
	return 10000;
}
std::string Sawmill::getTextureName() const {
	return "sawmill";
}
std::string Sawmill::getSoundName() const {
	return "wood";
}
std::string Sawmill::getResourceType() const {
	return "wood";
}
uint32_t Sawmill::getCollectionSpeed(uint32_t level) const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		6
	};
	return 2500 * levelCollectionSpeedBonus[level];
}
uint32_t Sawmill::getRadius(uint32_t level) const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 5 + levelRadiusBonus[level];
}
std::wstring Sawmill::getDescription() const {
	return L"Лесопилки обеспечивают Ваш город древесиной, необходимой для строительства большинства зданий.";
}
Resources Sawmill::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 20000}}),
		Resources({{"wood", 40000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Sawmill::getUpgradeTime() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
std::wstring Sawmill::getReadableName() const {
	return L"лесопилка";
}