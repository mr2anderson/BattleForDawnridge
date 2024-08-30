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


WarriorAttacker::WarriorAttacker() {
    this->target = nullptr;
}
WarriorAttacker::WarriorAttacker(uint32_t x, uint32_t y, uint32_t playerId) : Warrior(x, y, playerId) {
    this->target = nullptr;
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
    if (this->inRage()) {
        baseDamage = (1 + Parameters::get().getDouble("rage_mode_damage_bonus")) * baseDamage;
    }
    baseDamage = (1 + (float)state->getTimePtr()->getTimeMod().getPercentDelta(Player::IS_POSITIVE(this->getPlayerId())) / 100) * baseDamage;
    return baseDamage;
}
uint32_t WarriorAttacker::getAnimationNumber(const std::string& type, const std::string& direction) const {
    uint32_t baseResult = this->Warrior::getAnimationNumber(type, direction);
    if (baseResult != 0) {
        return baseResult;
    }

    if (type == "attack") {
        std::vector<std::string> possibleDirections = this->getAttackPossibleDirections();
        if (std::find(possibleDirections.begin(), possibleDirections.end(), direction) != possibleDirections.end()) {
            return this->getAttackAnimationsNumberInSet();
        }
    }
    return 0;
}
bool WarriorAttacker::blockBuildingAbility() const {
    return true;
}
uint32_t WarriorAttacker::getCurrentAnimationMs() const {
    uint32_t baseResult = this->Warrior::getCurrentAnimationMs();
    if (baseResult != 0) {
        return baseResult;
    }

    if (this->getCurrentAnimation() == "attack") {
        return 600;
    }
    return 0;
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
std::string WarriorAttacker::getDirection(uint32_t targetX, uint32_t targetY) const {
    if (targetY > this->getY()) {
        if (targetX > this->getX()) {
            return "se";
        }
        if (targetX < this->getX()) {
            return "sw";
        }
        return "s";
    }

    if (targetY < this->getY()) {
        if (targetX > this->getX()) {
            return "ne";
        }
        if (targetX < this->getX()) {
            return "nw";
        }
        return "n";
    }

    if (targetX < this->getX()) {
        return "w";
    }
    return "e";
}
Events WarriorAttacker::handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) {
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalUnits(); i = i + 1) {
        std::shared_ptr<Unit>u = state->getCollectionsPtr()->getUnit(i, FILTER::CLICK_PRIORITY);
        std::vector<std::tuple<uint32_t, uint32_t>> targets = this->canAttack(u);
        for (uint32_t j = 0; j < targets.size(); j = j + 1) {
            if (std::make_tuple(targetX, targetY) == targets.at(j)) {
                return this->startAttack(u, targetX, targetY);
            }
        }
    }

    return Events();
}
Events WarriorAttacker::startAttack(std::shared_ptr<Unit> u, uint32_t targetX, uint32_t targetY) {
    this->target = u;
    this->wipeMovementPoints();
    this->startAnimation("attack");
    this->setDirection(this->getDirection(targetX, targetY));

    Events events;
    events.add(std::make_shared<PlaySoundEvent>(this->getStartAttackSoundName()));
    events.add(std::make_shared<CreateAnimationEvent>(SuspendingAnimation(this->getThis<WarriorAttacker>())));
    return events;
}
Events WarriorAttacker::processCurrentAnimation(MapState *state) {
    Events events = this->Warrior::processCurrentAnimation(state);

    if (events.empty() and this->getCurrentAnimation() == "attack" and this->getCurrentAnimationState().finished) {
        events.add(std::make_shared<CloseAnimationEvent>());
        Damage baseDamage = this->getDamage(state);
        float k = 1 - (float)Parameters::get().getInt("damage_random_percent")  / 100 + (float)(GlobalRandomGenerator32::get().gen() % (2 * Parameters::get().getInt("damage_random_percent") + 1)) / 100;
        baseDamage = k * baseDamage;
        events = events + this->target->hit(baseDamage);
        this->startAnimation("talking");
    }

    return events;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorAttacker)