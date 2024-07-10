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


#include <vector>
#include <cstdint>


#pragma once


class PlainsGeneration {
public:
	PlainsGeneration();
	PlainsGeneration(uint32_t w, uint32_t h);

	uint32_t getType(uint32_t i, uint32_t j) const;
	uint32_t getW() const;
	uint32_t getH() const;

	static constexpr uint32_t TOTAL_PLAINS = 20;
private:
	std::vector<std::vector<uint32_t>> data;
};