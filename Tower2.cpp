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


#include "Tower2.hpp"
#include "Tower2Spec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"


Tower2::Tower2() = default;
Tower2::Tower2(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new Tower2Spec());
}
Building* Tower2::createSameTypeBuilding() const {
    return new Tower2(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t Tower2::getSX() const {
    return Parameters::get()->getInt("tower2_sx");
}
uint32_t Tower2::getSY() const {
    return Parameters::get()->getInt("tower2_sy");
}
uint32_t Tower2::getMaxHP() const {
    return Parameters::get()->getInt("tower2_max_hp");
}
Defence Tower2::getDefence() const {
    return Parameters::get()->getDefence("tower2_defence");
}
Resources Tower2::getCost() const {
    return Parameters::get()->getResources("tower2_cost");
}
std::string Tower2::getTextureName() const {
    return "tower2";
}
std::string Tower2::getSoundName() const {
    return "big_arrow1";
}
std::wstring Tower2::getDescription() const {
    return *Locales::get()->get("tower2_description");
}
uint32_t Tower2::getRegenerationSpeed() const {
    return Parameters::get()->getInt("tower2_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Tower2)