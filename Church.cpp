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


#include "Church.hpp"
#include "ChurchSpec.hpp"
#include "Parameters.hpp"
#include "Locales.hpp"


Church::Church() = default;
Church::Church(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new ChurchSpec());
}
Building* Church::createSameTypeBuilding() const {
    return new Church(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t Church::getSX() const {
    return Parameters::get()->getInt("church_sx");
}
uint32_t Church::getSY() const {
    return Parameters::get()->getInt("church_sy");
}
uint32_t Church::getMaxHP() const {
    return Parameters::get()->getInt("church_max_hp");
}
Defence Church::getDefence() const {
    return Parameters::get()->getDefence("church_defence");
}
Resources Church::getCost() const {
    return Parameters::get()->getResources("church_cost");
}
std::string Church::getTextureName() const {
    return "church";
}
std::string Church::getSoundName() const {
    return "church";
}
std::wstring Church::getDescription() const {
    return *Locales::get()->get("church_description");
}
uint32_t Church::getRegenerationSpeed() const {
    return Parameters::get()->getInt("church_regeneration_speed");
}