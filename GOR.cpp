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


#include "GOR.hpp"


GOR::GOR() = default;
GOR operator+(const GOR& a, const GOR& b) {
	GOR c = a;

	std::queue<PopUpElement*> elements = b.elements;
	while (!elements.empty()) {
		c.elements.push(elements.front());
		elements.pop();
	}
	c.events.insert(c.events.end(), b.events.begin(), b.events.end());

	return c;
}