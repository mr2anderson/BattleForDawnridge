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


#include "WarriorAttacker.hpp"
#include "Parameters.hpp"
#include "CreateAnimationEvent.hpp"
#include "CloseAnimationEvent.hpp"
#include "HighlightColors.hpp"
#include "PlaySoundEvent.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "StartWarriorAnimationEvent.hpp"
#include "ChangeWarriorDirectionEvent.hpp"


WarriorAttacker::WarriorAttacker() = default;
WarriorAttacker::WarriorAttacker(uint32_t x, uint32_t y, uint32_t playerId) : Warrior(x, y, playerId) {

}
uint32_t WarriorAttacker::getMinDamagePoints(MapState *state) const {
    uint32_t p = this->getDamage(state).getPoints();
    return (1 - (float)Parameters::get().getInt("damage_random_percent") / 100) * p;
}
uint32_t WarriorAttacker::getMaxDamagePoints(MapState *state) const {
    uint32_t p = this->getDamage(state).getPoints();
    return (1 + (float)Parameters::get().getInt("damage_random_percent") / 100) * p;
}
Damage WarriorAttacker::getDamage(MapState *state) const {
    Damage baseDamage = this->getBaseDamage();
    double k = 0;
    k = k + (double)state->getTimePtr()->getTimeMod().getPercentDelta(Player::IS_POSITIVE(this->getPlayerId())) / (double)100;
    if (this->inRage()) {
        k = k + Parameters::get().getDouble("rage_mode_damage_bonus");
    }
    if (this->isInspired()) {
        k = k + Parameters::get().getDouble("inspired_damage_bonus");
    }
    baseDamage = (1 + k) * baseDamage;
    return baseDamage;
}
bool WarriorAttacker::blockBuildingAbility() const {
    return true;
}
std::vector<SpecialMove> WarriorAttacker::getSpecialMoves(MapState *state) const {
    std::vector<SpecialMove> specialMoves;

    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalUnits(); i = i + 1) {
        std::shared_ptr<Unit>u = state->getCollectionsPtr()->getUnit(i, FILTER::CLICK_PRIORITY);
        std::vector<std::tuple<uint32_t, uint32_t>> targets = this->canAttack(u);
        for (uint32_t j = 0; j < targets.size(); j = j + 1) {
            specialMoves.emplace_back(std::get<0>(targets.at(j)), std::get<1>(targets.at(j)), HighlightColors::get().getWarriorAttackColor(this->getPlayerId()));
        }
    }

    return specialMoves;
}
Events WarriorAttacker::eventAfterAnimation(uint32_t targetX, uint32_t targetY) {
    return Events();
}
std::vector<std::tuple<uint32_t, uint32_t>> WarriorAttacker::canAttack(std::shared_ptr<Unit>u) const {
    std::vector<std::tuple<uint32_t, uint32_t>> result;
    if (!u->exist() or u->getPlayerId() == this->getPlayerId()) {
        return result;
    }

    for (uint32_t x = u->getX(); x < u->getX() + u->getSX(); x = x + 1) {
        for (uint32_t y = u->getY(); y < u->getY() + u->getSY(); y = y + 1) {
            if (x == this->getX() and y == this->getY()) {
                continue;
            }
            result.emplace_back(x, y);
        }
    }

    return result;
}
Events WarriorAttacker::handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) {
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalUnits(); i = i + 1) {
        std::shared_ptr<Unit>u = state->getCollectionsPtr()->getUnit(i, FILTER::CLICK_PRIORITY);
        std::vector<std::tuple<uint32_t, uint32_t>> targets = this->canAttack(u);
        for (uint32_t j = 0; j < targets.size(); j = j + 1) {
            if (std::make_tuple(targetX, targetY) == targets.at(j)) {
                return this->startAttack(state, u, targetX, targetY);
            }
        }
    }

    return Events();
}
bool WarriorAttacker::isInspirable() const {
    return true;
}
Events WarriorAttacker::startAttack(MapState *state, std::shared_ptr<Unit> u, uint32_t targetX, uint32_t targetY) {
    Events events;

    this->wipeMovementPoints();

    events.add(std::make_shared<StartWarriorAnimationEvent>(this->getThis<Warrior>(), "attack"));
    events.add(std::make_shared<ChangeWarriorDirectionEvent>(this->getThis<Warrior>(), this->getDirectionTo(targetX, targetY)));

    if (this->getStartAttackSoundName().has_value()) {
        events.add(std::make_shared<PlaySoundEvent>(this->getStartAttackSoundName().value()));
    }

    events.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this->getThis<WarriorAttacker>())));

    events = events + this->eventAfterAnimation(targetX, targetY);

    Damage baseDamage = this->getDamage(state);
    float k = 1 - (float)Parameters::get().getInt("damage_random_percent") / 100 + (float)(GlobalRandomGenerator32::get().gen() % (2 * Parameters::get().getInt("damage_random_percent") + 1)) / 100;
    baseDamage = k * baseDamage;
    events = events + u->hit(baseDamage);

    return events;
}
boost::optional<SpecialAnimation> WarriorAttacker::getSpecialAnimation() const {
    SpecialAnimation special;
    special.name = "attack";
    special.ms = 600;
    special.straightFrames = this->getAttackAnimationsNumberInSet();
    special.obliquelyFrames = this->haveObliquelyAttacks() * this->getAttackAnimationsNumberInSet();
    return special;
}
Events WarriorAttacker::processSpecialAnimation() {
    Events events;

    if (this->getCurrentAnimationState().finished) {
        events.add(std::make_shared<CloseAnimationEvent>());
        events.add(std::make_shared<StartWarriorAnimationEvent>(this->getThis<Warrior>(), "talking"));
    }

    return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorAttacker)