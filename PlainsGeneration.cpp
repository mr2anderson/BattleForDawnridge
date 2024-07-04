/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "PlainsGeneration.hpp"


PlainsGeneration::PlainsGeneration() = default;
PlainsGeneration::PlainsGeneration(uint32_t w, uint32_t h) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	this->data.resize(w);
	for (uint32_t i = 0; i < w; i = i + 1) {
		this->data[i].resize(h);
		for (uint32_t j = 0; j < h; j = j + 1) {
			this->data[i][j] = mersenne() % TOTAL_PLAINS;
		}
	}
}
uint32_t PlainsGeneration::getType(uint32_t i, uint32_t j) const {
	return this->data[i][j];
}
uint32_t PlainsGeneration::getW() const {
	return this->data.size();
}
uint32_t PlainsGeneration::getH() const {
	return this->data[0].size();
}