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


#include "WhiteMage.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"
#include "CreateEEvent.hpp"
#include "HolyMagick.hpp"
#include "PlaySoundEvent.hpp"


WhiteMage::WhiteMage() = default;
WhiteMage::WhiteMage(uint32_t x, uint32_t y, uint32_t playerId) :
        WarriorDstSingleAttacker(x, y, playerId) {

}
std::shared_ptr<Warrior> WhiteMage::cloneWarrior() const {
    return std::make_shared<WhiteMage>(*this);
}
UUID WhiteMage::getTypeUUID() const {
    return UUIDs::get().get("white_mage");
}
std::string WhiteMage::getBeenHitSoundName() const {
    return "ouch_old";
}
boost::optional<std::string> WhiteMage::getStartAttackSoundName() const {
    return boost::none;
}
uint32_t WhiteMage::getMaxHP() const {
    return Parameters::get().getInt("white_mage_max_hp");
}
Defence WhiteMage::getBaseDefence() const {
    return Parameters::get().getDefence("white_mage_defence");
}
Damage WhiteMage::getBaseDamage() const {
    return Parameters::get().getDamage("white_mage_damage");
}
Resources WhiteMage::getCost() const {
    return Parameters::get().getResources("white_mage_cost");
}
uint32_t WhiteMage::getTimeToProduce() const {
    return Parameters::get().getInt("white_mage_time_to_produce");
}
std::string WhiteMage::getSoundName() const {
    return "old";
}
StringLcl WhiteMage::getDescription() const {
    return StringLcl("{white_mage_description}");
}
uint32_t WhiteMage::getTalkingAnimationsNumberInSet() const {
    return 9;
}
uint32_t WhiteMage::getRunningAnimationsNumberInSet() const {
    return 8;
}
uint32_t WhiteMage::getBeenHitAnimationsNumberInSet() const {
    return 9;
}
uint32_t WhiteMage::getTippingOverAnimationsNumberInSet() const {
    return 13;
}
uint32_t WhiteMage::getAttackAnimationsNumberInSet() const {
    return 13;
}
std::string WhiteMage::getBaseTextureName() const {
    return "white_mage";
}
uint32_t WhiteMage::getMovementPoints() const {
    return Parameters::get().getInt("white_mage_movement_points");
}
uint32_t WhiteMage::getPopulation() const {
    return Parameters::get().getInt("white_mage_population");
}
uint32_t WhiteMage::getAttackRadius() const {
    return Parameters::get().getInt("white_mage_attack_radius");
}
Events WhiteMage::eventAfterAnimation(uint32_t targetX, uint32_t targetY) {
    std::shared_ptr<HolyMagick> magick = std::make_shared<HolyMagick>();
    magick->setTarget(targetX * 64 + 32, targetY * 64 + 63);

    Events events;
    events.add(std::make_shared<PlaySoundEvent>(magick->getSoundName()));
    events.add(std::make_shared<CreateEEvent>(magick));

    return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WhiteMage)