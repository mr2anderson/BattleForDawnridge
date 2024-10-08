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


#include <cstdint>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include "ArchiveType.hpp"


#pragma once


class MapSize {
public:
	MapSize();

	bool hasError() const {
		return false;
	}

	void setWidth(uint32_t newWidth);
	void setHeight(uint32_t newHeight);
	
	uint32_t getWidth() const;
	uint32_t getHeight() const;
private:
	uint32_t width, height;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->width;
        ar & this->height;
    }
};


BOOST_CLASS_EXPORT_KEY(MapSize)