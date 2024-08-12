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
#include "Defence.hpp"


#pragma once


class Damage {
public:
	Damage();
	Damage(uint32_t points, uint8_t type);

	friend Damage operator*(double k, Damage damage);
	uint32_t getHpLoss(Defence defence) const;
	StringLcl getReadable() const;

	enum TYPE {
		CUT,
		STAB,
		CRUSH,
	};
private:
	uint32_t points;
	uint8_t type;
};