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


#include "HighlightTable.hpp"


HighlightTable::HighlightTable() = default;
void HighlightTable::clear() {
	this->data.clear();
}
void HighlightTable::mark(uint32_t x, uint32_t y, const Unit* unitPtr) {
	std::tuple<uint32_t, uint32_t> p = std::make_tuple(x, y);
	std::vector<const Unit*> v = this->data[p];
	bool found = false;
	for (uint32_t i = 0; i < v.size(); i = i + 1) {
		if (v[i] == unitPtr) {
			v.erase(v.begin() + i);
			found = true;
			break;
		}
	}
	if (!found) {
		v.push_back(unitPtr);
	}
	this->data[p] = v;
}
bool HighlightTable::highlighted(uint32_t x, uint32_t y) const {
	std::tuple<uint32_t, uint32_t> p = std::make_tuple(x, y);
	if (this->data.find(p) == this->data.end()) {
		return false;
	}
	return !this->data.at(p).empty();
}