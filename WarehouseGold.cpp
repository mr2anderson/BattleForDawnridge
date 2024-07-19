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
    return 2;
}
uint32_t WarehouseGold::getSY() const {
    return 2;
}
uint32_t WarehouseGold::getMaxHP() const {
    return 10000;
}
Defence WarehouseGold::getDefence() const {
	return Defence::WOOD;
}
Resources WarehouseGold::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 10000));
	return cost;
}
uint32_t WarehouseGold::getRegenerationSpeed() const {
	return this->getMaxHP() / 4;
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
std::wstring WarehouseGold::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_gold_upper_case_readable_name");
}