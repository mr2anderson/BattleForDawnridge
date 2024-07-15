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
#include "Texts.hpp"


Mine::Mine() = default;
Mine::Mine(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	ResourceB(x, y,  playerId, units, resourcePointsPtr, go, mapW, mapH),
	Building(x, y, playerId, units) {}
Building* Mine::cloneBuilding() const {
	return new Mine(*this);
}
uint32_t Mine::getSX() const {
    return 2;
}
uint32_t Mine::getSY() const {
    return 2;
}
uint32_t Mine::getMaxHP() const {
    return 5000;
}
Defence Mine::getDefence() const {
	return DEFENCE::WOOD;
}
Resources Mine::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Mine::getRegenerationSpeed() const {
	return this->getMaxHP() / 2;
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
	return 1750;
}
uint32_t Mine::getRadius() const {
	return 4;
}
std::wstring Mine::getDescription() const {
	return *Texts::get()->get("mine_description");
}
std::wstring Mine::getUpperCaseReadableName() const {
	return *Texts::get()->get("mine_upper_case_readable_name");
}