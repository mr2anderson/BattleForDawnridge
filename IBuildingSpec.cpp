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


#include "IBuildingSpec.hpp"



Events IBuildingSpec::getActiveNewMoveEvent(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, MapState* state, uint32_t playerId, const std::string &soundName, bool works) {
	return Events();
}
Events IBuildingSpec::getHighlightEvent(MapState* state, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, bool works, bool connectedToOrigin) {
	return Events();
}
Events IBuildingSpec::getEventOnDestroy(MapState* state, uint32_t playerId) const {
	return Events();
}
std::vector<HorizontalSelectionWindowComponent> IBuildingSpec::getComponents(MapState* state, uint32_t playerId, const std::string &soundName, bool works, bool connectedToOrigin) {
	return std::vector<HorizontalSelectionWindowComponent>();
}
std::optional<BuildingShortInfo> IBuildingSpec::getShortInfo(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy) const {
	return std::nullopt;
}
Resources IBuildingSpec::getLimit() const {
	return Resources();
}
bool IBuildingSpec::isVictoryCondition() const {
	return false;
}
bool IBuildingSpec::allowBuilding(MapState* state, uint32_t x1, uint32_t y1, uint32_t sx1, uint32_t sy1, uint32_t playerId, bool works, bool connectedToOrigin, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) {
	return false;
}
bool IBuildingSpec::isOrigin() const {
	return false;
}
bool IBuildingSpec::isActiveConductor(bool works) const {
	return false;
}
uint32_t IBuildingSpec::getWarriorMovementCost(uint32_t thisPlayerId, uint32_t playerId) const {
	return 1;
}
bool IBuildingSpec::warriorCanStay(uint32_t thisPlayerId, uint32_t playerId) const {
	return true;
}
bool IBuildingSpec::isUltraHighObstacle(uint32_t thisPlayerId, uint32_t playerId) const {
	return false;
}
bool IBuildingSpec::isHighObstacle(uint32_t thisPlayerId, uint32_t playerId) const {
	return false;
}
bool IBuildingSpec::isLowObstacle(uint32_t thisPlayerId, uint32_t playerId) const {
	return false;
}