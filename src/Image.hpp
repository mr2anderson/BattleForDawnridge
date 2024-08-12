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


#include <boost/optional.hpp>
#include <boost/serialization/optional.hpp>
#include <memory>
#include "RectangularUiElement.hpp"
#include "IntRectSerializable.hpp"


#pragma once


class Image : public RectangularUiElement {
public:
    Image();
    Image(int32_t x, int32_t y, const std::string &textureName, boost::optional<IntRectSerializable> rect = boost::none); // creating image with size of texture
    Image(int32_t x, int32_t y, uint32_t size, const std::string &textureName, boost::optional<IntRectSerializable> rect = boost::none); // creating image making it fit in specified size by scaling

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    std::string textureName;
    boost::optional<uint32_t> size;
    boost::optional<IntRectSerializable> textureRect;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar& boost::serialization::base_object<RectangularUiElement>(*this);
        ar& this->textureName;
        ar& this->size;
        ar& this->textureRect;
    }
};


BOOST_CLASS_EXPORT_KEY(Image)