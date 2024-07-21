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
#include "SawmillSpec.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


Sawmill::Sawmill() = default;
Sawmill::Sawmill(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new SawmillSpec());
}
Building* Sawmill::cloneBuilding() const {
	return new Sawmill(*this);
}
Defence Sawmill::getDefence() const {
	return Balance::get()->getDefence("sawmill_defence");
}
Resources Sawmill::getCost() const {
	return Balance::get()->getResources("sawmill_cost");
}
uint32_t Sawmill::getSX() const {
	return Balance::get()->getInt("sawmill_sx");
}
uint32_t Sawmill::getSY() const {
	return Balance::get()->getInt("sawmill_sy");
}
uint32_t Sawmill::getMaxHP() const {
	return Balance::get()->getInt("sawmill_max_hp");
}
uint32_t Sawmill::getRegenerationSpeed() const {
	return Balance::get()->getInt("sawmill_regeneration_speed");
}
std::string Sawmill::getTextureName() const {
	return "sawmill";
}
std::string Sawmill::getSoundName() const {
	return "wood";
}
std::wstring Sawmill::getDescription() const {
	return *Texts::get()->get("sawmill_description");
}
std::wstring Sawmill::getUpperCaseReadableName() const {
	return *Texts::get()->get("sawmill_upper_case_readable_name");
}