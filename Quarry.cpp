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



#include "Quarry.hpp"


Quarry::Quarry() = default;
Quarry::Quarry(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr) : 
	ResourceB(x, y, 3, 3, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 3, 3, 20000, playerPtr) {}
Building* Quarry::cloneBuilding() const {
	return new Quarry(*this);
}
Resources Quarry::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Quarry::getRegenerationSpeed() const {
	return 10000;
}
std::string Quarry::getTextureName() const {
	return "quarry";
}
std::string Quarry::getSoundName() const {
	return "stone";
}
std::string Quarry::getResourceType() const {
	return "stone";
}
uint32_t Quarry::getCollectionSpeed(uint32_t level) const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		9
	};
	return 1000 * levelCollectionSpeedBonus[level];
}
uint32_t Quarry::getRadius(uint32_t level) const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 4 + levelRadiusBonus[level];
}
std::wstring Quarry::getDescription() const {
	return *Texts::get()->get("quarry_description");
}
Resources Quarry::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 16000}}),
		Resources({{"wood", 48000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Quarry::getUpgradeTime() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
std::wstring Quarry::getUpperCaseReadableName() const {
	return *Texts::get()->get("quarry_upper_case_readable_name");
}