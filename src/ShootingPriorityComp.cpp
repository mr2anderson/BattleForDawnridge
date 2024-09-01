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


#include "ShootingPriorityComp.hpp"


ShootingPriorityComp::ShootingPriorityComp(std::shared_ptr<const Building> b) {
    this->b = b;
}
bool ShootingPriorityComp::operator()(std::shared_ptr<Unit> u1, std::shared_ptr<Unit> u2) const {
    double k1 = u1->getDefence().getStab();
    double k2 = u2->getDefence().getStab();

    if (k1 == k2) {
        return (u1->getHP() < u2->getHP());
    }

    return (k1 < k2); // Selecting units with the highest resistance (this is not bug)
}