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


#include <SFML/Graphics.hpp>
#include <cstdint>
#include "ArchiveType.hpp"


#pragma once


class SerializableColor {
public:
    SerializableColor();
    SerializableColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    SerializableColor(sf::Color c);

    bool operator<(const SerializableColor &a) const;

    sf::Color getSfColor() const;
private:
    uint32_t c;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar & this->c;
    }
};