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



#include "ICollections.hpp"
#include "Collection.hpp"
#include "AreaResourcePoint.hpp"
#include "ConductionResourcePoint.hpp"
#include "Unit.hpp"
#include "Building.hpp"
#include "Warrior.hpp"


#pragma once


class Collections : public ICollections {
public:
	Collections();

	bool hasError(MapSize mapSize, uint32_t totalPlayers) const {
		return this->gos.hasError(mapSize, totalPlayers); // Skipping cheking sub class collections cuz they are built based on main collection
	}

	void add(std::shared_ptr<GO>object) override;

	uint32_t totalGOs() const override;
	uint32_t totalAreaRPs() const override;
    uint32_t totalConductionRPs() const override;
    uint32_t totalUnits() const override;
	uint32_t totalBuildings() const override;
	uint32_t totalWarriors() const override;

	std::shared_ptr<GO> getGO(uint32_t i, uint8_t filter) override;
    std::shared_ptr<AreaResourcePoint> getAreaRP(uint32_t i) override;
    std::shared_ptr<ConductionResourcePoint> getConductionRP(uint32_t i) override;
    std::shared_ptr<Unit> getUnit(uint32_t i) override;
	std::shared_ptr<Building>  getBuilding(uint32_t i) override;
	std::shared_ptr<Warrior>  getWarrior(uint32_t i) override;
private:
	Collection<GO> gos;
	Collection<AreaResourcePoint> areaRps;
	Collection<ConductionResourcePoint> conductionRps;
	Collection<Unit> units;
	Collection<Building> buildings;
	Collection<Warrior> warriors;

	void clearSubClassCollections();
	void addToSubClassCollections(std::shared_ptr<GO> object);

    friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<ICollections>(*this); // ICollection is an interface, but serialization is necessary in order to register inheritance in boost
		ar& this->gos;
		if (Archive::is_loading::value) {
			this->clearSubClassCollections();
			for (uint32_t i = 0; i < this->gos.size(); i = i + 1) {
				this->addToSubClassCollections(this->gos.at(i, FILTER::DEFAULT_PRIORITY));
			}
		}
	}
};


BOOST_CLASS_EXPORT_KEY(Collections)