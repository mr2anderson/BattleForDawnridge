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



#include "Mine.hpp"


Mine::Mine() = default;
Mine::Mine(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : 
	ResourceB(x, y, 4, 4, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 4, 4, 20000, playerPtr) {}
Resources Mine::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Mine::getRegenerationSpeed() const {
	return 10000;
}
std::string Mine::getTextureName() const {
	return "mine";
}
std::string Mine::getNewWindowSoundName() const {
	return "iron";
}
std::string Mine::getResourceType() const {
	return "iron";
}
uint32_t Mine::getCollectionSpeed(uint32_t level) const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		6
	};
	return 2500 * levelCollectionSpeedBonus[level];
}
uint32_t Mine::getRadius(uint32_t level) const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 5 + levelRadiusBonus[level];
}
std::wstring Mine::getDescription() const {
	return
		L"Шахты обеспечивают Ваш город железом, необходимым для создания армии.\n"
		+ this->getReadableHpInfo();
}
Resources Mine::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 20000}}),
		Resources({{"wood", 40000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Mine::getUpgradeTime() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
std::wstring Mine::getReadableName() const {
	return L"шахта";
}