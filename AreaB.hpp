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


#include "Building.hpp"


#pragma once


class AreaB : virtual public Building {
public:
	AreaB();
	AreaB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* player);

	bool inRadius(GO* go) const;
	bool inRadius(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) const;
	virtual Event getHighlightEvent() const;
protected:

	virtual uint32_t getRadius() const = 0;
};