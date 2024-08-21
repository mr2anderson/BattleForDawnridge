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


#include "Clock.hpp"
#include "math.hpp"


static constexpr uint32_t NS_IN_SECOND = bfdlib::math::pow(10, 9);
static constexpr uint32_t NS_IN_MILLISECOND = bfdlib::math::pow(10, 6);


Clock::Clock() {
    this->restart();
    this->nsBonus = 0;
}
uint32_t Clock::getMS() const {
    return this->getNS() / NS_IN_MILLISECOND;
}
uint64_t Clock::getNS() const {
    auto current = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(current - this->start).count() + this->nsBonus;
}
float Clock::getSecondsAsFloat() const {
    return (double)this->getNS() / (double)NS_IN_SECOND;
}
void Clock::restart() {
    this->start = std::chrono::high_resolution_clock::now();
}