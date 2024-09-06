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


#include "Range.hpp"
#include "RangeWarriorProducerSpec.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Range::Range() = default;
Range::Range(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(std::make_shared<RangeWarriorProducerSpec>());
}
std::shared_ptr<Building> Range::createSameTypeBuilding() const {
    return std::make_shared<Range>(this->getX(), this->getY(), this->getPlayerId());
}
UUID Range::getTypeUUID() const {
    return UUIDs::get().get("range");
}
uint32_t Range::getSX() const {
    return Parameters::get().getInt("range_sx");
}
uint32_t Range::getSY() const {
    return Parameters::get().getInt("range_sy");
}
uint32_t Range::getMaxHP() const {
    return Parameters::get().getInt("range_max_hp");
}
Defence Range::getDefence() const {
    return Parameters::get().getDefence("range_defence");
}
Resources Range::getCost() const {
    return Parameters::get().getResources("range_cost");
}
std::string Range::getTextureName() const {
    return "range";
}
std::string Range::getSoundName() const {
    return "bow";
}
StringLcl Range::getDescription() const {
    return StringLcl("{range_description}");
}
uint32_t Range::getRegenerationSpeed() const {
    return Parameters::get().getInt("range_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Range)