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


class IBuildingSpec : public std::enable_shared_from_this<IBuildingSpec> {
public:
    virtual ~IBuildingSpec();
	virtual std::shared_ptr<IBuildingSpec> clone() const = 0;

	virtual bool hasError(MapSize mapSize, uint32_t totalPlayers) const {
		return false;
	}

	virtual Events getActiveNewMoveEvent(std::shared_ptr<const Building> building, MapState* state);
	virtual Events getHighlightEvent(std::shared_ptr<const Building> building, MapState* state, uint8_t type) const;
	virtual Events getEventOnDestroy(std::shared_ptr<const Building> building, MapState* state) const;
	virtual std::vector<BuildingHorizontalSelectionWindowComponent> getComponents(std::shared_ptr<const Building> building, MapState* state);
	virtual boost::optional<BuildingShortInfo> getShortInfo(std::shared_ptr<const Building> building) const;
	virtual Resources getLimit(std::shared_ptr<const Building> building) const;
	virtual uint32_t getPopulationLimit(std::shared_ptr<const Building> building) const;
	virtual bool isVictoryCondition() const;
	virtual bool isOrigin() const;
	virtual bool isActiveConductor(std::shared_ptr<const Building> building) const;
	virtual uint32_t getWarriorMovementCost(std::shared_ptr<const Building> building, std::shared_ptr<Warrior> w) const;
	virtual bool warriorCanStay(std::shared_ptr<const Building> building, std::shared_ptr<const Warrior> w) const;
	virtual bool isUltraHighObstacle(std::shared_ptr<const Building> building, uint32_t playerId) const;
	virtual bool isHighObstacle(std::shared_ptr<const Building> building, uint32_t playerId) const;
	virtual bool isLowObstacle(std::shared_ptr<const Building> building, uint32_t playerId) const;
    virtual NewMoveMainPriority getNewMoveMainPriority() const;
protected:
	template<typename T> std::shared_ptr<T> getThis() {
		return std::static_pointer_cast<T>(this->shared_from_this());
	}
	template<typename T> std::shared_ptr<const T> getThis() const {
		return std::static_pointer_cast<const T>(this->shared_from_this());
	}
private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {

    }
};


BOOST_CLASS_EXPORT_KEY(IBuildingSpec)