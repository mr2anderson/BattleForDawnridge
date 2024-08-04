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


#include <boost/serialization/base_object.hpp>
#include "Events.hpp"
#include "MapState.hpp"
#include "BuildingHorizontalSelectionWindowComponent.hpp"
#include "BuildingShortInfo.hpp"
#include "NewMoveMainPriority.hpp"
#include "ArchiveType.hpp"


#pragma once


class Building;
class Warrior;


class IBuildingSpec {
public:
    virtual ~IBuildingSpec();
	virtual IBuildingSpec* clone() const = 0;

	virtual Events getActiveNewMoveEvent(const Building *building, MapState* state);
	virtual Events getHighlightEvent(const Building *building, MapState* state, uint8_t type) const;
	virtual Events getEventOnDestroy(const Building *building, MapState* state) const;
	virtual std::vector<BuildingHorizontalSelectionWindowComponent> getComponents(const Building *building, MapState* state);
	virtual std::optional<BuildingShortInfo> getShortInfo(const Building *building) const;
	virtual Resources getLimit(const Building *building) const;
	virtual uint32_t getPopulationLimit(const Building *building) const;
	virtual bool isVictoryCondition() const;
	virtual bool allowBuilding(const Building *building, MapState* state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) const;
	virtual bool isOrigin() const;
	virtual bool isActiveConductor(const Building *building) const;
	virtual uint32_t getWarriorMovementCost(const Building *building, const Warrior *w) const;
	virtual bool warriorCanStay(const Building *building, const Warrior *w) const;
	virtual bool isUltraHighObstacle(const Building *building, uint32_t playerId) const;
	virtual bool isHighObstacle(const Building *building, uint32_t playerId) const;
	virtual bool isLowObstacle(const Building *building, uint32_t playerId) const;
    virtual NewMoveMainPriority getNewMoveMainPriority() const;
private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {

    }
};


BOOST_CLASS_EXPORT_KEY(IBuildingSpec)