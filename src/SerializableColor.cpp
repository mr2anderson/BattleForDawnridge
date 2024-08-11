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


#include "SerializableColor.hpp"


SerializableColor::SerializableColor() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 255;
}
SerializableColor::SerializableColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}
SerializableColor::SerializableColor(sf::Color c) {
    this->r = c.r;
    this->g = c.g;
    this->b = c.b;
    this->a = c.a;
}
bool SerializableColor::operator<(const SerializableColor &c) const {
    if (this->r < c.r) {
        return true;
    }
    if (this->r > c.r) {
        return false;
    }

    if (this->g < c.g) {
        return true;
    }
    if (this->g > c.g) {
        return false;
    }

    if (this->b < c.b) {
        return true;
    }
    if (this->b > c.b) {
        return false;
    }

    if (this->a < c.a) {
        return true;
    }
    return false;
}
sf::Color SerializableColor::getSfColor() const {
    return {this->r, this->g, this->b, this->a};
}