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



#include "Mine.hpp"


Mine::Mine() = default;
Mine::Mine(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr) : 
	ResourceB(x, y, 3, 3, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 3, 3, 20000, playerPtr) {}
Building* Mine::cloneBuilding() const {
	return new Mine(*this);
}
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
std::string Mine::getSoundName() const {
	return "iron";
}
std::string Mine::getResourceType() const {
	return "iron";
}
uint32_t Mine::getCollectionSpeed() const {
	return 2000;
}
uint32_t Mine::getRadius() const {
	return 5;
}
std::wstring Mine::getDescription() const {
	return *Texts::get()->get("mine_description");
}
std::wstring Mine::getUpperCaseReadableName() const {
	return *Texts::get()->get("mine_upper_case_readable_name");
}