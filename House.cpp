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


#include <random>
#include "House.hpp"
#include "HouseSpec.hpp"
#include "Texts.hpp"


const uint32_t House::TOTAL_TYPES = 16;


House::House() {
	this->type = 1;
}
House::House(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {

	std::random_device rd;
	this->type = rd() % TOTAL_TYPES + 1;

	this->addSpec(new HouseSpec());
}
Building* House::cloneBuilding() const {
	return new House(*this);
}
uint32_t House::getSX() const {
	return 2;
}
uint32_t House::getSY() const {
	return 2;
}
uint32_t House::getMaxHP() const {
	return 5000;
}
Defence House::getDefence() const {
	return Defence::WOOD;
}
Resources House::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 15000));
	return cost;
}
std::string House::getTextureName() const {
	return "house" + std::to_string(this->type);
}
std::string House::getSoundName() const {
	return "bottles";
}
std::wstring House::getDescription() const {
	return *Texts::get()->get("house_description");
}
uint32_t House::getRegenerationSpeed() const {
	return this->getMaxHP() / 2;
}
std::wstring House::getUpperCaseReadableName() const {
	return *Texts::get()->get("house_upper_case_readable_name");
}