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


#include "Princess.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Princess::Princess() = default;
Princess::Princess(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorLeader(x, y, playerId) {

}
std::shared_ptr<Warrior> Princess::cloneWarrior() const {
    return std::make_shared<Princess>(*this);
}
UUID Princess::getTypeUUID() const {
    return UUIDs::get().get("princess");
}
std::string Princess::getBeenHitSoundName() const {
    return "ouch_woman_cute2";
}
std::string Princess::getStartInspireSoundName() const {
    return "princess";
}
uint32_t Princess::getMaxHP() const {
    return Parameters::get().getInt("princess_max_hp");
}
Defence Princess::getBaseDefence() const {
    return Parameters::get().getDefence("princess_defence");
}
uint32_t Princess::getInspireRadius() const {
    return Parameters::get().getInt("princess_inspire_radius");
}
Resources Princess::getCost() const {
    return Parameters::get().getResources("princess_cost");
}
uint32_t Princess::getTimeToProduce() const {
    return Parameters::get().getInt("princess_time_to_produce");
}
std::string Princess::getSoundName() const {
    return "princess";
}
StringLcl Princess::getDescription() const {
    return StringLcl("{princess_description}");
}
uint32_t Princess::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t Princess::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t Princess::getBeenHitAnimationsNumberInSet() const {
    return 9;
}
uint32_t Princess::getTippingOverAnimationsNumberInSet() const {
    return 11;
}
uint32_t Princess::getInspireAnimationsNumberInSet() const {
    return 9;
}
std::string Princess::getBaseTextureName() const {
    return "princess";
}
uint32_t Princess::getMovementPoints() const {
    return Parameters::get().getInt("princess_movement_points");
}
uint32_t Princess::getPopulation() const {
    return Parameters::get().getInt("princess_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Princess)