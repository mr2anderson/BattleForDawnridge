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
#include "Texts.hpp"


Gates1::Gates1() = default;
Gates1::Gates1(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<TerritoryOriginB>> originsPtr) :
        Gates(x, y, playerId, units, originsPtr),
        Building(x, y, playerId, units) {

}
Building* Gates1::cloneBuilding() const {
    return new Gates1(*this);
}
uint32_t Gates1::getSX() const {
    return 1;
}
uint32_t Gates1::getSY() const {
    return 1;
}
uint32_t Gates1::getMaxHP() const {
    return 40000;
}
Resources Gates1::getCost() const {
    Resources cost;
    cost.plus(Resource("wood", 7500));
    return cost;
}
uint32_t Gates1::getRegenerationSpeed() const {
    return 10000;
}
std::string Gates1::getTextureName() const {
    return "gates1";
}
std::string Gates1::getSoundName() const {
    return "stone";
}
std::wstring Gates1::getDescription() const {
    return *Texts::get()->get("gates1_description");
}
std::wstring Gates1::getUpperCaseReadableName() const {
    return *Texts::get()->get("gates1_upper_case_readable_name");
}