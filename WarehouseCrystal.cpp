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


#include "WarehouseCrystal.hpp"
#include "WarehouseCrystalSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"


WarehouseCrystal::WarehouseCrystal() = default;
WarehouseCrystal::WarehouseCrystal(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new WarehouseCrystalSpec());
}
Building* WarehouseCrystal::createSameTypeBuilding() const {
    return new WarehouseCrystal(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t WarehouseCrystal::getSX() const {
    return Parameters::get()->getInt("warehouse_crystal_sx");
}
uint32_t WarehouseCrystal::getSY() const {
    return Parameters::get()->getInt("warehouse_crystal_sy");
}
uint32_t WarehouseCrystal::getMaxHP() const {
    return Parameters::get()->getInt("warehouse_crystal_max_hp");
}
Defence WarehouseCrystal::getDefence() const {
    return Parameters::get()->getDefence("warehouse_crystal_defence");
}
Resources WarehouseCrystal::getCost() const {
    return Parameters::get()->getResources("warehouse_crystal_cost");
}
uint32_t WarehouseCrystal::getRegenerationSpeed() const {
    return Parameters::get()->getInt("warehouse_crystal_regeneration_speed");
}
std::string WarehouseCrystal::getTextureName() const {
    return "warehouse_crystal";
}
std::string WarehouseCrystal::getSoundName() const {
    return "crystal";
}
std::wstring WarehouseCrystal::getDescription() const {
    return *Locales::get()->get("warehouse_crystal_description");
}