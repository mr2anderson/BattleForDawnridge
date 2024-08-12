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
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Wall2::Wall2() = default;
Wall2::Wall2(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WallSpec());
}
Building* Wall2::createSameTypeBuilding() const {
	return new Wall2(this->getX(), this->getY(), this->getPlayerId());
}
UUID Wall2::getTypeUUID() const {
	return UUIDs::get()->get("wall2");
}
uint32_t Wall2::getSX() const {
	return Parameters::get()->getInt("wall2_sx");
}
uint32_t Wall2::getSY() const {
	return Parameters::get()->getInt("wall2_sy");
}
uint32_t Wall2::getMaxHP() const {
	return Parameters::get()->getInt("wall2_max_hp");
}
Defence Wall2::getDefence() const {
	return Parameters::get()->getDefence("wall2_defence");
}
Resources Wall2::getCost() const {
	return Parameters::get()->getResources("wall2_cost");
}
uint32_t Wall2::getRegenerationSpeed() const {
	return Parameters::get()->getInt("wall2_regeneration_speed");
}
std::string Wall2::getTextureName() const {
	return "wall2";
}
std::string Wall2::getSoundName() const {
	return "stone";
}
StringLcl Wall2::getDescription() const {
	return StringLcl("{wall2_description}");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Wall2)