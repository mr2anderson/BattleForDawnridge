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


#include "WarriorNearMultyAttacker.hpp"
#include "RefreshAttackedTableEvent.hpp"
#include "MarkAsAttackedEvent.hpp"
#include "Locales.hpp"


WarriorNearMultyAttacker::WarriorNearMultyAttacker() = default;
WarriorNearMultyAttacker::WarriorNearMultyAttacker(uint32_t x, uint32_t y, uint32_t playerId) : WarriorNearAttacker(x, y, playerId) {

}
void WarriorNearMultyAttacker::refreshAttackedTable() {
    this->attackedTable.clear();
}
void WarriorNearMultyAttacker::markAsAttacked(Unit *u) {
    this->attackedTable[u] = true;
}
Events WarriorNearMultyAttacker::newMove(MapState *state, uint32_t currentPlayerId) {
    Events events = this->WarriorAttacker::newMove(state, currentPlayerId);

    if (this->exist()) {
        Events refreshAttackedTable;
        refreshAttackedTable.add(std::make_shared<RefreshAttackedTableEvent>(this));
        events = refreshAttackedTable + events;
    }

    return events;
}
std::vector<std::tuple<uint32_t, uint32_t>> WarriorNearMultyAttacker::canAttack(Unit *u) const {
    if (this->attackedTable.find(u) == this->attackedTable.end()) {
        return this->WarriorNearAttacker::canAttack(u);
    }
    return {};
}
Events WarriorNearMultyAttacker::startAttack(Unit *u, uint32_t targetX, uint32_t targetY) {
    Events events = this->WarriorAttacker::startAttack(u, targetX, targetY);

    Events markAsAttacked;
    markAsAttacked.add(std::make_shared<MarkAsAttackedEvent>(this, u));
    events = events + markAsAttacked;

    return events;
}
std::wstring WarriorNearMultyAttacker::getSpecialInfoString() const {
    return *Locales::get()->get("damage") + this->getDamage().getReadable() + L". " + *Locales::get()->get("multy_attack");
}