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



#include "IBuildingSpec.hpp"
#include "HashTableMapPosition.hpp"


#pragma once


class AreaControllerSpec: public IBuildingSpec{
public:
	AreaControllerSpec();

	HashTableMapPosition<uint32_t> getAvailable(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state) const;
    static bool IN_RADIUS(const HashTableMapPosition<uint32_t>& available, const GO* target, uint8_t inRadiusType);
	Events getHighlightEvent(const Building *building, MapState* state, uint8_t type) const override;
	virtual bool ignoreUltraHighObstacles() const;
	virtual bool ignoreHighObstacles() const;
	virtual bool ignoreLowObstacles() const;
    virtual uint8_t getHighlightType() const = 0;
	virtual uint32_t getRadius() const = 0;
    virtual sf::Color getHighlightColor(uint32_t playerId) const = 0;

    enum HIGHLIGHT_TYPE {
        UNIVERSAL,
        TERRITORY,
        OTHER
    };
    enum IN_RADIUS_TYPE {
        FULLY,
        PARTIALLY
    };
private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<IBuildingSpec>(*this);
    }
};


BOOST_CLASS_EXPORT_KEY(AreaControllerSpec)