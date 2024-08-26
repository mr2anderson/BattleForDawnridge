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


#include "Valkyr.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Valkyr::Valkyr() = default;
Valkyr::Valkyr(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorNearMultyAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior>  Valkyr::cloneWarrior() const {
    return std::make_shared<Valkyr>(*this);
}
UUID Valkyr::getTypeUUID() const {
    return UUIDs::get().get("valkyr");
}
std::string Valkyr::getBeenHitSoundName() const {
    return "ouch_woman";
}
std::string Valkyr::getStartAttackSoundName() const {
    return "swing";
}
uint32_t Valkyr::getMaxHP() const {
    return Parameters::get().getInt("valkyr_max_hp");
}
Defence Valkyr::getBaseDefence() const {
    return Parameters::get().getDefence("valkyr_defence");
}
Damage Valkyr::getBaseDamage() const {
    return Parameters::get().getDamage("valkyr_damage");
}
Resources Valkyr::getCost() const {
    return Parameters::get().getResources("valkyr_cost");
}
uint32_t Valkyr::getTimeToProduce() const {
    return Parameters::get().getInt("valkyr_time_to_produce");
}
std::string Valkyr::getSoundName() const {
    return "valkyr";
}
StringLcl Valkyr::getDescription() const {
    return StringLcl("{valkyr_description}");
}
uint32_t Valkyr::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t Valkyr::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t Valkyr::getBeenHitAnimationsNumberInSet() const {
    return 9;
}
uint32_t Valkyr::getTippingOverAnimationsNumberInSet() const {
    return 11;
}
uint32_t Valkyr::getAttackAnimationsNumberInSet() const {
    return 13;
}
std::string Valkyr::getBaseTextureName() const {
    return "valkyr";
}
uint32_t Valkyr::getMovementPoints() const {
    return Parameters::get().getInt("valkyr_movement_points");
}
uint32_t Valkyr::getPopulation() const {
    return Parameters::get().getInt("valkyr_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Valkyr)