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


#include "BigArrow.hpp"
#include "GlobalRandomGenerator.hpp"
#include "GlobalClock.hpp"


const uint32_t BigArrow::TOTAL_TYPES = 25;
const uint32_t BigArrow::TOTAL_SOUNDS = 3;


BigArrow::BigArrow() = default;
std::string BigArrow::getTextureName() const {
    uint32_t type = GlobalClock::get()->getMs() / (1000 / TOTAL_TYPES) % TOTAL_TYPES + 1;
    return "big_arrow" + std::to_string(type);
}
std::string BigArrow::getIconTextureName() const {
    return "big_arrow_icon";
}
std::string BigArrow::getSoundName() const {
    return "big_arrow" + std::to_string(GlobalRandomGenerator::get()->gen() % TOTAL_SOUNDS + 1);
}
float BigArrow::getTime() const {
    return 0.5f;
}