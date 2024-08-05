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


#include "Gates1.hpp"
#include "GatesSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Gates1::Gates1() = default;
Gates1::Gates1(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new GatesSpec());
}
Building* Gates1::createSameTypeBuilding() const {
    return new Gates1(this->getX(), this->getY(), this->getPlayerId());
}
UUID Gates1::getTypeUUID() const {
    return UUIDs::get()->get("gates1");
}
uint32_t Gates1::getSX() const {
    return Parameters::get()->getInt("gates1_sx");
}
uint32_t Gates1::getSY() const {
    return Parameters::get()->getInt("gates1_sy");
}
uint32_t Gates1::getMaxHP() const {
    return Parameters::get()->getInt("gates1_max_hp");
}
Defence Gates1::getDefence() const {
    return Parameters::get()->getDefence("gates1_defence");
}
Resources Gates1::getCost() const {
    return Parameters::get()->getResources("gates1_cost");
}
uint32_t Gates1::getRegenerationSpeed() const {
    return Parameters::get()->getInt("gates1_regeneration_speed");
}
std::string Gates1::getTextureName() const {
    return "gates1";
}
std::string Gates1::getSoundName() const {
    return "stone";
}
std::wstring Gates1::getDescription() const {
    return *Locales::get()->get("gates1_description");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Gates1)