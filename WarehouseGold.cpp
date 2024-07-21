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


#include "WarehouseGold.hpp"
#include "WarehouseGoldWarehouseSpec.hpp"
#include "WarehouseGoldCollectorSpec.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


WarehouseGold::WarehouseGold() = default;
WarehouseGold::WarehouseGold(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WarehouseGoldWarehouseSpec());
	this->addSpec(new WarehouseGoldCollectorSpec());
}
Building* WarehouseGold::cloneBuilding() const {
	return new WarehouseGold(*this);
}
uint32_t WarehouseGold::getSX() const {
	return Balance::get()->getInt("warehouse_gold_sx");
}
uint32_t WarehouseGold::getSY() const {
	return Balance::get()->getInt("warehouse_gold_sy");
}
uint32_t WarehouseGold::getMaxHP() const {
	return Balance::get()->getInt("warehouse_gold_max_hp");
}
Defence WarehouseGold::getDefence() const {
	return Balance::get()->getDefence("warehouse_gold_defence");
}
Resources WarehouseGold::getCost() const {
	return Balance::get()->getResources("warehouse_gold_cost");
}
uint32_t WarehouseGold::getRegenerationSpeed() const {
	return Balance::get()->getInt("warehouse_gold_regeneration_speed");
}
std::string WarehouseGold::getTextureName() const {
	return "warehouse_gold";
}
std::string WarehouseGold::getSoundName() const {
	return "gold";
}
std::wstring WarehouseGold::getDescription() const {
	return *Texts::get()->get("warehouse_gold_description");
}
