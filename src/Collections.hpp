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



#include "ArchiveType.hpp"
#include "Collection.hpp"


#pragma once


class GO;
class AreaResourcePoint;
class ConductionResourcePoint;
class Unit;
class Building;
class Warrior;


class Collections {
public:
	Collections();

	void add(GO *object);

	uint32_t totalGOs() const;
	uint32_t totalAreaRPs() const;
    uint32_t totalConductionRPs() const;
    uint32_t totalUnits() const;
	uint32_t totalBuildings() const;
	uint32_t totalWarriors() const;

	GO* getGO(uint32_t i, uint8_t filter);
    const GO* getGO(uint32_t i, uint8_t filter) const;
    AreaResourcePoint* getAreaRP(uint32_t i);
    ConductionResourcePoint* getConductionRP(uint32_t i);
    Unit* getUnit(uint32_t i);
	Building* getBuilding(uint32_t i);
	Warrior* getWarrior(uint32_t i);
private:
	Collection<GO> gos;
	Collection<AreaResourcePoint> areaRps;
    Collection<ConductionResourcePoint> conductionRps;
    Collection<Unit> units;
	Collection<Building> buildings;
	Collection<Warrior> warriors;

    void addToSubClassCollections(GO *object);


    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        if (Archive::is_loading::value) {
            *this = Collections();
            ar & this->gos;
            for (uint32_t i = 0; i < this->gos.size(); i = i + 1) {
                this->addToSubClassCollections(this->gos.at(i, FILTER::DEFAULT_PRIORITY));
            }
        }
        else {
            ar & this->gos;
        }
    }
};