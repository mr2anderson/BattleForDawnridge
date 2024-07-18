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


#include <map>
#include "IBuildingSpec.hpp"


#pragma once


class AreaControllerSpec: public IBuildingSpec{
public:
	AreaControllerSpec();

	std::map<std::tuple<uint32_t, uint32_t>, uint32_t> getAvailable(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state);
	Events getHighlightEvent(const Building *building, MapState* state) override;
	bool inRadius(const Building *building, MapState* state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2);
	virtual bool ignoreUltraHighObstacles() const;
	virtual bool ignoreHighObstacles() const;
	virtual bool ignoreLowObstacles() const;
	virtual uint32_t getRadius() const = 0;
private:
	uint64_t prevHash;
	std::map<std::tuple<uint32_t, uint32_t>, uint32_t> prevAvailable;

	uint64_t getHash(MapState* state) const;
};