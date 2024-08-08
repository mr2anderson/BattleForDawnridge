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


#include "Tower1.hpp"
#include "Tower1Spec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Tower1::Tower1() = default;
Tower1::Tower1(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new Tower1Spec());
}
Building* Tower1::createSameTypeBuilding() const {
    return new Tower1(this->getX(), this->getY(), this->getPlayerId());
}
UUID Tower1::getTypeUUID() const {
    return UUIDs::get()->get("tower1");
}
uint32_t Tower1::getSX() const {
    return Parameters::get()->getInt("tower1_sx");
}
uint32_t Tower1::getSY() const {
    return Parameters::get()->getInt("tower1_sy");
}
uint32_t Tower1::getMaxHP() const {
    return Parameters::get()->getInt("tower1_max_hp");
}
Defence Tower1::getDefence() const {
    return Parameters::get()->getDefence("tower1_defence");
}
Resources Tower1::getCost() const {
    return Parameters::get()->getResources("tower1_cost");
}
std::string Tower1::getTextureName() const {
    return "tower1";
}
std::string Tower1::getSoundName() const {
    return "big_arrow1";
}
StringLcl Tower1::getDescription() const {
    return StringLcl("{tower1_description}");
}
uint32_t Tower1::getRegenerationSpeed() const {
    return Parameters::get()->getInt("tower1_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Tower1)