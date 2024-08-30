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


#include <memory>
#include <boost/serialization/shared_ptr.hpp>
#include "ICollections.hpp"
#include "MapSize.hpp"
#include "Players.hpp"
#include "Time.hpp"


#pragma once


class MapState {
public:
	MapState();

	bool hasError() const {
		return (this->icollections == nullptr or this->mapSize.hasError() or this->players.hasError() or this->icollections->hasError(this->mapSize, this->players.total()) or time.hasError());
	}

	std::shared_ptr<ICollections> getCollectionsPtr();
	MapSize* getMapSizePtr();
	Players* getPlayersPtr();
    Time* getTimePtr();

	const MapSize* getMapSizePtr() const;
	const Players* getPlayersPtr() const;
private:
	std::shared_ptr<ICollections> icollections;
	MapSize mapSize;
	Players players;
    Time time;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->icollections;
        ar & this->mapSize;
        ar & this->players;
        ar & this->time;
    }
};