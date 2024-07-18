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


#include "Road.hpp"
#include "RoadSpec.hpp"
#include "Texts.hpp"


Road::Road() = default;
Road::Road(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new RoadSpec());
}
Building* Road::cloneBuilding() const {
	return new Road(*this);
}
uint32_t Road::getSX() const {
    return 1;
}
uint32_t Road::getSY() const {
    return 1;
}
uint32_t Road::getMaxHP() const {
    return 500;
}
Defence Road::getDefence() const {
	return Defence::WOOD;
}
Resources Road::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 1500));
	return cost;
}
uint32_t Road::getRegenerationSpeed() const {
	return this->getMaxHP();
}
std::string Road::getTextureName() const {
	return "road";
}
std::string Road::getSoundName() const {
	return "road";
}
std::wstring Road::getDescription() const {
	return *Texts::get()->get("road_description");
}
std::wstring Road::getUpperCaseReadableName() const {
	return *Texts::get()->get("road_upper_case_readable_name");
}