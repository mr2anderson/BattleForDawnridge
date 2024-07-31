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


#include "Collections.hpp"
#include "MapSize.hpp"
#include "Players.hpp"


#pragma once


class MapState {
public:
	MapState();

	Collections* getCollectionsPtr();
	MapSize* getMapSizePtr();
	Players* getPlayersPtr();

	const Collections* getCollectionsPtr() const;
private:
	Collections collections;
	MapSize mapSize;
	Players players;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->collections;
        ar & this->mapSize;
        ar & this->players;
    }
};


BOOST_CLASS_EXPORT_KEY(MapState)