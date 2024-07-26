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


#include <limits>
#include "ShootingEfficiencyComp.hpp"


ShootingEfficiencyComp::ShootingEfficiencyComp(Damage damage) {
    this->damage = damage;
}
bool ShootingEfficiencyComp::operator()(const Unit *u1, const Unit *u2) {
    uint32_t hpLoss1 = this->damage.getHpLoss(u1->getDefence());
    uint32_t hpLoss2 = this->damage.getHpLoss(u2->getDefence());

    uint32_t shotsToKill1;
    if (hpLoss1 == 0) {
        shotsToKill1 = std::numeric_limits<uint32_t>::max();
    }
    else {
        shotsToKill1 = u1->getHP() / hpLoss1 + (bool)(u1->getHP() % hpLoss1);
    }

    uint32_t shotsToKill2;
    if (hpLoss2 == 0) {
        shotsToKill2 = std::numeric_limits<uint32_t>::max();
    }
    else {
        shotsToKill2 = u2->getHP() / hpLoss2 + (bool)(u2->getHP() % hpLoss2);
    }

    if (shotsToKill1 < shotsToKill2) {
        return true;
    }
    if (shotsToKill1 > shotsToKill2) {
        return false;
    }
    return (hpLoss1 < hpLoss2);
}