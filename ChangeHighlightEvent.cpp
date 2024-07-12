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


#include "ChangeHighlightEvent.hpp"


ChangeHighlightEvent::ChangeHighlightEvent(const Unit* u, sf::Color color, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->u = u;
	this->color = color;
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
}
bool operator<(ChangeHighlightEvent a, ChangeHighlightEvent b) {
	if (a.u < b.u) {
		return true;
	}
	if (a.u > b.u) {
		return false;
	}

	if (a.color.r < b.color.r) {
		return true;
	}
	if (a.color.r > b.color.r) {
		return false;
	}

	if (a.color.g < b.color.g) {
		return true;
	}
	if (a.color.g > b.color.g) {
		return false;
	}

	if (a.color.b < b.color.b) {
		return true;
	}
	if (a.color.b > b.color.b) {
		return false;
	}

	if (a.x < b.x) {
		return true;
	}
	if (a.x > b.x) {
		return false;
	}

	if (a.y < b.y) {
		return true;
	}
	if (a.y > b.y) {
		return false;
	}

	if (a.sx < b.sx) {
		return true;
	}
	if (a.sx > b.sx) {
		return false;
	}

	if (a.sy < b.sy) {
		return true;
	}
	return false;
}
const Unit* ChangeHighlightEvent::getUnit() const {
	return this->u;
}
sf::Color ChangeHighlightEvent::getColor() const {
	return this->color;
}
uint32_t ChangeHighlightEvent::getX() const {
	return this->x;
}
uint32_t ChangeHighlightEvent::getY() const {
	return this->y;
}
uint32_t ChangeHighlightEvent::getSX() const {
	return this->sx;
}
uint32_t ChangeHighlightEvent::getSY() const {
	return this->sy;
}