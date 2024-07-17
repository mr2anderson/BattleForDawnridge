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


#include "TerritoryB.hpp"


TerritoryB::TerritoryB() = default;
TerritoryB::TerritoryB(uint32_t x, uint32_t y, uint32_t player, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	AreaBLandscapeSensible(x, y, player, units, go, mapW, mapH),
	Building(x, y, player, units) {

}
bool TerritoryB::allowBuilding(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId) {
	if (!this->works() or !this->belongTo(playerId)) {
		return false;
	}
	return this->inRadius(x, y, sx, sy);
}
Events TerritoryB::getHighlightEvent() {
	if (!this->works()) {
		return Events();
	}
	return this->AreaBLandscapeSensible::getHighlightEvent();
}
bool TerritoryB::ignoreUltraHighObstacles() const {
	return false;
}
bool TerritoryB::ignoreHighObstacles() const {
	return false;
}
bool TerritoryB::ignoreLowObstacles() const {
	return false;
}