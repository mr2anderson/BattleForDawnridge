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


SerializableColor::SerializableColor() : SerializableColor(0, 0, 0, 255) {

}
SerializableColor::SerializableColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->c = (uint32_t)r | ((uint32_t)g << 8) | ((uint32_t)b << 16) | ((uint32_t)a << 24);
}
SerializableColor::SerializableColor(sf::Color c) : SerializableColor(c.r, c.g, c.b, c.a) {

}
bool SerializableColor::operator<(const SerializableColor &a) const {
    return this->c < a.c;
}
sf::Color SerializableColor::getSfColor() const {
    uint8_t r = (this->c) & 0xFF;
    uint8_t g = (this->c >> 8) & 0xFF;
    uint8_t b = (this->c >> 16) & 0xFF;
    uint8_t a = (this->c >> 24) & 0xFF;
    return {r, g, b, a};
}