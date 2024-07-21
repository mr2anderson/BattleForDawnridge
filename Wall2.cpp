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


#include "Wall2.hpp"
#include "WallSpec.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


Wall2::Wall2() = default;
Wall2::Wall2(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WallSpec());
}
Building* Wall2::cloneBuilding() const {
	return new Wall2(*this);
}
uint32_t Wall2::getSX() const {
	return Balance::get()->getInt("wall2_sx");
}
uint32_t Wall2::getSY() const {
	return Balance::get()->getInt("wall2_sy");
}
uint32_t Wall2::getMaxHP() const {
	return Balance::get()->getInt("wall2_max_hp");
}
Defence Wall2::getDefence() const {
	return Balance::get()->getDefence("wall2_defence");
}
Resources Wall2::getCost() const {
	return Balance::get()->getResources("wall2_cost");
}
uint32_t Wall2::getRegenerationSpeed() const {
	return Balance::get()->getInt("wall2_regeneration_speed");
}
std::string Wall2::getTextureName() const {
	return "wall2";
}
std::string Wall2::getSoundName() const {
	return "stone";
}
std::wstring Wall2::getDescription() const {
	return *Texts::get()->get("wall2_description");
}
std::wstring Wall2::getUpperCaseReadableName() const {
	return *Texts::get()->get("wall2_upper_case_readable_name");
}