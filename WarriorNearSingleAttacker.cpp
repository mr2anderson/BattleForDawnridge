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


#include "WarriorNearSingleAttacker.hpp"
#include "RefreshAttackAbilityEvent.hpp"
#include "WipeAttackAbilityEvent.hpp"
#include "Locales.hpp"


WarriorNearSingleAttacker::WarriorNearSingleAttacker() {
    this->attackAbility = true;
}
WarriorNearSingleAttacker::WarriorNearSingleAttacker(uint32_t x, uint32_t y, uint32_t playerId) : WarriorNearAttacker(x, y, playerId) {
    this->attackAbility = true;
}
Events WarriorNearSingleAttacker::newMove(MapState *state, uint32_t currentPlayerId) {
    Events events = Warrior::newMove(state, currentPlayerId);

    if (this->exist()) {
        Events refreshAttackAbilityEvent;
        refreshAttackAbilityEvent.add(std::make_shared<RefreshAttackAbilityEvent>(this));
        events = refreshAttackAbilityEvent + events;
    }

    return events;
}
std::vector<std::tuple<uint32_t, uint32_t>> WarriorNearSingleAttacker::canAttack(Unit *u) const {
    if (this->attackAbility) {
        return this->WarriorNearAttacker::canAttack(u);
    }
    return {};
}
Events WarriorNearSingleAttacker::startAttack(Unit *u, uint32_t targetX, uint32_t targetY) {
    Events events = WarriorNearAttacker::startAttack(u, targetX, targetY);

    Events wipeAttackAbilityEvent;
    wipeAttackAbilityEvent.add(std::make_shared<WipeAttackAbilityEvent>(this));

    return wipeAttackAbilityEvent + events;
}
void WarriorNearSingleAttacker::refreshAbility() {
    this->attackAbility = true;
}
void WarriorNearSingleAttacker::wipeAbility() {
    this->attackAbility = false;
}
std::wstring WarriorNearSingleAttacker::getSpecialInfoString() const {
    std::wstring str =
            *Locales::get()->get("damage") + this->getDamage().getReadable() + L". " +
            *Locales::get()->get("single_attack") + L". ";

    if (this->attackAbility) {
        str = str + *Locales::get()->get("can_attack");
    }
    else {
        str = str + *Locales::get()->get("cant_attack_until_next_move");
    }

    return str;
}