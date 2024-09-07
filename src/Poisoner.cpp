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


#include "Poisoner.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"
#include "CreateEEvent.hpp"
#include "PoisonerArrow.hpp"
#include "PlaySoundEvent.hpp"


Poisoner::Poisoner() = default;
Poisoner::Poisoner(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorDstSingleAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior> Poisoner::cloneWarrior() const {
    return std::make_shared<Poisoner>(*this);
}
UUID Poisoner::getTypeUUID() const {
    return UUIDs::get().get("poisoner");
}
std::string Poisoner::getBeenHitSoundName() const {
    return "ouch";
}
boost::optional<std::string> Poisoner::getStartAttackSoundName() const {
    return boost::none;
}
uint32_t Poisoner::getMaxHP() const {
    return Parameters::get().getInt("poisoner_max_hp");
}
Defence Poisoner::getBaseDefence() const {
    return Parameters::get().getDefence("poisoner_defence");
}
Damage Poisoner::getBaseDamage() const {
    return Parameters::get().getDamage("poisoner_damage");
}
Resources Poisoner::getCost() const {
    return Parameters::get().getResources("poisoner_cost");
}
uint32_t Poisoner::getTimeToProduce() const {
    return Parameters::get().getInt("poisoner_time_to_produce");
}
std::string Poisoner::getSoundName() const {
    return "knife";
}
StringLcl Poisoner::getDescription() const {
    return StringLcl("{poisoner_description}");
}
uint32_t Poisoner::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t Poisoner::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t Poisoner::getBeenHitAnimationsNumberInSet() const {
    return 9;
}
uint32_t Poisoner::getTippingOverAnimationsNumberInSet() const {
    return 11;
}
uint32_t Poisoner::getAttackAnimationsNumberInSet() const {
    return 13;
}
std::string Poisoner::getBaseTextureName() const {
    return "poisoner";
}
uint32_t Poisoner::getMovementPoints() const {
    return Parameters::get().getInt("poisoner_movement_points");
}
uint32_t Poisoner::getPopulation() const {
    return Parameters::get().getInt("poisoner_population");
}
uint32_t Poisoner::getAttackRadius() const {
    return Parameters::get().getInt("poisoner_attack_radius");
}
Events Poisoner::eventAfterAnimation(uint32_t targetX, uint32_t targetY) {
    std::shared_ptr<PoisonerArrow> poisonerArrow = std::make_shared<PoisonerArrow>();
    poisonerArrow->setSrc(this->getCenterX(), this->getCenterY());
    poisonerArrow->setDst(64 * targetX + 32, 64 * targetY + 32);

    Events events;
    events.add(std::make_shared<PlaySoundEvent>(poisonerArrow->getSoundName()));
    events.add(std::make_shared<CreateEEvent>(poisonerArrow));

    return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(Poisoner)