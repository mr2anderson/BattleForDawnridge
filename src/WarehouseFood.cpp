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


#include "WarehouseFood.hpp"
#include "WarehouseFoodSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


WarehouseFood::WarehouseFood() = default;
WarehouseFood::WarehouseFood(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(std::make_shared<WarehouseFoodSpec>());
}
std::shared_ptr<Building>  WarehouseFood::createSameTypeBuilding() const {
	return std::make_shared<WarehouseFood>(this->getX(), this->getY(), this->getPlayerId());
}
UUID WarehouseFood::getTypeUUID() const {
	return UUIDs::get().get("warehouse_food");
}
uint32_t WarehouseFood::getSX() const {
	return Parameters::get().getInt("warehouse_food_sx");
}
uint32_t WarehouseFood::getSY() const {
	return Parameters::get().getInt("warehouse_food_sy");
}
uint32_t WarehouseFood::getMaxHP() const {
	return Parameters::get().getInt("warehouse_food_max_hp");
}
Defence WarehouseFood::getDefence() const {
	return Parameters::get().getDefence("warehouse_food_defence");
}
Resources WarehouseFood::getCost() const {
	return Parameters::get().getResources("warehouse_food_cost");
}
uint32_t WarehouseFood::getRegenerationSpeed() const {
	return Parameters::get().getInt("warehouse_food_regeneration_speed");
}
std::string WarehouseFood::getTextureName() const {
	return "warehouse_food";
}
std::string WarehouseFood::getSoundName() const {
	return "food";
}
StringLcl WarehouseFood::getDescription() const {
	return StringLcl("{warehouse_food_description}");
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarehouseFood)