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


#include "Killer.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Killer::Killer() = default;
Killer::Killer(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorNearSingleAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior> Killer::cloneWarrior() const {
    return std::make_shared<Killer>(*this);
}
UUID Killer::getTypeUUID() const {
    return UUIDs::get().get("killer");
}
std::string Killer::getBeenHitSoundName() const {
    return "ouch";
}
std::string Killer::getStartAttackSoundName() const {
    return "knife";
}
uint32_t Killer::getMaxHP() const {
    return Parameters::get().getInt("killer_max_hp");
}
Defence Killer::getBaseDefence() const {
    return Parameters::get().getDefence("killer_defence");
}
Damage Killer::getBaseDamage() const {
    return Parameters::get().getDamage("killer_damage");
}
Resources Killer::getCost() const {
    return Parameters::get().getResources("killer_cost");
}
uint32_t Killer::getTimeToProduce() const {
    return Parameters::get().getInt("killer_time_to_produce");
}
std::string Killer::getSoundName() const {
    return "knife";
}
StringLcl Killer::getDescription() const {
    return StringLcl("{killer_description}");
}
uint32_t Killer::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t Killer::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t Killer::getBeenHitAnimationsNumberInSet() const {
    return 9;
}
uint32_t Killer::getTippingOverAnimationsNumberInSet() const {
    return 9;
}
uint32_t Killer::getAttackAnimationsNumberInSet() const {
    return 13;
}
std::string Killer::getBaseTextureName() const {
    return "killer";
}
uint32_t Killer::getMovementPoints() const {
    return Parameters::get().getInt("killer_movement_points");
}
uint32_t Killer::getPopulation() const {
    return Parameters::get().getInt("killer_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Killer)