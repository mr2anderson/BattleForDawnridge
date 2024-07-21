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
#include "Texts.hpp"
#include "Balance.hpp"


WarehouseFood::WarehouseFood() = default;
WarehouseFood::WarehouseFood(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WarehouseFoodSpec());
}
Building* WarehouseFood::cloneBuilding() const {
	return new WarehouseFood(*this);
}
uint32_t WarehouseFood::getSX() const {
	return Balance::get()->getInt("warehouse_food_sx");
}
uint32_t WarehouseFood::getSY() const {
	return Balance::get()->getInt("warehouse_food_sy");
}
uint32_t WarehouseFood::getMaxHP() const {
	return Balance::get()->getInt("warehouse_food_max_hp");
}
Defence WarehouseFood::getDefence() const {
	return Balance::get()->getDefence("warehouse_food_defence");
}
Resources WarehouseFood::getCost() const {
	return Balance::get()->getResources("warehouse_food_cost");
}
uint32_t WarehouseFood::getRegenerationSpeed() const {
	return Balance::get()->getInt("warehouse_food_regeneration_speed");
}
std::string WarehouseFood::getTextureName() const {
	return "warehouse_food";
}
std::string WarehouseFood::getSoundName() const {
	return "food";
}
std::wstring WarehouseFood::getDescription() const {
	return *Texts::get()->get("warehouse_food_description");
}
