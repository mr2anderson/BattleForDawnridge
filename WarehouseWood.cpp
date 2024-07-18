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
#include "Texts.hpp"


WarehouseWood::WarehouseWood() = default;
WarehouseWood::WarehouseWood(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WarehouseWoodSpec());
}
Building* WarehouseWood::cloneBuilding() const {
	return new WarehouseWood(*this);
}
uint32_t WarehouseWood::getSX() const {
    return 2;
}
uint32_t WarehouseWood::getSY() const {
    return 2;
}
uint32_t WarehouseWood::getMaxHP() const {
    return 10000;
}
Defence WarehouseWood::getDefence() const {
	return Defence::WOOD;
}
Resources WarehouseWood::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 10000));
	return cost;
}
uint32_t WarehouseWood::getRegenerationSpeed() const {
	return this->getMaxHP() / 5;
}
std::string WarehouseWood::getTextureName() const {
	return "warehouse_wood";
}
std::string WarehouseWood::getSoundName() const {
	return "wood";
}
std::wstring WarehouseWood::getDescription() const {
	return *Texts::get()->get("warehouse_wood_description");
}
std::wstring WarehouseWood::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_wood_upper_case_readable_name");
}