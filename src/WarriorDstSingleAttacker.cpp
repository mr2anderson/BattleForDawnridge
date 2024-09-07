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


#include "WarriorDstSingleAttacker.hpp"
#include "RefreshAttackAbilityEvent.hpp"
#include "WipeAttackAbilityEvent.hpp"
#include "Locales.hpp"
#include "ColorTheme.hpp"


WarriorDstSingleAttacker::WarriorDstSingleAttacker() {
    this->attackAbility = true;
}
WarriorDstSingleAttacker::WarriorDstSingleAttacker(uint32_t x, uint32_t y, uint32_t playerId) : WarriorDstAttacker(x, y, playerId) {
    this->attackAbility = true;
}
Events WarriorDstSingleAttacker::endMove(MapState *state, uint32_t currentPlayerId) {
    Events events = Warrior::endMove(state, currentPlayerId);

    if (this->exist()) {
        Events refreshAttackAbilityEvent;
        refreshAttackAbilityEvent.add(std::make_shared<RefreshAttackAbilityEvent>(this->getThis<WarriorDstSingleAttacker>()));
        events = refreshAttackAbilityEvent + events;
    }

    return events;
}
std::vector<std::tuple<uint32_t, uint32_t>> WarriorDstSingleAttacker::canAttack(std::shared_ptr<Unit>u) const {
    if (this->attackAbility) {
        return this->WarriorDstAttacker::canAttack(u);
    }
    return {};
}
Events WarriorDstSingleAttacker::startAttack(MapState *state, std::shared_ptr<Unit>u, uint32_t targetX, uint32_t targetY) {
    Events events = WarriorDstAttacker::startAttack(state, u, targetX, targetY);

    Events wipeAttackAbilityEvent;
    wipeAttackAbilityEvent.add(std::make_shared<WipeAttackAbilityEvent>(this->getThis<WarriorDstSingleAttacker>()));

    return wipeAttackAbilityEvent + events;
}
void WarriorDstSingleAttacker::refreshAbility() {
    this->attackAbility = true;
}
void WarriorDstSingleAttacker::wipeAbility() {
    this->attackAbility = false;
}
StringLcl WarriorDstSingleAttacker::getSpecialInfoString(MapState *state) const {
    StringLcl str =
            StringLcl("{damage}") + this->getDamage(state).getTypeReadable() + " " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->getMinDamagePoints(state)) + "-" + std::to_string(this->getMaxDamagePoints(state)) + ". " +
            StringLcl("{single_attack}") + ". {attack_radius} " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->getAttackRadius()) + ". ";

    if (this->attackAbility) {
        str = str + StringLcl("{can_attack}");
    }
    else {
        str = str + StringLcl("{cant_attack_until_next_move}");
    }

    return str;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorDstSingleAttacker)