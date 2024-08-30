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


#include "PatriarchProjectile.hpp"


const uint32_t PatriarchProjectile::TOTAL_TYPES = 48;


PatriarchProjectile::PatriarchProjectile() = default;
std::string PatriarchProjectile::getTextureName() const {
    return "patriarch_projectile" + std::to_string(std::min(TOTAL_TYPES, (uint32_t)(this->animationClock.getMS() / ((uint32_t)(1000 * this->getTime()) / TOTAL_TYPES) + 1)));
}
std::string PatriarchProjectile::getSoundName() const {
    return "patriarch_heal";
}
float PatriarchProjectile::getTime() const {
    return 0.5f;
}


BOOST_CLASS_EXPORT_IMPLEMENT(PatriarchProjectile)