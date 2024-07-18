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


#include "SetHighlightEvent.hpp"


SetHighlightEvent::SetHighlightEvent(sf::Color color, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->color = color;
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;
}
bool operator<(SetHighlightEvent a, SetHighlightEvent b) {
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
sf::Color SetHighlightEvent::getColor() const {
	return this->color;
}
uint32_t SetHighlightEvent::getX() const {
	return this->x;
}
uint32_t SetHighlightEvent::getY() const {
	return this->y;
}
uint32_t SetHighlightEvent::getSX() const {
	return this->sx;
}
uint32_t SetHighlightEvent::getSY() const {
	return this->sy;
}