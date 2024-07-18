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


#include "AreaControllerSpec.hpp"


#pragma once


class TerritoryExpanderSpec : public AreaControllerSpec {
public:
	TerritoryExpanderSpec();

	bool allowBuilding(MapState* state, uint32_t x1, uint32_t y1, uint32_t sx1, uint32_t sy1, uint32_t playerId, bool works, bool connectedToOrigin, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) override;
	uint32_t getRadius() const override;
	virtual uint32_t getTerritoryExpandingRadius() const = 0;
};