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


#include "IBuildingSpec.hpp"


#pragma once


class WallSpec : public IBuildingSpec {
public:
	WallSpec();
	IBuildingSpec* clone() const override;

	uint32_t getWarriorMovementCost(const Building *b, const Warrior *w) const override;
	bool warriorCanStay(const Building *b, const Warrior *w) const override;
	bool isHighObstacle(const Building *b, uint32_t playerId) const override;
};