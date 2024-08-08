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


#include "WarehouseWood.hpp"
#include "WarehouseWoodSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


WarehouseWood::WarehouseWood() = default;
WarehouseWood::WarehouseWood(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WarehouseWoodSpec());
}
Building* WarehouseWood::createSameTypeBuilding() const {
	return new WarehouseWood(this->getX(), this->getY(), this->getPlayerId());
}
UUID WarehouseWood::getTypeUUID() const {
	return UUIDs::get()->get("warehouse_wood");
}
uint32_t WarehouseWood::getSX() const {
	return Parameters::get()->getInt("warehouse_wood_sx");
}
uint32_t WarehouseWood::getSY() const {
	return Parameters::get()->getInt("warehouse_wood_sy");
}
uint32_t WarehouseWood::getMaxHP() const {
	return Parameters::get()->getInt("warehouse_wood_max_hp");
}
Defence WarehouseWood::getDefence() const {
	return Parameters::get()->getDefence("warehouse_wood_defence");
}
Resources WarehouseWood::getCost() const {
	return Parameters::get()->getResources("warehouse_wood_cost");
}
uint32_t WarehouseWood::getRegenerationSpeed() const {
	return Parameters::get()->getInt("warehouse_wood_regeneration_speed");
}
std::string WarehouseWood::getTextureName() const {
	return "warehouse_wood";
}
std::string WarehouseWood::getSoundName() const {
	return "wood";
}
StringLcl WarehouseWood::getDescription() const {
	return StringLcl("{warehouse_wood_description}");
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarehouseWood)