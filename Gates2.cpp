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


#include "Gates2.hpp"
#include "GatesSpec.hpp"
#include "Texts.hpp"
#include "Balance.hpp"


Gates2::Gates2() = default;
Gates2::Gates2(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new GatesSpec());
}
Building* Gates2::cloneBuilding() const {
    return new Gates2(*this);
}
uint32_t Gates2::getSX() const {
    return Balance::get()->getInt("gates2_sx");
}
uint32_t Gates2::getSY() const {
    return Balance::get()->getInt("gates2_sy");
}
uint32_t Gates2::getMaxHP() const {
    return Balance::get()->getInt("gates2_max_hp");
}
Defence Gates2::getDefence() const {
    return Balance::get()->getDefence("gates2_defence");
}
Resources Gates2::getCost() const {
    return Balance::get()->getResources("gates2_cost");
}
uint32_t Gates2::getRegenerationSpeed() const {
    return Balance::get()->getInt("gates2_regeneration_speed");
}
std::string Gates2::getTextureName() const {
    return "gates2";
}
std::string Gates2::getSoundName() const {
    return "stone";
}
std::wstring Gates2::getDescription() const {
    return *Texts::get()->get("gates2_description");
}
