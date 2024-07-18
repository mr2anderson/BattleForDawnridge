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


WarehouseStone::WarehouseStone() = default;
WarehouseStone::WarehouseStone(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y, playerId) {
	this->addSpec(new WarehouseStoneSpec());
}
Building* WarehouseStone::cloneBuilding() const {
	return new WarehouseStone(*this);
}
uint32_t WarehouseStone::getSX() const {
    return 2;
}
uint32_t WarehouseStone::getSY() const {
    return 2;
}
uint32_t WarehouseStone::getMaxHP() const {
    return 10000;
}
Defence WarehouseStone::getDefence() const {
	return Defence::WOOD;
}
Resources WarehouseStone::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 10000));
	return cost;
}
uint32_t WarehouseStone::getRegenerationSpeed() const {
	return this->getMaxHP() / 5;
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