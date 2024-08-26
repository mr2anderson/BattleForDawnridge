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


#include <cstdint>
#include "ColorBlender.hpp"


ColorBlender::ColorBlender() = default;
sf::Color ColorBlender::blend(sf::Color c1, sf::Color c2) const {
    return this->blend({c1, c2});
}
sf::Color ColorBlender::blend(std::vector<sf::Color> colors) const {
    uint32_t r = 0;
    uint32_t g = 0;
    uint32_t b = 0;
    uint32_t a = 0;

    for (auto color : colors) {
        r = r + color.r;
        g = g + color.g;
        b = b + color.b;
        a = a + color.a;
    }

    return sf::Color(r / colors.size(), g / colors.size(), b / colors.size(), a / colors.size());
}