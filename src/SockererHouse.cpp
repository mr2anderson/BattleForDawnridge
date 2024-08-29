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


#include "SockererHouse.hpp"
#include "SockererHouseSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


SockererHouse::SockererHouse() = default;
SockererHouse::SockererHouse(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(std::make_shared<SockererHouseSpec>());
}
std::shared_ptr<Building> SockererHouse::createSameTypeBuilding() const {
	return std::make_shared<SockererHouse>(this->getX(), this->getY(), this->getPlayerId());
}
UUID SockererHouse::getTypeUUID() const {
	return UUIDs::get().get("sockerer_house");
}
uint32_t SockererHouse::getSX() const {
	return Parameters::get().getInt("sockerer_house_sx");
}
uint32_t SockererHouse::getSY() const {
	return Parameters::get().getInt("sockerer_house_sy");
}
uint32_t SockererHouse::getMaxHP() const {
	return Parameters::get().getInt("sockerer_house_max_hp");
}
Defence SockererHouse::getDefence() const {
	return Parameters::get().getDefence("sockerer_house_defence");
}
Resources SockererHouse::getCost() const {
	return Parameters::get().getResources("sockerer_house_cost");
}
std::string SockererHouse::getTextureName() const {
	return "sockerer_house";
}
std::string SockererHouse::getSoundName() const {
	return "sockerer_house";
}
StringLcl SockererHouse::getDescription() const {
	return StringLcl("{sockerer_house_description}");
}
uint32_t SockererHouse::getRegenerationSpeed() const {
	return Parameters::get().getInt("sockerer_house_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(SockererHouse)