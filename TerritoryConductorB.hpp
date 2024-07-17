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


#include "TerritoryOriginB.hpp"
#include "ConductorB.hpp"
#include "Collection.hpp"


#pragma once


class TerritoryConductorB : public TerritoryB, public ConductorB {
public:
	TerritoryConductorB();
	TerritoryConductorB(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<Collection<Unit>> units, std::shared_ptr<Collection<GO>> go, uint32_t mapW, uint32_t mapH, std::shared_ptr<Collection<TerritoryOriginB>> originsPtr);
	
	bool allowBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId) override;
	Events getHighlightEvent() override;
protected:
	bool connectedToTerritoryOriginB() const;
	HorizontalSelectionWindowComponent getNotConnectedComponent() const;
private:
	std::shared_ptr<Collection<TerritoryOriginB>> originsPtr;
};