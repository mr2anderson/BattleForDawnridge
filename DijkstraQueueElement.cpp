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


#include "DijkstraQueueElement.hpp"


DijkstraQueueElement::DijkstraQueueElement() = default;
DijkstraQueueElement::DijkstraQueueElement(uint64_t dst, uint32_t x, uint32_t y) {
	this->dst = dst;
	this->x = x;
	this->y = y;
}
DijkstraQueueElement::DijkstraQueueElement(uint64_t dst, std::tuple<uint32_t, uint32_t> p) : DijkstraQueueElement(dst, std::get<0>(p), std::get<1>(p)) {

}
bool operator>(DijkstraQueueElement a, DijkstraQueueElement b) {
	if (a.dst > b.dst) {
		return true;
	}
	if (a.dst < b.dst) {
		return false;
	}

	if (a.x > b.x) {
		return true;
	}
	if (a.x < b.x) {
		return false;
	}
	
	if (a.y > b.y) {
		return true;
	}
	return false;
}