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
    struct SPEC {
        static constexpr uint8_t NONE = 0;
        static constexpr uint8_t POISON = 1;
    };

	Damage();
	Damage(uint32_t points, uint8_t type, uint8_t spec = SPEC::NONE);

	friend Damage operator*(double k, Damage damage);
	uint32_t getHpLoss(Defence defence) const;
	uint32_t getPoints() const;
	StringLcl getReadable() const;
	StringLcl getTypeReadable() const;
    bool hasSpec(uint8_t val) const;

	enum TYPE {
		CUT,
		STAB,
		CRUSH,
        SERVICE
	};
private:
	uint32_t points;
	uint8_t type;
    uint8_t spec;
};