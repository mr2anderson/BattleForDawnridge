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


#include "House.hpp"
#include "HouseSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "GlobalRandomGenerator.hpp"


const uint32_t House::TOTAL_TYPES = 16;


House::House() {
	this->type = 1;
}
House::House(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {

	this->type = GlobalRandomGenerator::get()->gen() % TOTAL_TYPES + 1;

	this->addSpec(new HouseSpec());
}
Building* House::createSameTypeBuilding() const {
	return new House(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t House::getSX() const {
	return Parameters::get()->getInt("house_sx");
}
uint32_t House::getSY() const {
	return Parameters::get()->getInt("house_sy");
}
uint32_t House::getMaxHP() const {
	return Parameters::get()->getInt("house_max_hp");
}
Defence House::getDefence() const {
	return Parameters::get()->getDefence("house_defence");
}
Resources House::getCost() const {
	return Parameters::get()->getResources("house_cost");
}
std::string House::getTextureName() const {
	return "house" + std::to_string(this->type);
}
std::string House::getSoundName() const {
	return "bottles";
}
std::wstring House::getDescription() const {
	return *Locales::get()->get("house_description");
}
uint32_t House::getRegenerationSpeed() const {
	return Parameters::get()->getInt("house_regeneration_speed");
}