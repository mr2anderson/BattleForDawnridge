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
#include "ShootingPriorityComp.hpp"


ShootingPriorityComp::ShootingPriorityComp(const GO* shootingBuilding) {
    this->shootingBuilding = shootingBuilding;
}
bool ShootingPriorityComp::operator()(const GO* go1, const GO* go2) const {
    uint32_t dst1 = std::numeric_limits<uint32_t>::max();
    uint32_t dst2 = std::numeric_limits<uint32_t>::max();

    for (uint32_t x = this->shootingBuilding->getX(); x < this->shootingBuilding->getX() + this->shootingBuilding->getSX(); x = x + 1) {
        for (uint32_t y = this->shootingBuilding->getY(); y < this->shootingBuilding->getY() + this->shootingBuilding->getSY(); y = y + 1) {

            for (uint32_t x2 = go1->getX(); x2 < go1->getX() + go1->getSX(); x2 = x2 + 1) {
                for (uint32_t y2 = go1->getY(); y2 < go1->getY() + go1->getSY(); y2 = y2 + 1) {
                    uint32_t dst = std::max(x, x2) - std::min(x, x2) + std::max(y, y2) - std::min(y, y2);
                    dst1 = std::min(dst1, dst);
                }
            }

            for (uint32_t x2 = go2->getX(); x2 < go2->getX() + go2->getSX(); x2 = x2 + 1) {
                for (uint32_t y2 = go2->getY(); y2 < go2->getY() + go2->getSY(); y2 = y2 + 1) {
                    uint32_t dst = std::max(x, x2) - std::min(x, x2) + std::max(y, y2) - std::min(y, y2);
                    dst2 = std::min(dst2, dst);
                }
            }
        }
    }

    return (dst1 <= dst2);
}