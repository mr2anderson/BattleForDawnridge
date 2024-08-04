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


#include <map>
#include "IBuildingSpec.hpp"


#pragma once


class AreaControllerSpec: public IBuildingSpec{
public:
	AreaControllerSpec();

    enum IN_RADIUS_TYPE {
        FULLY,
        PARTIALLY
    };

	std::map<std::tuple<uint32_t, uint32_t>, uint32_t> getAvailable(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state) const;
	Events getHighlightEvent(const Building *building, MapState* state, uint8_t type) const override;
	bool inRadius(const Building *building, MapState* state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2, uint8_t type = IN_RADIUS_TYPE::FULLY) const;
	virtual bool ignoreUltraHighObstacles() const;
	virtual bool ignoreHighObstacles() const;
	virtual bool ignoreLowObstacles() const;
    virtual uint8_t getHighlightType() const = 0;
	virtual uint32_t getRadius() const = 0;
    virtual sf::Color getHighlightColor(uint32_t playerId) const = 0;

    enum HIGHLIGHT_TYPE {
        UNIVERSAL,
        TERRITORY,
        ATTACK,
        OTHER
    };
private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<IBuildingSpec>(*this);
    }
};


BOOST_CLASS_EXPORT_KEY(AreaControllerSpec)