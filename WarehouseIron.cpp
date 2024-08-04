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


#include "WarehouseIron.hpp"
#include "WarehouseIronSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"


WarehouseIron::WarehouseIron() = default;
WarehouseIron::WarehouseIron(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y,playerId) {
	this->addSpec(new WarehouseIronSpec());
}
Building* WarehouseIron::createSameTypeBuilding() const {
	return new WarehouseIron(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t WarehouseIron::getSX() const {
	return Parameters::get()->getInt("warehouse_iron_sx");
}
uint32_t WarehouseIron::getSY() const {
	return Parameters::get()->getInt("warehouse_iron_sy");
}
uint32_t WarehouseIron::getMaxHP() const {
	return Parameters::get()->getInt("warehouse_iron_max_hp");
}
Defence WarehouseIron::getDefence() const {
	return Parameters::get()->getDefence("warehouse_iron_defence");
}
Resources WarehouseIron::getCost() const {
	return Parameters::get()->getResources("warehouse_iron_cost");
}
uint32_t WarehouseIron::getRegenerationSpeed() const {
	return Parameters::get()->getInt("warehouse_iron_regeneration_speed");
}
std::string WarehouseIron::getTextureName() const {
	return "warehouse_iron";
}
std::string WarehouseIron::getSoundName() const {
	return "iron";
}
std::wstring WarehouseIron::getDescription() const {
	return *Locales::get()->get("warehouse_iron_description");
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarehouseIron)