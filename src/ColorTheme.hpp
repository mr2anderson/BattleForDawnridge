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


#include <SFML/Graphics/Color.hpp>
#include <cstdint>


#pragma once


namespace COLOR_THEME {
    static const sf::Color UI_COLOR_TRANSPARENT = sf::Color(1, 0, 3, 75);




    static const sf::Color STATE_COLOR_WORST = sf::Color(100, 0, 20);
    static const sf::Color STATE_COLOR_BEST = sf::Color(0, 100, 20);
    static const sf::Color STATE_COLOR_NEUTRAL = STATE_COLOR_BEST;

    template<typename T> static sf::Color STATE_COLOR(T a, T b) {
        if (a >= b) {
            return STATE_COLOR_BEST;
        }
        uint32_t red = 100 - 100 * a / b;
        uint32_t green = 100 * a / b;
        uint32_t blue = 20;
        return sf::Color(red, green, blue);
    }

    template<typename T> static sf::Color STATE_COLOR_REV(T current, T minBest, T maxWorst) {
        if (current <= minBest) {
            return STATE_COLOR_BEST;
        }
        if (current >= maxWorst) {
            return STATE_COLOR_WORST;
        }
        uint32_t dstToMin = current - minBest;
        uint32_t dstToMax = maxWorst - current;
        uint32_t red = 100 - 100 * dstToMax / dstToMin;
        uint32_t green = 100 * dstToMax / dstToMin;
        uint32_t blue = 20;
        return sf::Color(red, green, blue);
    }
}