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
#include "ArchiveType.hpp"



#pragma once


struct CollectionsImpl;
class GO;
class AreaResourcePoint;
class ConductionResourcePoint;
class Unit;
class Building;
class Warrior;


class Collections {
public:
	Collections();
    ~Collections();

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
    std::unique_ptr<CollectionsImpl> impl;

    void clearSubClassCollections();
    void addToSubClassCollections(GO *object);

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version);
};