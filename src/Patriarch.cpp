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


#include "Patriarch.hpp"
#include "PatriarchProjectile.hpp"
#include "Parameters.hpp"
#include "Locales.hpp"
#include "UUIDs.hpp"


Patriarch::Patriarch() = default;
Patriarch::Patriarch(uint32_t x, uint32_t y, uint32_t playerId) : WarriorProjectileHealer(x, y, playerId) {

}
std::shared_ptr<Warrior> Patriarch::cloneWarrior() const {
    return std::make_shared<Patriarch>(*this);
}
UUID Patriarch::getTypeUUID() const {
    return UUIDs::get().get("patriarch");
}
std::string Patriarch::getBeenHitSoundName() const {
    return "ouch_old";
}
uint32_t Patriarch::getMaxHP() const {
    return Parameters::get().getInt("patriarch_max_hp");
}
Defence Patriarch::getBaseDefence() const {
    return Parameters::get().getDefence("patriarch_defence");
}
uint32_t Patriarch::getHealingRadius() const {
    return Parameters::get().getInt("patriarch_healing_radius");
}
Resources Patriarch::getCost() const {
    return Parameters::get().getResources("patriarch_cost");
}
uint32_t Patriarch::getTimeToProduce() const {
    return Parameters::get().getInt("patriarch_time_to_produce");
}
std::string Patriarch::getSoundName() const {
    return "old";
}
StringLcl Patriarch::getDescription() const {
    return StringLcl("{patriarch_description}");
}
uint32_t Patriarch::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t Patriarch::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t Patriarch::getBeenHitAnimationsNumberInSet() const {
    return 9;
}
uint32_t Patriarch::getTippingOverAnimationsNumberInSet() const {
    return 13;
}
std::shared_ptr<Projectile> Patriarch::getProjectile() const {
    return std::make_shared<PatriarchProjectile>();
}
std::string Patriarch::getBaseTextureName() const {
    return "patriarch";
}
uint32_t Patriarch::getMovementPoints() const {
    return Parameters::get().getInt("patriarch_movement_points");
}
uint32_t Patriarch::getPopulation() const {
    return Parameters::get().getInt("patriarch_population");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Patriarch)