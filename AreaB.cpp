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


#include "AreaB.hpp"


AreaB::AreaB() = default;
AreaB::AreaB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* player) :
	Building(x, y, sx, sy, maxHp, player) {

}
bool AreaB::inRadius(GO *go) const {
	return this->inRadius(go->getX(), go->getY(), go->getSX(), go->getSY());
}
bool AreaB::inRadius(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) const {
	bool ok2 = true;
	for (uint32_t x2 = x; x2 <= x + sx; x2 = x2 + sx) {
		for (uint32_t y2 = y; y2 <= y + sy; y2 = y2 + sy) {

			bool ok1 = false;
			for (uint32_t x1 = this->getX(); x1 <= this->getX() + this->getSX(); x1 = x1 + this->getSX()) {
				for (uint32_t y1 = this->getY(); y1 <= this->getY() + this->getSY(); y1 = y1 + this->getSY()) {
					uint32_t dx = std::max(x1, x2) - std::min(x1, x2);
					uint32_t dy = std::max(y1, y1) - std::min(y1, y2);
					if (dx < this->getRadius() and dy < this->getRadius()) {
						ok1 = true;
						break;
					}
				}
				if (ok1) {
					break;
				}
			}

			if (!ok1) {
				ok2 = false;
				break;
			}
		}
		if (!ok2) {
			break;
		}
	}
	return ok2;
}
GEvent AreaB::getHighlightEvent() const {
	GEvent gEvent;

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
			gEvent.changeHighlight.emplace_back(this, x, y);
		}
	}

	return gEvent;
}