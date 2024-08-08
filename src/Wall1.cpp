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


#include "Wall1.hpp"
#include "WallSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Wall1::Wall1() = default;
Wall1::Wall1(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WallSpec());
}
Building* Wall1::createSameTypeBuilding() const {
	return new Wall1(this->getX(), this->getY(), this->getPlayerId());
}
UUID Wall1::getTypeUUID() const {
	return UUIDs::get()->get("wall1");
}
uint32_t Wall1::getSX() const {
    return Parameters::get()->getInt("wall1_sx");
}
uint32_t Wall1::getSY() const {
	return Parameters::get()->getInt("wall1_sy");
}
uint32_t Wall1::getMaxHP() const {
	return Parameters::get()->getInt("wall1_max_hp");
}
Defence Wall1::getDefence() const {
	return Parameters::get()->getDefence("wall1_defence");
}
Resources Wall1::getCost() const {
	return Parameters::get()->getResources("wall1_cost");
}
uint32_t Wall1::getRegenerationSpeed() const {
	return Parameters::get()->getInt("wall1_regeneration_speed");
}
std::string Wall1::getTextureName() const {
	return "wall1";
}
std::string Wall1::getSoundName() const {
	return "stone";
}
StringLcl Wall1::getDescription() const {
	return StringLcl("{wall1_description}");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Wall1)