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


#include "HealerProjectile.hpp"


const uint32_t HealerProjectile::TOTAL_TYPES = 30;


HealerProjectile::HealerProjectile() = default;
std::string HealerProjectile::getTextureName() const {
    return "healer_projectile" + std::to_string(std::min(HealerProjectile::TOTAL_TYPES, (uint32_t)(this->animationClock.getMS() / ((uint32_t)(1000 * this->getTime()) / TOTAL_TYPES) + 1)));
}
std::string HealerProjectile::getIconTextureName() const {
    return "healer_projectile1";
}
std::string HealerProjectile::getSoundName() const {
    return "healer";
}
float HealerProjectile::getTime() const {
    return 0.5;
}