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


#include "WarehouseStone.hpp"
#include "WarehouseStoneSpec.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


WarehouseStone::WarehouseStone() = default;
WarehouseStone::WarehouseStone(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WarehouseStoneSpec());
}
Building* WarehouseStone::cloneBuilding() const {
	return new WarehouseStone(*this);
}
uint32_t WarehouseStone::getSX() const {
	return Balance::get()->getInt("warehouse_stone_sx");
}
uint32_t WarehouseStone::getSY() const {
	return Balance::get()->getInt("warehouse_stone_sy");
}
uint32_t WarehouseStone::getMaxHP() const {
	return Balance::get()->getInt("warehouse_stone_max_hp");
}
Defence WarehouseStone::getDefence() const {
	return Balance::get()->getDefence("warehouse_stone_defence");
}
Resources WarehouseStone::getCost() const {
	return Balance::get()->getResources("warehouse_stone_cost");
}
uint32_t WarehouseStone::getRegenerationSpeed() const {
	return Balance::get()->getInt("warehouse_stone_regeneration_speed");
}
std::string WarehouseStone::getTextureName() const {
	return "warehouse_stone";
}
std::string WarehouseStone::getSoundName() const {
	return "stone";
}
std::wstring WarehouseStone::getDescription() const {
	return *Texts::get()->get("warehouse_stone_description");
}
std::wstring WarehouseStone::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_stone_upper_case_readable_name");
}