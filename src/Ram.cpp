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


#include "Ram.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"


Ram::Ram() = default;
Ram::Ram(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorNearSingleAttacker(x, y, playerId) {

}
UUID Ram::getTypeUUID() const {
    return UUIDs::get().get("ram");
}
std::shared_ptr<Warrior>  Ram::cloneWarrior() const {
    return std::make_shared<Ram>(*this);
}
std::string Ram::getBeenHitSoundName() const {
    return "vehicle_hit";
}
boost::optional<std::string> Ram::getStartAttackSoundName() const {
    return boost::make_optional<std::string>("vehicle");
}
uint32_t Ram::getMaxHP() const {
    return Parameters::get().getInt("ram_max_hp");
}
Defence Ram::getBaseDefence() const {
    return Parameters::get().getDefence("ram_defence");
}
Damage Ram::getBaseDamage() const {
    return Parameters::get().getDamage("ram_damage");
}
Resources Ram::getCost() const {
    return Parameters::get().getResources("ram_cost");
}
uint32_t Ram::getTimeToProduce() const {
    return Parameters::get().getInt("ram_time_to_produce");
}
std::string Ram::getSoundName() const {
    return "vehicle";
}
StringLcl Ram::getDescription() const {
    return StringLcl("{ram_description}");
}
bool Ram::isVehicle() const {
    return true;
}
uint32_t Ram::getTalkingAnimationsNumberInSet() const {
    return 1;
}
uint32_t Ram::getRunningAnimationsNumberInSet() const {
    return 1;
}
uint32_t Ram::getBeenHitAnimationsNumberInSet() const {
    return 1;
}
uint32_t Ram::getTippingOverAnimationsNumberInSet() const {
    return 1;
}
uint32_t Ram::getAttackAnimationsNumberInSet() const {
    return 18;
}
std::string Ram::getBaseTextureName() const {
    return "ram";
}
uint32_t Ram::getMovementPoints() const {
    return Parameters::get().getInt("ram_movement_points");
}
uint32_t Ram::getPopulation() const {
    return Parameters::get().getInt("ram_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Ram)