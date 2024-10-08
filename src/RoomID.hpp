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
#include <string>
#include <boost/serialization/access.hpp>
#include "StringLcl.hpp"


#pragma once


class RoomID {
public:
	RoomID();
	RoomID(const std::string& value);

	bool operator==(const RoomID& b) const;
	bool operator!=(const RoomID& b) const;

	StringLcl string() const;
    std::string key() const;

    static const uint32_t READABLE_LEN;
private:
	std::string _key;

	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar & this->_key;
	}
};


template <> struct std::hash<RoomID> {
	std::size_t operator()(const RoomID& k) const {
		return std::hash<std::string>{}(k.key());
	}
};