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
#include "Texts.hpp"


WarehouseIron::WarehouseIron() = default;
WarehouseIron::WarehouseIron(uint32_t x, uint32_t y, uint32_t playerId) :
	Building(x, y,playerId) {
	this->addSpec(new WarehouseIronSpec());
}
Building* WarehouseIron::cloneBuilding() const {
	return new WarehouseIron(*this);
}
uint32_t WarehouseIron::getSX() const {
    return 2;
}
uint32_t WarehouseIron::getSY() const {
    return 2;
}
uint32_t WarehouseIron::getMaxHP() const {
    return 10000;
}
Defence WarehouseIron::getDefence() const {
	return Defence::WOOD;
}
Resources WarehouseIron::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 10000));
	return cost;
}
uint32_t WarehouseIron::getRegenerationSpeed() const {
	return this->getMaxHP() / 5;
}
std::string WarehouseIron::getTextureName() const {
	return "warehouse_iron";
}
std::string WarehouseIron::getSoundName() const {
	return "iron";
}
std::wstring WarehouseIron::getDescription() const {
	return *Texts::get()->get("warehouse_iron_description");
}
std::wstring WarehouseIron::getUpperCaseReadableName() const {
	return *Texts::get()->get("warehouse_iron_upper_case_readable_name");
}