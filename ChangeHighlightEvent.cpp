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


ChangeHighlightEvent::ChangeHighlightEvent(const Unit* u, uint32_t x, uint32_t y) {
	this->u = u;
	this->x = x;
	this->y = y;
}
const Unit* ChangeHighlightEvent::getUnit() const {
	return this->u;
}
uint32_t ChangeHighlightEvent::getX() const {
	return this->x;
}
uint32_t ChangeHighlightEvent::getY() const {
	return this->y;
}