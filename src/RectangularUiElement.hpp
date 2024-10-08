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
#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include "ArchiveType.hpp"


#pragma once


class RectangularUiElement : public sf::Drawable {
public:
    RectangularUiElement();
    RectangularUiElement(int32_t x, int32_t y, uint32_t w, uint32_t h);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    int32_t getX() const;
    int32_t getY() const;
    uint32_t getW() const;
    uint32_t getH() const;
private:
    int32_t x, y;
    uint32_t w, h;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar& this->x;
        ar& this->y;
        ar& this->w;
        ar& this->h;
    }
};


BOOST_CLASS_EXPORT_KEY(RectangularUiElement)