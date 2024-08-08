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



#include "AreaResourcePoint.hpp"


#pragma once


class Forest : public AreaResourcePoint {
public:
	Forest();
	Forest(uint32_t x, uint32_t y, uint32_t type);
	UUID getTypeUUID() const override;

    uint32_t getSX() const override;
    uint32_t getSY() const override;
    uint32_t getMaxHP() const override;
	std::string getResourceType() const override;
	std::string getSoundName() const override;
private:
	uint32_t type;

	std::string getTextureName() const override;
	sf::IntRect getTextureRect() const override;
	StringLcl getDescription() const override;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<ResourcePoint>(*this);
        ar & this->type;
    }
};


BOOST_CLASS_EXPORT_KEY(Forest)