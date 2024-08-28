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


#include "WarriorHealer.hpp"
#include "Parameters.hpp"
#include "Locales.hpp"
#include "HighlightColors.hpp"
#include "RefreshHealingAbilityEvent.hpp"
#include "WipeHealingAbilityEvent.hpp"


WarriorHealer::WarriorHealer() {
    this->healingAvailable = true;
}
WarriorHealer::WarriorHealer(uint32_t x, uint32_t y, uint32_t playerId) : Warrior(x, y, playerId) {
    this->healingAvailable = true;
}
void WarriorHealer::refreshHealingAbility() {
    this->healingAvailable = true;
}
void WarriorHealer::wipeHealingAbility() {
    this->healingAvailable = false;
}
uint32_t WarriorHealer::getHealingSpeed() const {
    uint32_t healingSpeed = this->getBaseHealingSpeed();
    if (this->inRage()) {
        healingSpeed = healingSpeed * (1 + Parameters::get().getDouble("rage_mode_healing_speed_bonus"));
    }
    return healingSpeed;
}
bool WarriorHealer::blockBuildingAbility() const {
    return false;
}
Events WarriorHealer::heal(std::shared_ptr<Warrior> w) {
    this->wipeMovementPoints();
    this->setDirection(this->getDirection(w));

    Events events;
    events.add(std::make_shared<WipeHealingAbilityEvent>(this->getThis<WarriorHealer>()));

    return events;
}
std::string WarriorHealer::getDirection(std::shared_ptr<Warrior> w) const {
    if (w->getY() > this->getY()) {
        return "s";
    }
    if (w->getY() < this->getY()) {
        return "n";
    }
    if (w->getX() > this->getX()) {
        return "e";
    }
    return "w";
}
Events WarriorHealer::newMove(MapState *state, uint32_t playerId) {
    Events events = Warrior::newMove(state, playerId);

    if (this->exist()) {
        Events refreshHealingAbility;
        refreshHealingAbility.add(std::make_shared<RefreshHealingAbilityEvent>(this->getThis<WarriorHealer>()));
        events = refreshHealingAbility + events;
    }

    return events;
}
bool WarriorHealer::healVehicles() const {
    return false;
}
bool WarriorHealer::canHeal(std::shared_ptr<Warrior> w) const {
    if (!this->healingAvailable or !w->exist() or w->getPlayerId() != this->getPlayerId() or w->getHP() == w->getMaxHP() or w->isVehicle() != this->healVehicles()) {
        return false;
    }

    uint32_t dx = std::max(this->getX(), w->getX()) - std::min(this->getX(), w->getX());
    uint32_t dy = std::max(this->getY(), w->getY()) - std::min(this->getY(), w->getY());

    return (dx < 2 and dy < 2 and (uint32_t)(dx == 1) + (uint32_t)(dy == 1) == 1);
}
std::vector<SpecialMove> WarriorHealer::getSpecialMoves(MapState *state) const {
    std::vector<SpecialMove> moves;

    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
        std::shared_ptr<Warrior> w = state->getCollectionsPtr()->getWarrior(i);
        if (this->canHeal(w)) {
            moves.emplace_back(w->getX(), w->getY(), HighlightColors::get().getWarriorHealColor(this->getPlayerId()));
        }
    }

    return moves;
}
Events WarriorHealer::handleSpecialMove(MapState *state, uint32_t targetX, uint32_t targetY) {
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
        std::shared_ptr<Warrior> w = state->getCollectionsPtr()->getWarrior(i);
        if (w->getX() == targetX and w->getY() == targetY and this->canHeal(w)) {
            return this->heal(w);
        }
    }

    return {};
}
StringLcl WarriorHealer::getSpecialInfoString() const {
    StringLcl str = StringLcl("{healing_speed}") + std::to_string(this->getHealingSpeed()) + ". ";
    if (this->healingAvailable) {
        str = str + StringLcl("{can_heal}");
    }
    else {
        str = str + StringLcl("{cant_heal_until_next_move}");
    }

    return str;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorHealer)