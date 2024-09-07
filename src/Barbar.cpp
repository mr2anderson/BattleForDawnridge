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


#include "Barbar.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Barbar::Barbar() = default;
Barbar::Barbar(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorNearSingleAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior> Barbar::cloneWarrior() const {
    return std::make_shared<Barbar>(*this);
}
UUID Barbar::getTypeUUID() const {
    return UUIDs::get().get("barbar");
}
std::string Barbar::getBeenHitSoundName() const {
    return "barbar";
}
boost::optional<std::string> Barbar::getStartAttackSoundName() const {
    return boost::make_optional<std::string>("swing");
}
uint32_t Barbar::getMaxHP() const {
    return Parameters::get().getInt("barbar_max_hp");
}
Defence Barbar::getBaseDefence() const {
    return Parameters::get().getDefence("barbar_defence");
}
Damage Barbar::getBaseDamage() const {
    return Parameters::get().getDamage("barbar_damage");
}
Resources Barbar::getCost() const {
    return Parameters::get().getResources("barbar_cost");
}
uint32_t Barbar::getTimeToProduce() const {
    return Parameters::get().getInt("barbar_time_to_produce");
}
std::string Barbar::getSoundName() const {
    return "barbar";
}
StringLcl Barbar::getDescription() const {
    return StringLcl("{barbar_description}");
}
uint32_t Barbar::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t Barbar::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t Barbar::getBeenHitAnimationsNumberInSet() const {
    return 8;
}
uint32_t Barbar::getTippingOverAnimationsNumberInSet() const {
    return 11;
}
uint32_t Barbar::getAttackAnimationsNumberInSet() const {
    return 11;
}
std::string Barbar::getBaseTextureName() const {
    return "barbar";
}
uint32_t Barbar::getMovementPoints() const {
    return Parameters::get().getInt("barbar_movement_points");
}
uint32_t Barbar::getPopulation() const {
    return Parameters::get().getInt("barbar_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Barbar)