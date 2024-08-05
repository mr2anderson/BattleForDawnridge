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


#include "SFColorComp.hpp"


bool SFColorComp::operator()(const sf::Color &c1, const sf::Color &c2) const {
    if (c1.r < c2.r) {
        return true;
    }
    if (c1.r > c2.r) {
        return false;
    }

    if (c1.g < c2.g) {
        return true;
    }
    if (c1.g > c2.g) {
        return false;
    }

    if (c1.b < c2.b) {
        return true;
    }
    if (c1.b > c2.b) {
        return false;
    }

    if (c1.a < c2.a) {
        return true;
    }
    return false;
}