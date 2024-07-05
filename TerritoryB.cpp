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


#include "TerritoryB.hpp"


#pragma once


TerritoryB::TerritoryB() = default;
TerritoryB::TerritoryB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player *player) : 
	AreaB(x, y, sx, sy, maxHp, player),
	HpSensitiveB(x, y, sx, sy, maxHp, player),
	Building(x, y, sx, sy, maxHp, player) {

}
bool TerritoryB::allowBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, const Player& player) const {
	if (!this->works() or !this->belongTo(&player)) {
		return false;
	}
	return this->inRadius(x, y, sx, sy);
}
Event TerritoryB::getHighlightEvent() const {
	if (!this->works()) {
		return Event();
	}
	return this->AreaB::getHighlightEvent();
}