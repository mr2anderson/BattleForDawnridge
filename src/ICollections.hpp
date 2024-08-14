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
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include "ArchiveType.hpp"


#pragma once


class GO;
class AreaResourcePoint;
class ConductionResourcePoint;
class Unit;
class Building;
class Warrior;


class ICollections {
public:
	ICollections(); // For boost serialization
	virtual ~ICollections();

	virtual void add(GO* object) = 0;

	virtual uint32_t totalGOs() const = 0;
	virtual uint32_t totalAreaRPs() const = 0;
	virtual uint32_t totalConductionRPs() const = 0;
	virtual uint32_t totalUnits() const = 0;
	virtual uint32_t totalBuildings() const = 0;
	virtual uint32_t totalWarriors() const = 0;

	virtual GO* getGO(uint32_t i, uint8_t filter) = 0;
	virtual AreaResourcePoint* getAreaRP(uint32_t i) = 0;
	virtual ConductionResourcePoint* getConductionRP(uint32_t i) = 0;
	virtual Unit* getUnit(uint32_t i) = 0;
	virtual Building* getBuilding(uint32_t i) = 0;
	virtual Warrior* getWarrior(uint32_t i) = 0;
private:
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		
	}
};


BOOST_CLASS_EXPORT_KEY(ICollections)