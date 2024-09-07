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


#include "HunterArrow.hpp"
#include "GlobalClock.hpp"


const uint32_t HunterArrow::TOTAL_TYPES = 1;


HunterArrow::HunterArrow() = default;
std::string HunterArrow::getTextureName() const {
    uint32_t type = GlobalClock::get().getMs() / ((uint32_t)(1000 * this->getTime()) / TOTAL_TYPES) % TOTAL_TYPES + 1;
    return "hunter_arrow" + std::to_string(type);
}
std::string HunterArrow::getSoundName() const {
    return "bow";
}
float HunterArrow::getTime() const {
    return 0.2f;
}


BOOST_CLASS_EXPORT_IMPLEMENT(HunterArrow)