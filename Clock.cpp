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


#include <cmath>
#include "Clock.hpp"


static const double NS_IN_SECOND = std::pow(10, 9);


Clock::Clock() {
    this->restart();
}
uint32_t Clock::getMS() const {
    auto current = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->start).count();
}
float Clock::getSecondsAsFloat() const {
    auto current = std::chrono::high_resolution_clock::now();
    return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(current - this->start).count() / NS_IN_SECOND;
}
void Clock::restart() {
    this->start = std::chrono::high_resolution_clock::now();
}