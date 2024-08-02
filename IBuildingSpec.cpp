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


IBuildingSpec::~IBuildingSpec() = default;
Events IBuildingSpec::getActiveNewMoveEvent(const Building *building, MapState* state) {
	return Events();
}
Events IBuildingSpec::getHighlightEvent(const Building *building, MapState* state, uint8_t type) {
	return Events();
}
Events IBuildingSpec::getEventOnDestroy(const Building *building, MapState* state) const {
	return Events();
}
std::vector<BuildingHorizontalSelectionWindowComponent> IBuildingSpec::getComponents(const Building *building, MapState* state) {
	return std::vector<BuildingHorizontalSelectionWindowComponent>();
}
std::optional<BuildingShortInfo> IBuildingSpec::getShortInfo(const Building *building) const {
	return std::nullopt;
}
Resources IBuildingSpec::getLimit(const Building *building) const {
	return Resources();
}
uint32_t IBuildingSpec::getPopulationLimit(const Building *building) const {
	return 0;
}
bool IBuildingSpec::isVictoryCondition() const {
	return false;
}
bool IBuildingSpec::allowBuilding(const Building *building, MapState* state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2) {
	return false;
}
bool IBuildingSpec::isOrigin() const {
	return false;
}
bool IBuildingSpec::isActiveConductor(const Building *building) const {
	return false;
}
uint32_t IBuildingSpec::getWarriorMovementCost(const Building *building, const Warrior *w) const {
	return 1;
}
bool IBuildingSpec::warriorCanStay(const Building *building, const Warrior *w) const {
	return true;
}
bool IBuildingSpec::isUltraHighObstacle(const Building *building, uint32_t playerId) const {
	return false;
}
bool IBuildingSpec::isHighObstacle(const Building *building, uint32_t playerId) const {
	return false;
}
bool IBuildingSpec::isLowObstacle(const Building *building, uint32_t playerId) const {
	return false;
}
uint8_t IBuildingSpec::getNewMoveMainPriority() const {
    return 0;
}


BOOST_CLASS_EXPORT_IMPLEMENT(IBuildingSpec)