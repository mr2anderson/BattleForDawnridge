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



#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include "ArchiveType.hpp"
#include "Collection.hpp"


#pragma once


class GO;
class Effect;
class ResourcePoint;
class Unit;
class Building;
class Warrior;


class Collections {
public:
	Collections();

	void add(GO *object);

	uint32_t totalGOs() const;
	uint32_t totalEffects() const;
	uint32_t totalRPs() const;
    uint32_t totalUnits() const;
	uint32_t totalBuildings() const;
	uint32_t totalWarriors() const;

	GO* getGO(uint32_t i);
	Effect* getEffect(uint32_t i);
	ResourcePoint* getRP(uint32_t i);
    Unit* getUnit(uint32_t i);
	Building* getBuilding(uint32_t i);
	Warrior* getWarrior(uint32_t i);

	const GO* getGO(uint32_t i) const;
private:
	Collection<GO> gos;
	Collection<Effect> effects;
	Collection<ResourcePoint> rps;
    Collection<Unit> units;
	Collection<Building> buildings;
	Collection<Warrior> warriors;


    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version);
};


BOOST_CLASS_EXPORT_KEY(Collections)