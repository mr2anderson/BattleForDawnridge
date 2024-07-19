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


Gates2::Gates2() = default;
Gates2::Gates2(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new GatesSpec());
}
Building* Gates2::cloneBuilding() const {
    return new Gates2(*this);
}
uint32_t Gates2::getSX() const {
    return 1;
}
uint32_t Gates2::getSY() const {
    return 1;
}
uint32_t Gates2::getMaxHP() const {
    return 15000;
}
Defence Gates2::getDefence() const {
    return Defence::STONE;
}
Resources Gates2::getCost() const {
    Resources cost;
    cost.plus(Resource("stone", 30000));
    return cost;
}
uint32_t Gates2::getRegenerationSpeed() const {
    return this->getMaxHP() / 6;
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
std::wstring Gates2::getUpperCaseReadableName() const {
    return *Texts::get()->get("gates2_upper_case_readable_name");
}