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


#include "AreaB.hpp"


AreaB::AreaB() = default;
AreaB::AreaB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, Player* player) :
	Building(x, y, sx, sy, maxHp, player) {

}
bool AreaB::inRadius(GO *go) const {
	return this->inRadius(go->getX(), go->getY(), go->getSX(), go->getSY());
}
bool AreaB::inRadius(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) const {
	sf::IntRect rect2;
	rect2.left = (int32_t)this->getX() - (int32_t)this->getRadius();
	rect2.top = (int32_t)this->getY() - (int32_t)this->getRadius();
	rect2.width = this->getSX() + 2 * this->getRadius() + 1;
	rect2.height = this->getSY() + 2 * this->getRadius() + 1 ;
	for (uint32_t i = x; i <= x + sx; i = i + sx) {
		for (uint32_t j = y; j <= y + sy; j = j + sy) {
			if (!rect2.contains(i, j)) {
				return false;
			}
		}
	}
	return true;
}
Event AreaB::getHighlightEvent() const {
	Event gEvent;
	if (!this->exist()) {
		return gEvent;
	}

	uint32_t x1, y1;
	uint32_t x2, y2;

	x1 = this->getX() + this->getSX() - 1 + this->getRadius();
	y1 = this->getY() + this->getSY() - 1 + this->getRadius();

	x2 = this->getX();
	y2 = this->getY();

	if (this->getX() > this->getRadius()) {
		x2 = this->getX() - this->getRadius();
	}
	else {
		x2 = 0;
	}

	if (this->getY() > this->getRadius()) {
		y2 = this->getY() - this->getRadius();
	}
	else {
		y2 = 0;
	}

	for (uint32_t x = x2; x <= x1; x = x + 1) {
		for (uint32_t y = y2; y <= y1; y = y + 1) {
			gEvent.addChangeHighlightEvent(std::make_tuple(this, x, y));
		}
	}

	return gEvent;
}