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



#include <set>
#include <boost/serialization/access.hpp>
#include "serialize-tuple/serialize_tuple.h"
#include "SetHighlightEvent.hpp"
#include "HashTableMapPosition.hpp"
#include "SerializableColor.hpp"
#include "ArchiveType.hpp"


#pragma once


class HighlightTable {
public:
	HighlightTable();

	void clear();
	void mark(SetHighlightEvent e);
	std::vector<sf::RectangleShape> getRects() const;
private:
	HashTableMapPosition<std::set<SerializableColor>> data;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar & this->data;
    }
};