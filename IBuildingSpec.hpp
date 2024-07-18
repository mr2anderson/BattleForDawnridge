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


#include "Events.hpp"
#include "MapState.hpp"
#include "HorizontalSelectionWindowComponent.hpp"
#include "BuildingShortInfo.hpp"


#pragma once


class IBuildingSpec {
public:
	virtual IBuildingSpec* clone() const = 0;

	virtual Events getActiveNewMoveEvent(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, MapState* state, uint32_t playerId, const std::string &soundName, bool works);
	virtual Events getHighlightEvent(MapState* state, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, bool works, bool connectedToOrigin);
	virtual Events getEventOnDestroy(MapState* state, uint32_t playerId) const;
	virtual std::vector<HorizontalSelectionWindowComponent> getComponents(MapState* state, uint32_t playerId, const std::string &soundName, bool works, bool connectedToOrigin);
	virtual std::optional<BuildingShortInfo> getShortInfo(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy) const;
	virtual Resources getLimit() const;
	virtual bool isVictoryCondition() const;
	virtual bool allowBuilding(MapState* state, uint32_t x1, uint32_t y1, uint32_t sx1, uint32_t sy1, uint32_t playerId, bool works, bool connectedToOrigin, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2);
	virtual bool isOrigin() const;
	virtual bool isActiveConductor(bool works) const;
	virtual uint32_t getWarriorMovementCost(uint32_t thisPlayerId, uint32_t playerId) const;
	virtual bool warriorCanStay(uint32_t thisPlayerId, uint32_t playerId) const;
	virtual bool isUltraHighObstacle(uint32_t thisPlayerId, uint32_t playerId) const;
	virtual bool isHighObstacle(uint32_t thisPlayerId, uint32_t playerId) const;
	virtual bool isLowObstacle(uint32_t thisPlayerId, uint32_t playerId) const;
};