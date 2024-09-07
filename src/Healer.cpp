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


#include "Healer.hpp"
#include "HealerProjectile.hpp"
#include "Parameters.hpp"
#include "Locales.hpp"
#include "UUIDs.hpp"


Healer::Healer() = default;
Healer::Healer(uint32_t x, uint32_t y, uint32_t playerId) : WarriorHealer(x, y, playerId) {

}
std::shared_ptr<Warrior> Healer::cloneWarrior() const {
    return std::make_shared<Healer>(*this);
}
UUID Healer::getTypeUUID() const {
    return UUIDs::get().get("healer");
}
std::string Healer::getBeenHitSoundName() const {
    return "ouch_woman_cute";
}
uint32_t Healer::getMaxHP() const {
    return Parameters::get().getInt("healer_max_hp");
}
Defence Healer::getBaseDefence() const {
    return Parameters::get().getDefence("healer_defence");
}
uint32_t Healer::getHealingRadius() const {
    return Parameters::get().getInt("healer_healing_radius");
}
Resources Healer::getCost() const {
    return Parameters::get().getResources("healer_cost");
}
uint32_t Healer::getTimeToProduce() const {
    return Parameters::get().getInt("healer_time_to_produce");
}
std::string Healer::getSoundName() const {
    return "healer";
}
StringLcl Healer::getDescription() const {
    return StringLcl("{healer_description}");
}
bool Healer::isFemale() const {
    return true;
}
bool Healer::isFlying() const {
    return true;
}
bool Healer::delayBetweenTalkingAnimations() const {
    return false;
}
uint32_t Healer::getTalkingAnimationsNumberInSet() const {
    return 16;
}
uint32_t Healer::getRunningAnimationsNumberInSet() const {
    return 16;
}
uint32_t Healer::getBeenHitAnimationsNumberInSet() const {
    return 16;
}
uint32_t Healer::getTippingOverAnimationsNumberInSet() const {
    return 16;
}
std::shared_ptr<Projectile> Healer::getProjectile() const {
    return std::make_shared<HealerProjectile>();
}
std::string Healer::getBaseTextureName() const {
    return "healer";
}
uint32_t Healer::getMovementPoints() const {
    return Parameters::get().getInt("healer_movement_points");
}
uint32_t Healer::getPopulation() const {
    return Parameters::get().getInt("healer_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Healer)