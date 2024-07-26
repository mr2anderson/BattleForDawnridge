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


#include "WarriorNearAttacker.hpp"


WarriorNearAttacker::WarriorNearAttacker() = default;
WarriorNearAttacker::WarriorNearAttacker(uint32_t x, uint32_t y, uint32_t playerId) : WarriorAttacker(x, y, playerId) {

}
std::vector<std::tuple<uint32_t, uint32_t>> WarriorNearAttacker::canAttack(Unit *u) const {
    std::vector<std::tuple<uint32_t, uint32_t>> candidates = this->WarriorAttacker::canAttack(u);

    std::vector<std::tuple<uint32_t, uint32_t>> result;
    for (uint32_t i = 0; i < candidates.size(); i = i + 1) {
        uint32_t dx = std::max(std::get<0>(candidates[i]), this->getX()) - std::min(std::get<0>(candidates[i]), this->getX());
        uint32_t dy = std::max(std::get<1>(candidates[i]), this->getY()) - std::min(std::get<1>(candidates[i]), this->getY());
        if (dx > 1 or dy > 1) {
            continue;
        }
        if ((uint32_t)(dx > 0) + (uint32_t)(dy > 0) == 2) {
            continue;
        }
        result.push_back(candidates[i]);
    }
    return result;
}
std::vector<std::string> WarriorNearAttacker::getAttackPossibleDirections() const {
    return {"n", "s", "w", "e"};
}