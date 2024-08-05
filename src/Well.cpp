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



#include "Well.hpp"
#include "WellSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "UUIDs.hpp"


const uint32_t Well::TOTAL_TYPES = 8;


Well::Well() {
    this->type = 1;
}
Well::Well(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {

    this->type = GlobalRandomGenerator32::get()->gen() % TOTAL_TYPES + 1;

    this->addSpec(new WellSpec());
}
Building* Well::createSameTypeBuilding() const {
    return new Well(this->getX(), this->getY(), this->getPlayerId());
}
UUID Well::getTypeUUID() const {
    return UUIDs::get()->get("well");
}
Defence Well::getDefence() const {
    return Parameters::get()->getDefence("well_defence");
}
Resources Well::getCost() const {
    return Parameters::get()->getResources("well_cost");
}
uint32_t Well::getSX() const {
    return Parameters::get()->getInt("well_sx");
}
uint32_t Well::getSY() const {
    return Parameters::get()->getInt("well_sy");
}
uint32_t Well::getMaxHP() const {
    return Parameters::get()->getInt("well_max_hp");
}
uint32_t Well::getRegenerationSpeed() const {
    return Parameters::get()->getInt("well_regeneration_speed");
}
std::string Well::getTextureName() const {
    return "well" + std::to_string(this->type);
}
std::string Well::getSoundName() const {
    return "crystal";
}
std::wstring Well::getDescription() const {
    return *Locales::get()->get("well_description");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Well)