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


#include "Clock.hpp"


#pragma once


class Timer {
public:
	Timer();
	Timer(uint32_t ms, uint8_t type);

	bool ready() const;
	void reset();

	enum TYPE {
		FIRST_INSTANTLY,
		FIRST_DEFAULT
	};
private:
	Clock clock;
	uint32_t ms;
	uint8_t type;
	bool first;

	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar& this->clock;
		ar& this->ms;
		ar& this->type;
		ar& this->first;
	}
};