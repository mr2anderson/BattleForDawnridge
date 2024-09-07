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


#include "Academy.hpp"
#include "AcademySpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Academy::Academy() = default;
Academy::Academy(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(std::make_shared<AcademySpec>());
}
std::shared_ptr<Building> Academy::createSameTypeBuilding() const {
    return std::make_shared<Academy>(this->getX(), this->getY(), this->getPlayerId());
}
UUID Academy::getTypeUUID() const {
    return UUIDs::get().get("academy");
}
uint32_t Academy::getSX() const {
    return Parameters::get().getInt("academy_sx");
}
uint32_t Academy::getSY() const {
    return Parameters::get().getInt("academy_sy");
}
uint32_t Academy::getMaxHP() const {
    return Parameters::get().getInt("academy_max_hp");
}
Defence Academy::getDefence() const {
    return Parameters::get().getDefence("academy_defence");
}
Resources Academy::getCost() const {
    return Parameters::get().getResources("academy_cost");
}
std::string Academy::getTextureName() const {
    return "academy";
}
std::string Academy::getSoundName() const {
    return "academy";
}
StringLcl Academy::getDescription() const {
    return StringLcl("{academy_description}");
}
uint32_t Academy::getRegenerationSpeed() const {
    return Parameters::get().getInt("academy_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Academy)