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
#include "MineSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Mine::Mine() = default;
Mine::Mine(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(std::make_shared<MineSpec>());
}
std::shared_ptr<Building>  Mine::createSameTypeBuilding() const {
	return std::make_shared<Mine>(this->getX(), this->getY(), this->getPlayerId());
}
UUID Mine::getTypeUUID() const {
	return UUIDs::get().get("mine");
}
uint32_t Mine::getSX() const {
	return Parameters::get().getInt("mine_sx");
}
uint32_t Mine::getSY() const {
	return Parameters::get().getInt("mine_sy");
}
uint32_t Mine::getMaxHP() const {
	return Parameters::get().getInt("mine_max_hp");
}
Defence Mine::getDefence() const {
	return Parameters::get().getDefence("mine_defence");
}
Resources Mine::getCost() const {
	return Parameters::get().getResources("mine_cost");
}
uint32_t Mine::getRegenerationSpeed() const {
	return Parameters::get().getInt("mine_regeneration_speed");
}
std::string Mine::getTextureName() const {
	return "mine";
}
std::string Mine::getSoundName() const {
	return "iron";
}
StringLcl Mine::getDescription() const {
	return StringLcl("{mine_description}");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Mine)