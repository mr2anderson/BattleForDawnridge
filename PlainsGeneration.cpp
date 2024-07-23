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


#include "PlainsGeneration.hpp"
#include "GlobalRandomGenerator.hpp"


const uint32_t PlainsGeneration::TOTAL_PLAINS = 5;


PlainsGeneration::PlainsGeneration() {
	this->s1 = GlobalRandomGenerator::get()->gen();
	this->s2 = GlobalRandomGenerator::get()->gen();
}
uint32_t PlainsGeneration::getType(uint32_t i, uint32_t j) const {
	return (i * this->s1 + j * this->s2) % TOTAL_PLAINS + 1;
}