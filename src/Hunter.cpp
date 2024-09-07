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


#include "Hunter.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"
#include "CreateEEvent.hpp"
#include "BigArrow.hpp"


Hunter::Hunter() = default;
Hunter::Hunter(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorDstSingleAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior> Hunter::cloneWarrior() const {
    return std::make_shared<Hunter>(*this);
}
UUID Hunter::getTypeUUID() const {
    return UUIDs::get().get("hunter");
}
std::string Hunter::getBeenHitSoundName() const {
    return "ouch";
}
std::string Hunter::getStartAttackSoundName() const {
    return "bow";
}
uint32_t Hunter::getMaxHP() const {
    return Parameters::get().getInt("hunter_max_hp");
}
Defence Hunter::getBaseDefence() const {
    return Parameters::get().getDefence("hunter_defence");
}
Damage Hunter::getBaseDamage() const {
    return Parameters::get().getDamage("hunter_damage");
}
Resources Hunter::getCost() const {
    return Parameters::get().getResources("hunter_cost");
}
uint32_t Hunter::getTimeToProduce() const {
    return Parameters::get().getInt("hunter_time_to_produce");
}
std::string Hunter::getSoundName() const {
    return "bow";
}
StringLcl Hunter::getDescription() const {
    return StringLcl("{hunter_description}");
}
uint32_t Hunter::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t Hunter::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t Hunter::getBeenHitAnimationsNumberInSet() const {
    return 9;
}
uint32_t Hunter::getTippingOverAnimationsNumberInSet() const {
    return 11;
}
uint32_t Hunter::getAttackAnimationsNumberInSet() const {
    return 13;
}
std::string Hunter::getBaseTextureName() const {
    return "hunter";
}
uint32_t Hunter::getMovementPoints() const {
    return Parameters::get().getInt("hunter_movement_points");
}
uint32_t Hunter::getPopulation() const {
    return Parameters::get().getInt("hunter_population");
}
uint32_t Hunter::getAttackRadius() const {
    return Parameters::get().getInt("hunter_attack_radius");
}
Events Hunter::eventAfterAnimation(uint32_t targetX, uint32_t targetY) {
    std::shared_ptr<BigArrow> bigArrow = std::make_shared<BigArrow>();
    bigArrow->setSrc(this->getCenterX(), this->getCenterY());
    bigArrow->setDst(64 * targetX + 32, 64 * targetY + 32);

    Events events;
    events.add(std::make_shared<CreateEEvent>(bigArrow));

    return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(Hunter)