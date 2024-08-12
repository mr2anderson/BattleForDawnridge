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


#include "FlyingE.hpp"


#pragma once


class HPFlyingE : public FlyingE {
public:
    HPFlyingE();
    HPFlyingE(uint32_t delta, bool plus, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);
private:
    uint32_t delta;
    bool plus;

    std::unique_ptr<sf::Drawable> getDrawable(sf::Vector2f position, sf::Color color) const override;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar& boost::serialization::base_object<FlyingE>(*this);
        ar& this->delta;
        ar& this->plus;
    }
};


BOOST_CLASS_EXPORT_KEY(HPFlyingE)